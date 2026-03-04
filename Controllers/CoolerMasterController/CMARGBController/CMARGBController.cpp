/*---------------------------------------------------------*\
| CMARGBController.cpp                                      |
|                                                           |
|   Driver for Cooler Master ARGB controller                |
|                                                           |
|   Chris M (Dr_No)                             10 Oct 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "CMARGBController.h"
#include "StringUtils.h"

/*---------------------------------------------------------*\
| Map to convert port index to port ID used in protocol     |
\*---------------------------------------------------------*/
static unsigned char cm_argb_port_index_to_id[5] =
{
    CM_ARGB_PORT_ARGB_1,
    CM_ARGB_PORT_ARGB_2,
    CM_ARGB_PORT_ARGB_3,
    CM_ARGB_PORT_ARGB_4,
    CM_ARGB_PORT_RGB
};

CMARGBController::CMARGBController(hid_device* dev_handle, char *path)
{
    dev         = dev_handle;
    location    = path;

    /*-----------------------------------------------------*\
    | Get device name from HID manufacturer and product     |
    | strings                                               |
    \*-----------------------------------------------------*/
    wchar_t name_string[HID_MAX_STR];

    hid_get_manufacturer_string(dev, name_string, HID_MAX_STR);
    device_name = StringUtils::wstring_to_string(name_string);

    hid_get_product_string(dev, name_string, HID_MAX_STR);
    device_name.append(" ").append(StringUtils::wstring_to_string(name_string));
}

CMARGBController::~CMARGBController()
{
    hid_close(dev);
}

std::string CMARGBController::GetDeviceName()
{
    return(device_name);
}

std::string CMARGBController::GetLocation()
{
    return("HID: " + location);
}

std::string CMARGBController::GetVersion()
{
    /*-----------------------------------------------------*\
    | This device uses the serial value to determine the    |
    | version.  It does not report a proper unique serial.  |
    \*-----------------------------------------------------*/
    std::string serial_string = GetSerial();

    if(serial_string == CM_ARGB_FW0023)
    {
        return("0023");
    }
    else if(serial_string == CM_ARGB_FW0028)
    {
        return("0028");
    }
    else
    {
        return("Unsupported");
    }
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

void CMARGBController::GetPortStatus
    (
    unsigned char   port_idx,
    unsigned char*  port_mode,
    unsigned char*  port_speed,
    unsigned char*  port_brightness,
    bool*           port_random,
    unsigned char*  port_red,
    unsigned char*  port_green,
    unsigned char*  port_blue
    )
{
    unsigned char buffer[CM_ARGB_PACKET_SIZE]   = {0x00, 0x80, 0x0B, 0x01};
    int buffer_size                             = (sizeof(buffer) / sizeof(buffer[0]));
    int rgb_offset                              = 0;
    int zone;

    /*-----------------------------------------------------*\
    | RGB port is handled differently from ARGB ports       |
    \*-----------------------------------------------------*/
    if(cm_argb_port_index_to_id[port_idx] != CM_ARGB_PORT_RGB)
    {
        zone                                    = cm_argb_port_index_to_id[port_idx];
        buffer[CM_ARGB_COMMAND_BYTE]            = 0x0B;
    }
    else
    {
        zone                                    = 0x00;
        buffer[CM_ARGB_COMMAND_BYTE]            = 0x0A;
        rgb_offset                              = 1;
    }

    /*-----------------------------------------------------*\
    | If this is the group then just return the first       |
    | status                                                |
    \*-----------------------------------------------------*/
    buffer[CM_ARGB_ZONE_BYTE]                   = ( zone > 0x08 ) ? 0x01 : zone;

    /*-----------------------------------------------------*\
    | Send the command and read the response                |
    \*-----------------------------------------------------*/
    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_ARGB_INTERRUPT_TIMEOUT);

    /*-----------------------------------------------------*\
    | Read data out of response                             |
    \*-----------------------------------------------------*/
    *port_mode                                  = buffer[4 - rgb_offset];
    *port_random                                = (buffer[5 - rgb_offset] == 0x00);
    *port_speed                                 = buffer[6 - rgb_offset];
    *port_brightness                            = buffer[7 - rgb_offset];
    *port_red                                   = buffer[8 - rgb_offset];
    *port_green                                 = buffer[9 - rgb_offset];
    *port_blue                                  = buffer[10 - rgb_offset];
}

void CMARGBController::SetPortLEDCount(unsigned char port_idx, unsigned char led_count)
{
    unsigned char buffer[CM_ARGB_PACKET_SIZE]   = {0x00, 0x80, 0x0D, 0x02};
    int  buffer_size                            = (sizeof(buffer) / sizeof(buffer[0]));

    buffer[CM_ARGB_ZONE_BYTE]                   = cm_argb_port_index_to_id[port_idx];
    buffer[CM_ARGB_MODE_BYTE]                   = led_count;
    buffer[CM_ARGB_COLOUR_INDEX_BYTE]           = 1;

    /*-----------------------------------------------------*\
    | Send the command                                      |
    \*-----------------------------------------------------*/
    hid_write(dev, buffer, buffer_size);
}

void CMARGBController::SetPortMode
    (
    unsigned char   port_idx,
    unsigned char   port_mode,
    unsigned char   port_speed,
    unsigned char   port_brightness,
    bool            port_random,
    unsigned char   port_red,
    unsigned char   port_green,
    unsigned char   port_blue
    )
{
    unsigned char buffer[CM_ARGB_PACKET_SIZE]   = {0x00};
    int  buffer_size                            = (sizeof(buffer) / sizeof(buffer[0]));
    bool boolARGB_header                        = (cm_argb_port_index_to_id[port_idx] != CM_ARGB_PORT_RGB);
    bool boolPassthru                           = (port_mode == CM_ARGB_MODE_PASSTHRU) || (port_mode == CM_RGB_MODE_PASSTHRU);
    bool boolDirect                             = (port_mode == CM_ARGB_MODE_DIRECT);
    unsigned char function                      = boolPassthru ? (boolARGB_header ? 0x02 : 0x04) : (boolARGB_header ? 0x01 : 0x03);
    buffer[CM_ARGB_REPORT_BYTE]                 = 0x80;
    buffer[CM_ARGB_COMMAND_BYTE]                = 0x01;

    if(boolDirect)
    {
        buffer[CM_ARGB_FUNCTION_BYTE]           = 0x01;
        buffer[CM_ARGB_ZONE_BYTE]               = 0x02;

        /*-------------------------------------------------*\
        | Send the command                                  |
        \*-------------------------------------------------*/
        hid_write(dev, buffer, buffer_size);

        /*-------------------------------------------------*\
        | Direct mode is now set up and no other mode       |
        | packet is required                                |
        \*-------------------------------------------------*/
        return;
    }

    buffer[CM_ARGB_FUNCTION_BYTE]               = function;

    /*-----------------------------------------------------*\
    | Send the command                                      |
    \*-----------------------------------------------------*/
    hid_write(dev, buffer, buffer_size);

    /*-----------------------------------------------------*\
    | ARGB ports send command 0x0B, RGB port sends 0x04     |
    \*-----------------------------------------------------*/
    if(boolARGB_header)
    {
        buffer[CM_ARGB_COMMAND_BYTE]                        = 0x0B;
        buffer[CM_ARGB_FUNCTION_BYTE]                       = (false) ? 0x01 : 0x02;
        buffer[CM_ARGB_ZONE_BYTE]                           = cm_argb_port_index_to_id[port_idx];
        buffer[CM_ARGB_MODE_BYTE]                           = port_mode;
        buffer[CM_ARGB_COLOUR_INDEX_BYTE]                   = port_random ? 0x00 : 0x10;
        buffer[CM_ARGB_SPEED_BYTE]                          = port_speed;
        buffer[CM_ARGB_BRIGHTNESS_BYTE]                     = port_brightness;
        buffer[CM_ARGB_RED_BYTE]                            = port_red;
        buffer[CM_ARGB_GREEN_BYTE]                          = port_green;
        buffer[CM_ARGB_BLUE_BYTE]                           = port_blue;
    }
    else
    {
        buffer[CM_ARGB_COMMAND_BYTE]                        = boolPassthru ? 0x01 : 0x04;
        buffer[CM_ARGB_MODE_BYTE + CM_RGB_OFFSET]           = port_mode;
        buffer[CM_ARGB_COLOUR_INDEX_BYTE + CM_RGB_OFFSET]   = port_random ? 0x00 : 0x10;
        buffer[CM_ARGB_SPEED_BYTE + CM_RGB_OFFSET]          = port_speed;
        buffer[CM_ARGB_BRIGHTNESS_BYTE + CM_RGB_OFFSET]     = port_brightness;
        buffer[CM_ARGB_RED_BYTE + CM_RGB_OFFSET]            = port_red;
        buffer[CM_ARGB_GREEN_BYTE + CM_RGB_OFFSET]          = port_green;
        buffer[CM_ARGB_BLUE_BYTE + CM_RGB_OFFSET]           = port_blue;
    }

    /*-----------------------------------------------------*\
    | Send the command and wait for response                |
    \*-----------------------------------------------------*/
    hid_write(dev, buffer, buffer_size);
}

void CMARGBController::SetPortLEDsDirect(unsigned char port_idx, RGBColor *led_colours, unsigned int led_count)
{
    const unsigned char buffer_size     = CM_ARGB_PACKET_SIZE;
    unsigned char buffer[buffer_size]   = { 0x00, 0x00, 0x07, 0x02 };
    unsigned char packet_count          = 0;
    std::vector<uint8_t> colours;

    /*-----------------------------------------------------*\
    | Set up the RGB triplets to send                       |
    \*-----------------------------------------------------*/
    for(unsigned int i = 0; i < led_count; i++)
    {
        RGBColor      colour            = led_colours[i];

        colours.push_back(RGBGetRValue(colour));
        colours.push_back(RGBGetGValue(colour));
        colours.push_back(RGBGetBValue(colour));
    }

    buffer[CM_ARGB_FUNCTION_BYTE]       = port_idx;
    buffer[CM_ARGB_ZONE_BYTE]           = led_count;
    unsigned char buffer_idx            = CM_ARGB_MODE_BYTE;

    for(std::vector<unsigned char>::iterator it = colours.begin(); it != colours.end(); buffer_idx = CM_ARGB_COMMAND_BYTE)
    {
        /*-------------------------------------------------*\
        | Fill the write buffer till its full or the        |
        | colour buffer is empty                            |
        \*-------------------------------------------------*/
        buffer[CM_ARGB_REPORT_BYTE] = packet_count;
        while((buffer_idx < buffer_size) && (it != colours.end()))
        {
            buffer[buffer_idx] = *it;
            buffer_idx++;
            it++;
        }

        if(it == colours.end())
        {
            buffer[CM_ARGB_REPORT_BYTE] += 0x80;
        }

        /*-------------------------------------------------*\
        | Send the buffer                                   |
        \*-------------------------------------------------*/
        hid_write(dev, buffer, buffer_size);

        /*-------------------------------------------------*\
        | Reset the write buffer                            |
        \*-------------------------------------------------*/
        memset(buffer, 0x00, buffer_size );
        packet_count++;
    }
}
