/*---------------------------------------------------------*\
| CryorigH7QuadLumiController.cpp                           |
|                                                           |
|   Driver for Cryorig H7 Quad Lumi                         |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      15 Apr 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include "CryorigH7QuadLumiController.h"
#include "LogManager.h"
#include "StringUtils.h"

CryorigH7QuadLumiController::CryorigH7QuadLumiController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;

    SendFirmwareRequest();
}

CryorigH7QuadLumiController::~CryorigH7QuadLumiController()
{
    hid_close(dev);
}

std::string CryorigH7QuadLumiController::GetLocation()
{
    return("HID: " + location);
}

std::string CryorigH7QuadLumiController::GetName()
{
    return(name);
}

std::string CryorigH7QuadLumiController::GetFirmwareVersion()
{
    return(firmware_version);
}

std::string CryorigH7QuadLumiController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void CryorigH7QuadLumiController::SetChannelEffect
    (
    unsigned char   channel,
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
        SendPacket(channel, mode, direction, 0, speed, 0, NULL);
    }
    /*-----------------------------------------------------*\
    | If mode requires indexed colors, send color index     |
    | packets for each mode color                           |
    \*-----------------------------------------------------*/
    else if(num_colors <= 8)
    {
        for(unsigned int color_idx = 0; color_idx < num_colors; color_idx++)
        {
            /*-----------------------------------------------------*\
            | Fill in color data (5 entries per color)              |
            \*-----------------------------------------------------*/
            for(int idx = 0; idx < 40; idx++)
            {
                int pixel_idx = idx * 3;
                RGBColor color = colors[color_idx];
                color_data[pixel_idx + 0x00] = RGBGetGValue(color);
                color_data[pixel_idx + 0x01] = RGBGetRValue(color);
                color_data[pixel_idx + 0x02] = RGBGetBValue(color);
            }

            /*-----------------------------------------------------*\
            | Send mode and color data                              |
            \*-----------------------------------------------------*/
            SendPacket(channel, mode, direction, (unsigned char)color_idx, speed, 5, &color_data[0]);
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
        for(unsigned int idx = 0; idx < num_colors; idx++)
        {
            unsigned int pixel_idx = idx * 3;
            RGBColor color = colors[idx];
            color_data[pixel_idx + 0x00] = RGBGetGValue(color);
            color_data[pixel_idx + 0x01] = RGBGetRValue(color);
            color_data[pixel_idx + 0x02] = RGBGetBValue(color);
        }

        /*-----------------------------------------------------*\
        | Send mode and color data                              |
        \*-----------------------------------------------------*/
        SendPacket(channel, mode, direction, 0, speed, num_colors, &color_data[0]);
    }
}

void CryorigH7QuadLumiController::SetChannelLEDs
    (
    unsigned char   channel,
    RGBColor *      colors,
    unsigned int    num_colors
    )
{
    unsigned char color_data[120];

    /*-----------------------------------------------------*\
    | Fill in color data (up to 40 colors)                  |
    \*-----------------------------------------------------*/
    for(unsigned int idx = 0; idx < num_colors; idx++)
    {
        unsigned int pixel_idx = idx * 3;
        RGBColor color = colors[idx];
        color_data[pixel_idx + 0x00] = RGBGetGValue(color);
        color_data[pixel_idx + 0x01] = RGBGetRValue(color);
        color_data[pixel_idx + 0x02] = RGBGetBValue(color);
    }

    /*-----------------------------------------------------*\
    | Send color data                                       |
    \*-----------------------------------------------------*/
    SendPacket(channel, CRYORIG_H7_QUAD_LUMI_MODE_FIXED, false, 0, 0, num_colors, &color_data[0]);
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void CryorigH7QuadLumiController::SendPacket
    (
    unsigned char   channel,
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
    | Set up RGB packet                                     |
    \*-----------------------------------------------------*/
    usb_buf[0x00]       = 0x02;
    usb_buf[0x01]       = 0x4C;

    /*-----------------------------------------------------*\
    | Set channel and direction in RGB packet               |
    \*-----------------------------------------------------*/
    usb_buf[0x02]       = (channel + 1) | (direction ? (1 << 4) : 0);

    /*-----------------------------------------------------*\
    | Set mode in RGB packet                                |
    \*-----------------------------------------------------*/
    usb_buf[0x03]       = mode;

    /*-----------------------------------------------------*\
    | Set color index and speed in RGB packet               |
    \*-----------------------------------------------------*/
    usb_buf[0x04]       = ( color_idx << 5 ) | speed;

    /*-----------------------------------------------------*\
    | Copy in color data bytes                              |
    \*-----------------------------------------------------*/
    memcpy(&usb_buf[0x05 + (channel * 15)], color_data, color_count * 3);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
}

void CryorigH7QuadLumiController::SendFirmwareRequest()
{
    unsigned char   usb_buf[17];
    unsigned int    ret_val = 0;

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Firmware Request packet                        |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x02;
    usb_buf[0x01]   = 0x5C;

    hid_write(dev, usb_buf, 65);

    /*-----------------------------------------------------*\
    | Receive packets until 0x11 0x01 is received           |
    \*-----------------------------------------------------*/
    do
    {
        ret_val = hid_read(dev, usb_buf, sizeof(usb_buf));
    } while( (ret_val != 17) );

    snprintf(firmware_version, 16, "%u.%u", usb_buf[0x0D], usb_buf[0x0E]);
}
