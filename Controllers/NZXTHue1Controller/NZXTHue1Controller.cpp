/*---------------------------------------------------------*\
| NZXTHue1Controller.cpp                                    |
|                                                           |
|   Driver for NZXT Hue 1 (Smart Device V1)                 |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      16 Apr 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "NZXTHue1Controller.h"
#include "StringUtils.h"

NZXTHue1Controller::NZXTHue1Controller(hid_device* dev_handle, unsigned int /*fan_channels*/, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;

    Initialize();
}

NZXTHue1Controller::~NZXTHue1Controller()
{

}

std::string NZXTHue1Controller::GetFirmwareVersion()
{
    return(firmware_version);
}

std::string NZXTHue1Controller::GetLocation()
{
    return("HID: " + location);
}

std::string NZXTHue1Controller::GetName()
{
    return(name);
}

std::string NZXTHue1Controller::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

unsigned int NZXTHue1Controller::GetAccessoryType()
{
    return(accessory_type);
}

void NZXTHue1Controller::SetEffect
    (
    unsigned char   mode,
    unsigned char   speed,
    bool            direction,
    RGBColor *      colors,
    unsigned int    num_colors
    )
{
    unsigned char color_data[120];

    /*-----------------------------------------------------*\
    | If mode requires no colors, send packet               |
    \*-----------------------------------------------------*/
    if(num_colors == 0)
    {
        /*-----------------------------------------------------*\
        | Send mode without color data                          |
        \*-----------------------------------------------------*/
        SendPacket(mode, direction, 0, speed, 0, NULL);
    }
    /*-----------------------------------------------------*\
    | If mode requires indexed colors, send color index     |
    | packets for each mode color                           |
    \*-----------------------------------------------------*/
    else if(num_colors <= 8)
    {
        for(std::size_t color_idx = 0; color_idx < num_colors; color_idx++)
        {
            /*-----------------------------------------------------*\
            | Fill in color data (40 entries per color)             |
            \*-----------------------------------------------------*/
            for (std::size_t idx = 0; idx < 40; idx++)
            {
                int pixel_idx = (int)idx * 3;
                RGBColor color = colors[color_idx];
                color_data[pixel_idx + 0x00] = RGBGetGValue(color);
                color_data[pixel_idx + 0x01] = RGBGetRValue(color);
                color_data[pixel_idx + 0x02] = RGBGetBValue(color);
            }

            /*-----------------------------------------------------*\
            | Send mode and color data                              |
            \*-----------------------------------------------------*/
            SendPacket(mode, direction, (unsigned char)color_idx, speed, 40, &color_data[0]);
        }
    }
    /*-----------------------------------------------------*\
    | If mode requires per-LED colors, fill colors array    |
    \*-----------------------------------------------------*/
    else
    {
        /*-----------------------------------------------------*\
        | Fill in color data (up to 40 colors)                  |
        \*-----------------------------------------------------*/
        for (std::size_t idx = 0; idx < num_colors; idx++)
        {
            int pixel_idx = (int)idx * 3;
            RGBColor color = colors[idx];
            color_data[pixel_idx + 0x00] = RGBGetGValue(color);
            color_data[pixel_idx + 0x01] = RGBGetRValue(color);
            color_data[pixel_idx + 0x02] = RGBGetBValue(color);
        }

        /*-----------------------------------------------------*\
        | Send mode and color data                              |
        \*-----------------------------------------------------*/
        SendPacket(mode, direction, 0, speed, num_colors, &color_data[0]);
    }
}

void NZXTHue1Controller::SetLEDs
    (
    RGBColor *      colors,
    unsigned int    num_colors
    )
{
    unsigned char color_data[120];

    /*-----------------------------------------------------*\
    | Fill in color data (up to 40 colors)                  |
    \*-----------------------------------------------------*/
    for (std::size_t idx = 0; idx < num_colors; idx++)
    {
        int pixel_idx = (int)idx * 3;
        RGBColor color = colors[idx];
        color_data[pixel_idx + 0x00] = RGBGetGValue(color);
        color_data[pixel_idx + 0x01] = RGBGetRValue(color);
        color_data[pixel_idx + 0x02] = RGBGetBValue(color);
    }

    /*-----------------------------------------------------*\
    | Send color data                                       |
    \*-----------------------------------------------------*/
    SendPacket(HUE_1_MODE_FIXED, false, 0, 0, num_colors, &color_data[0]);
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void NZXTHue1Controller::Initialize()
{
    unsigned char   usb_buf[65];
    unsigned int    ret_val = 0;

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Send Initialize command                               |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x01;
    usb_buf[0x01]   = 0x5C;

    hid_write(dev, usb_buf, 65);

    /*-----------------------------------------------------*\
    | Send Start Reporting command                          |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x01;
    usb_buf[0x01]   = 0x5D;

    hid_write(dev, usb_buf, 65);

    /*-----------------------------------------------------*\
    | Receive packets until a valid read has occurred       |
    \*-----------------------------------------------------*/
    do
    {
        ret_val = hid_read(dev, usb_buf, 21);
    } while( (ret_val != 21) );

    /*-----------------------------------------------------*\
    | Determine firmware version                            |
    |                                                       |
    |   0x0B is the major value                             |
    |   0x0E is the minor value                             |
    \*-----------------------------------------------------*/
    snprintf(firmware_version, 16, "%u.%u", usb_buf[0x0B], usb_buf[0x0E]);

    /*-----------------------------------------------------*\
    | Determine device count and type                       |
    |                                                       |
    |   0x11 defines the number of devices (strip, fan)     |
    |   0x10 defines the type (strip or fan)                |
    |       0: Hue+ RGB Strip (10 LEDs)                     |
    |       1: Aer RGB Fan (8 LEDs)                         |
    |   All connected devices must be of the same type      |
    \*-----------------------------------------------------*/
    unsigned char   dev_count   = usb_buf[0x11];
    accessory_type              = (usb_buf[0x10] >> 3);

    if(accessory_type == HUE_1_ACCESSORY_STRIP)
    {

        num_leds = dev_count * 10;
    }
    else
    {
        num_leds = dev_count * 8;
    }
}

void NZXTHue1Controller::SendPacket
    (
    unsigned char   mode,
    bool            direction,
    unsigned char   color_idx,
    unsigned char   speed,
    unsigned char   color_count,
    unsigned char*  color_data
    )
{
    unsigned char   usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up RGB packet 1                                   |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x02;
    usb_buf[0x01]   = 0x4B;

    /*-----------------------------------------------------*\
    | Set mode in RGB packet                                |
    \*-----------------------------------------------------*/
    usb_buf[0x02]   = mode;

    /*-----------------------------------------------------*\
    | Set options bitfield in RGB packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x03]   = direction ? ( 1 << 4 ) : 0;

    /*-----------------------------------------------------*\
    | Set color index and speed in RGB packet               |
    \*-----------------------------------------------------*/
    usb_buf[0x04]   = ( color_idx << 5 ) | speed;

    /*-----------------------------------------------------*\
    | Copy in color data bytes                              |
    \*-----------------------------------------------------*/
    unsigned int colors_in_packet = 20;

    if(color_count < 20)
    {
        colors_in_packet = color_count;
    }

    memcpy(&usb_buf[0x05], color_data, colors_in_packet * 3);

    /*-----------------------------------------------------*\
    | Write RGB packet                                      |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up RGB packet 2                                   |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x03;

    /*-----------------------------------------------------*\
    | Copy in color data bytes                              |
    \*-----------------------------------------------------*/
    colors_in_packet = color_count - colors_in_packet;

    memcpy(&usb_buf[0x01], color_data, colors_in_packet * 3);

    hid_write(dev, usb_buf, 65);
}
