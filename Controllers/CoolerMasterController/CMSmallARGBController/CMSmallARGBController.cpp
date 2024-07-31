/*---------------------------------------------------------*\
| CMSmallARGBController.cpp                                 |
|                                                           |
|   Driver for Cooler Master Small ARGB controller          |
|                                                           |
|   Chris M (Dr_No)                             31 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "CMSmallARGBController.h"
#include "StringUtils.h"

cm_small_argb_headers cm_small_argb_header_data[1] =
{
    { "CM Small ARGB",  0x01, true,  12 }
};

CMSmallARGBController::CMSmallARGBController(hid_device* dev_handle, char *_path, unsigned char _zone_idx)
{
    dev                     = dev_handle;
    location                = _path;
    zone_index              = _zone_idx;
    current_speed           = CM_SMALL_ARGB_SPEED_NORMAL;

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

CMSmallARGBController::~CMSmallARGBController()
{
    if(dev)
    {
        hid_close(dev);
    }
}

void CMSmallARGBController::GetStatus()
{
    unsigned char buffer[CM_SMALL_ARGB_PACKET_SIZE]     = { 0x00, 0x80, 0x01, 0x01 };
    int buffer_size                                     = (sizeof(buffer) / sizeof(buffer[0]));
    int header                                          = zone_index - 1;

    buffer[CM_SMALL_ARGB_ZONE_BYTE]                     = header;
    buffer[CM_SMALL_ARGB_MODE_BYTE]                     = 0x01;
    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_SMALL_ARGB_INTERRUPT_TIMEOUT);

    memset(buffer, 0x00, buffer_size );

    buffer[CM_SMALL_ARGB_COMMAND_BYTE]                  = 0x0B;
    buffer[CM_SMALL_ARGB_FUNCTION_BYTE]                 = 0x01;
    buffer[CM_SMALL_ARGB_ZONE_BYTE]                     = 0x01;
    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_SMALL_ARGB_INTERRUPT_TIMEOUT);

    current_mode                                        = buffer[4];
    current_speed                                       = buffer[5];
    bool_random                                         = buffer[6] == 0x00;
    current_brightness                                  = buffer[7];
    current_red                                         = buffer[8];
    current_green                                       = buffer[9];
    current_blue                                        = buffer[10];
}

std::string CMSmallARGBController::GetDeviceName()
{
    return(device_name);
}

std::string CMSmallARGBController::GetSerial()
{
    wchar_t serial_string[HID_MAX_STR];
    int ret = hid_get_serial_number_string(dev, serial_string, HID_MAX_STR);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string CMSmallARGBController::GetLocation()
{
    return("HID: " + location);
}

unsigned char CMSmallARGBController::GetZoneIndex()
{
    return(zone_index);
}

unsigned char CMSmallARGBController::GetMode()
{
    return(current_mode);
}

unsigned char CMSmallARGBController::GetLedRed()
{
    return(current_red);
}

unsigned char CMSmallARGBController::GetLedGreen()
{
    return(current_green);
}

unsigned char CMSmallARGBController::GetLedBlue()
{
    return(current_blue);
}

unsigned char CMSmallARGBController::GetLedSpeed()
{
    return(current_speed);
}

bool CMSmallARGBController::GetRandomColours()
{
    return(bool_random);
}

void CMSmallARGBController::SetLedCount(int zone, int led_count)
{
    unsigned char buffer[CM_SMALL_ARGB_PACKET_SIZE]     = { 0x00, 0x80, 0x0D, 0x02 };
    int  buffer_size                                    = (sizeof(buffer) / sizeof(buffer[0]));

    buffer[CM_SMALL_ARGB_ZONE_BYTE]                     = zone;
    buffer[CM_SMALL_ARGB_MODE_BYTE]                     = (0x0F - led_count > 0) ? 0x0F - led_count : 0x01;
    buffer[CM_SMALL_ARGB_SPEED_BYTE]                    = led_count;

    hid_write(dev, buffer, buffer_size);
}

void CMSmallARGBController::SetMode(unsigned char mode, unsigned char speed, unsigned char brightness, RGBColor colour, bool random_colours)
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

void CMSmallARGBController::SetLedsDirect(RGBColor* led_colours, unsigned int led_count)
{
    const unsigned char buffer_size     = CM_SMALL_ARGB_PACKET_SIZE;
    unsigned char buffer[buffer_size]   = { 0x00, 0x00, 0x10, 0x02 };
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

    buffer[CM_SMALL_ARGB_ZONE_BYTE]     = zone_index - 1; //argb_header_data[zone_index].header;
    buffer[CM_SMALL_ARGB_MODE_BYTE]     = led_count;
    unsigned char buffer_idx            = CM_SMALL_ARGB_MODE_BYTE + 1;

    for(std::vector<unsigned char>::iterator it = colours.begin(); it != colours.end(); buffer_idx = CM_SMALL_ARGB_COMMAND_BYTE)
    {
        /*-----------------------------------------------------------------*\
        | Fill the write buffer till its full or the colour buffer is empty |
        \*-----------------------------------------------------------------*/
        buffer[CM_SMALL_ARGB_REPORT_BYTE] = packet_count;
        while (( buffer_idx < buffer_size) && ( it != colours.end() ))
        {
            buffer[buffer_idx] = *it;
            buffer_idx++;
            it++;
        }

        if(it == colours.end())
        {
            buffer[CM_SMALL_ARGB_REPORT_BYTE] += 0x80;
        }

        hid_write(dev, buffer, buffer_size);

        /*-----------------------------------------------------------------*\
        | Reset the write buffer                                            |
        \*-----------------------------------------------------------------*/
        memset(buffer, 0x00, buffer_size );
        packet_count++;
    }
}

void CMSmallARGBController::SendUpdate()
{
    unsigned char buffer[CM_SMALL_ARGB_PACKET_SIZE]     = { 0x00 };
    int  buffer_size                                    = (sizeof(buffer) / sizeof(buffer[0]));
    bool boolPassthru                                   = ( current_mode == CM_SMALL_ARGB_MODE_PASSTHRU );
    bool boolDirect                                     = ( current_mode == CM_SMALL_ARGB_MODE_DIRECT );
    unsigned char function                              = boolPassthru ? 0x02 : 0x01;
    buffer[CM_SMALL_ARGB_REPORT_BYTE]                   = 0x80;
    buffer[CM_SMALL_ARGB_COMMAND_BYTE]                  = boolDirect   ? 0x10 : 0x01;
    buffer[CM_SMALL_ARGB_FUNCTION_BYTE]                 = boolDirect   ? 0x01 : function;
    buffer[CM_SMALL_ARGB_MODE_BYTE]                     = boolPassthru ? 0x00 : 0x02;

    hid_write(dev, buffer, buffer_size);

    buffer[CM_SMALL_ARGB_COMMAND_BYTE]                  = 0x0b;
    buffer[CM_SMALL_ARGB_FUNCTION_BYTE]                 = (false) ? 0x01 : 0x02; //This controls custom mode TODO
    buffer[CM_SMALL_ARGB_ZONE_BYTE]                     = cm_small_argb_header_data[zone_index].header;
    buffer[CM_SMALL_ARGB_MODE_BYTE]                     = current_mode;
    buffer[CM_SMALL_ARGB_SPEED_BYTE]                    = current_speed;
    buffer[CM_SMALL_ARGB_COLOUR_INDEX_BYTE]             = (bool_random) ? 0x00 : 0x10; //This looks to still be the colour index and controls random colours
    buffer[CM_SMALL_ARGB_BRIGHTNESS_BYTE]               = current_brightness;
    buffer[CM_SMALL_ARGB_RED_BYTE]                      = current_red;
    buffer[CM_SMALL_ARGB_GREEN_BYTE]                    = current_green;
    buffer[CM_SMALL_ARGB_BLUE_BYTE]                     = current_blue;

    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_SMALL_ARGB_INTERRUPT_TIMEOUT);
}
