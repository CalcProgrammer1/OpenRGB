/*---------------------------------------------------------*\
| CMARGBController.cpp                                      |
|                                                           |
|   Driver for Cooler Master ARGB controller                |
|                                                           |
|   Chris M (Dr_No)                             10 Oct 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "CMARGBController.h"
#include "StringUtils.h"

CMARGBController::CMARGBController(hid_device* dev_handle, char *_path, unsigned char _zone_idx, std::shared_ptr<std::mutex> cm_mutex)
{
    dev                     = dev_handle;
    location                = _path;
    zone_index              = _zone_idx;
    mutex_ptr               = cm_mutex;

    /*---------------------------------------------------------*\
    | Get device name from HID manufacturer and product strings |
    \*---------------------------------------------------------*/
    wchar_t name_string[HID_MAX_STR];

    hid_get_manufacturer_string(dev, name_string, HID_MAX_STR);
    device_name = StringUtils::wstring_to_string(name_string);

    hid_get_product_string(dev, name_string, HID_MAX_STR);
    device_name.append(" ").append(StringUtils::wstring_to_string(name_string));

    GetStatus();
}

CMARGBController::~CMARGBController()
{
    if(mutex_ptr.use_count() <= 1)
    {
        hid_close(dev);
    }
}

void CMARGBController::GetStatus()
{
    unsigned char buffer[CM_ARGB_PACKET_SIZE]   = { 0x00, 0x80, 0x0B, 0x01 };
    int buffer_size                             = (sizeof(buffer) / sizeof(buffer[0]));
    int rgb_offset                              = 0;
    int zone;

    if (argb_header_data[zone_index].digital)
    {
        zone                                    = argb_header_data[zone_index].header;
        buffer[CM_ARGB_COMMAND_BYTE]            = 0x0B;
    }
    else
    {
        zone                                    = 0x00;
        buffer[CM_ARGB_COMMAND_BYTE]            = 0x0A;
        rgb_offset                              = 1;
    }

    /*---------------------------------------------*\
    | Guard the writes to the controller until the  |
    |   for loop has completed to avoid collisons   |
    \*---------------------------------------------*/
    std::lock_guard<std::mutex> guard(*mutex_ptr);

    /*---------------------------------------------------------*\
    | If this is the group then just return the first status    |
    \*---------------------------------------------------------*/
    buffer[CM_ARGB_ZONE_BYTE]                   = ( zone > 0x08 ) ? 0x01 : zone;
    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_ARGB_INTERRUPT_TIMEOUT);

    current_mode                                = buffer[4 - rgb_offset];
    bool_random                                 = ( buffer[5 - rgb_offset] == 0x00 );
    current_speed                               = buffer[6 - rgb_offset];
    current_brightness                          = buffer[7 - rgb_offset];
    current_red                                 = buffer[8 - rgb_offset];
    current_green                               = buffer[9 - rgb_offset];
    current_blue                                = buffer[10 - rgb_offset];
}

std::string CMARGBController::GetDeviceName()
{
    return(device_name);
}

std::string CMARGBController::GetSerial()
{
    wchar_t serial_string[HID_MAX_STR];
    int ret = hid_get_serial_number_string(dev, serial_string, HID_MAX_STR);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string CMARGBController::GetLocation()
{
    return("HID: " + location);
}

unsigned char CMARGBController::GetZoneIndex()
{
    return(zone_index);
}

unsigned char CMARGBController::GetMode()
{
    return(current_mode);
}

unsigned char CMARGBController::GetLedRed()
{
    return(current_red);
}

unsigned char CMARGBController::GetLedGreen()
{
    return(current_green);
}

unsigned char CMARGBController::GetLedBlue()
{
    return(current_blue);
}

unsigned char CMARGBController::GetLedSpeed()
{
    return(current_speed);
}

bool CMARGBController::GetRandomColours()
{
    return(bool_random);
}

void CMARGBController::SetLedCount(int zone, int led_count)
{
    unsigned char buffer[CM_ARGB_PACKET_SIZE]   = { 0x00, 0x80, 0x0D, 0x02 };
    int  buffer_size                            = (sizeof(buffer) / sizeof(buffer[0]));

    buffer[CM_ARGB_ZONE_BYTE]                   = zone;
    buffer[CM_ARGB_MODE_BYTE]                   = led_count;
    buffer[CM_ARGB_COLOUR_INDEX_BYTE]           = (0x0F - led_count > 0) ? 0x0F - led_count : 0x01;

    /*---------------------------------------------*\
    | Guard the writes to the controller until the  |
    |   for loop has completed to avoid collisons   |
    \*---------------------------------------------*/
    std::lock_guard<std::mutex> guard(*mutex_ptr);

    hid_write(dev, buffer, buffer_size);
}

void CMARGBController::SetMode(uint8_t mode, uint8_t speed, uint8_t brightness, RGBColor colour, bool random_colours)
{
    bool needs_update   = !( (current_mode == mode) && (current_speed == speed) && (current_brightness == brightness) && (ToRGBColor(current_red, current_green, current_blue) == colour));

    if (needs_update)
    {
        current_mode        = mode;
        current_speed       = speed;
        current_brightness  = brightness;
        current_red         = RGBGetRValue(colour);
        current_green       = RGBGetGValue(colour);
        current_blue        = RGBGetBValue(colour);
        bool_random         = random_colours;

        SendUpdate();
    }
}

void CMARGBController::SetLedsDirect(RGBColor *led_colours, unsigned int led_count)
{
    const unsigned char buffer_size     = CM_ARGB_PACKET_SIZE;
    unsigned char buffer[buffer_size]   = { 0x00, 0x00, 0x07, 0x02 };
    unsigned char packet_count          = 0;
    std::vector<uint8_t> colours;

    /*---------------------------------------------*\
    | Set up the RGB triplets to send               |
    \*---------------------------------------------*/
    for(unsigned int i = 0; i < led_count; i++)
    {
        RGBColor      colour            = led_colours[i];

        colours.push_back( RGBGetRValue(colour) );
        colours.push_back( RGBGetGValue(colour) );
        colours.push_back( RGBGetBValue(colour) );
    }

    buffer[CM_ARGB_FUNCTION_BYTE]       = zone_index - 1;
    buffer[CM_ARGB_ZONE_BYTE]           = led_count;
    unsigned char buffer_idx            = CM_ARGB_MODE_BYTE;

    /*---------------------------------------------*\
    | Guard the writes to the controller until the  |
    |   for loop has completed to avoid collisons   |
    \*---------------------------------------------*/
    std::lock_guard<std::mutex> guard(*mutex_ptr);

    for(std::vector<unsigned char>::iterator it = colours.begin(); it != colours.end(); buffer_idx = CM_ARGB_COMMAND_BYTE)
    {
        /*-----------------------------------------------------------------*\
        | Fill the write buffer till its full or the colour buffer is empty |
        \*-----------------------------------------------------------------*/
        buffer[CM_ARGB_REPORT_BYTE] = packet_count;
        while (( buffer_idx < buffer_size) && ( it != colours.end() ))
        {
            buffer[buffer_idx] = *it;
            buffer_idx++;
            it++;
        }

        if(it == colours.end())
        {
            buffer[CM_ARGB_REPORT_BYTE] += 0x80;
        }

        hid_write(dev, buffer, buffer_size);

        /*-----------------------------------------------------------------*\
        | Reset the write buffer                                            |
        \*-----------------------------------------------------------------*/
        memset(buffer, 0x00, buffer_size );
        packet_count++;
    }
}

void CMARGBController::SendUpdate()
{
    /*---------------------------------------------*\
    | Guard the writes to the controller            |
    \*---------------------------------------------*/
    std::lock_guard<std::mutex> guard(*mutex_ptr);

    unsigned char buffer[CM_ARGB_PACKET_SIZE]   = { 0x00 };
    int  buffer_size                            = (sizeof(buffer) / sizeof(buffer[0]));
    bool boolARGB_header                        = argb_header_data[zone_index].digital;
    bool boolPassthru                           = ( current_mode == CM_ARGB_MODE_PASSTHRU ) || ( current_mode == CM_RGB_MODE_PASSTHRU );
    bool boolDirect                             = ( current_mode == CM_ARGB_MODE_DIRECT );
    unsigned char function                      = boolPassthru ? (boolARGB_header ? 0x02 : 0x04) : (boolARGB_header ? 0x01 : 0x03);
    buffer[CM_ARGB_REPORT_BYTE]                 = 0x80;
    buffer[CM_ARGB_COMMAND_BYTE]                = 0x01;

    if(boolDirect)
    {
        buffer[CM_ARGB_FUNCTION_BYTE]           = 0x01;
        buffer[CM_ARGB_ZONE_BYTE]               = 0x02;

        hid_write(dev, buffer, buffer_size);
        hid_read_timeout(dev, buffer, buffer_size, CM_ARGB_INTERRUPT_TIMEOUT);

        /*-----------------------------------------------------------------*\
        | Direct mode is now set up and no other mode packet is required    |
        \*-----------------------------------------------------------------*/
        return;
    }

    buffer[CM_ARGB_FUNCTION_BYTE]           = function;

    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_ARGB_INTERRUPT_TIMEOUT);

    if(boolARGB_header)
    {
        buffer[CM_ARGB_COMMAND_BYTE]            = 0x0B;                                     //ARGB sends 0x0B (1011) RGB sends 0x04 (0100)
        buffer[CM_ARGB_FUNCTION_BYTE]           = (false) ? 0x01 : 0x02;                    //This controls direct mode TODO
        buffer[CM_ARGB_ZONE_BYTE]               = argb_header_data[zone_index].header;
        buffer[CM_ARGB_MODE_BYTE]               = current_mode;
        buffer[CM_ARGB_COLOUR_INDEX_BYTE]       = bool_random ? 0x00 : 0x10;
        buffer[CM_ARGB_SPEED_BYTE]              = current_speed;
        buffer[CM_ARGB_BRIGHTNESS_BYTE]         = current_brightness;
        buffer[CM_ARGB_RED_BYTE]                = current_red;
        buffer[CM_ARGB_GREEN_BYTE]              = current_green;
        buffer[CM_ARGB_BLUE_BYTE]               = current_blue;
    }
    else
    {
        buffer[CM_ARGB_COMMAND_BYTE]                        = boolPassthru ? 0x01 : 0x04;   //ARGB sends 0x0b (1011) RGB sends 0x04 (0100)
        buffer[CM_ARGB_MODE_BYTE + CM_RGB_OFFSET]           = current_mode;
        buffer[CM_ARGB_COLOUR_INDEX_BYTE + CM_RGB_OFFSET]   = bool_random ? 0x00 : 0x10;
        buffer[CM_ARGB_SPEED_BYTE + CM_RGB_OFFSET]          = current_speed;
        buffer[CM_ARGB_BRIGHTNESS_BYTE + CM_RGB_OFFSET]     = current_brightness;
        buffer[CM_ARGB_RED_BYTE + CM_RGB_OFFSET]            = current_red;
        buffer[CM_ARGB_GREEN_BYTE + CM_RGB_OFFSET]          = current_green;
        buffer[CM_ARGB_BLUE_BYTE + CM_RGB_OFFSET]           = current_blue;
    }

    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_ARGB_INTERRUPT_TIMEOUT);
}
