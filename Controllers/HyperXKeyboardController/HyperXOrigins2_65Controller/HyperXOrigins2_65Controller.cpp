/*---------------------------------------------------------*\
| HyperXOrigins2_65Controller.cpp                           |
|                                                           |
|   Driver for HyperX Origins 2 65 keyboard                 |
|                                                           |
|   Ricardo Amorim                              28 Mar 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include "HyperXOrigins2_65Controller.h"
#include "StringUtils.h"

HyperXOrigins2_65Controller::HyperXOrigins2_65Controller(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

HyperXOrigins2_65Controller::~HyperXOrigins2_65Controller()
{
    hid_close(dev);
}

std::string HyperXOrigins2_65Controller::GetDeviceLocation()
{
    return("HID " + location);
}

std::string HyperXOrigins2_65Controller::GetNameString()
{
    return(name);
}

std::string HyperXOrigins2_65Controller::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return ("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void HyperXOrigins2_65Controller::SetLEDsDirect(std::vector<RGBColor> colors)
{
    /*-----------------------------------------------------*\
    | Set up variables to track progress of color transmit  |
    | Do this after inserting blanks                        |
    \*-----------------------------------------------------*/
    int colors_to_send = (int)colors.size();
    int colors_sent    = 0;

    SendDirectInitialization();

    for(int pkt_idx = 0; pkt_idx < 4; pkt_idx++)
    {
        if(colors_to_send > 20)
        {
            SendDirectColorPacket(&colors[colors_sent], 20, pkt_idx);
            colors_sent    += 20;
            colors_to_send -= 20;
        }
        else if(colors_to_send > 0)
        {
            SendDirectColorPacket(&colors[colors_sent], colors_to_send, pkt_idx);
            colors_sent    += colors_to_send;
            colors_to_send -= colors_to_send;
        }
        else
        {
            RGBColor temp = 0x00000000;
            SendDirectColorPacket(&temp, 1, pkt_idx);
        }
    }
}

void HyperXOrigins2_65Controller::SendDirectInitialization()
{
    unsigned char buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Direct Initialization packet                   |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x44;
    buf[0x01]   = 0x01;
    buf[0x02]   = 0x04;
    buf[0x03]   = 0x00;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, buf, 65);
}

void HyperXOrigins2_65Controller::SendDirectColorPacket
    (
    RGBColor*       color_data,
    unsigned int    color_count,
    unsigned int    seq
    )
{
    unsigned char buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Direct Initialization packet                   |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x44;
    buf[0x01]   = 0x02;
    buf[0x02]   = (unsigned char)seq;
    buf[0x03]   = 0x00;

    /*-----------------------------------------------------*\
    | The maximum number of colors per packet is 20         |
    \*-----------------------------------------------------*/
    if(color_count > 20)
    {
        color_count = 20;
    }

    /*-----------------------------------------------------*\
    | Copy in color data                                    |
    \*-----------------------------------------------------*/
    for(unsigned int color_idx = 0; color_idx < color_count; color_idx++)
    {
        buf[4 + (color_idx * 3)]     = RGBGetRValue(color_data[color_idx]);
        buf[4 + (color_idx * 3) + 1] = RGBGetGValue(color_data[color_idx]);
        buf[4 + (color_idx * 3) + 2] = RGBGetBValue(color_data[color_idx]);
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, buf, 65);
}
