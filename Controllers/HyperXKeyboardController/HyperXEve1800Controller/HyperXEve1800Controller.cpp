/*---------------------------------------------------------*\
| HyperXEve1800Controller.cpp                              |
|                                                           |
|   Driver for HyperX Eve 1800 keyboard                    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include "HyperXEve1800Controller.h"
#include "StringUtils.h"

HyperXEve1800Controller::HyperXEve1800Controller(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

HyperXEve1800Controller::~HyperXEve1800Controller()
{
    hid_close(dev);
}

std::string HyperXEve1800Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string HyperXEve1800Controller::GetNameString()
{
    return(name);
}

std::string HyperXEve1800Controller::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void HyperXEve1800Controller::SetBrightness(unsigned int brightness)
{
    unsigned char buf[65];

    memset(buf, 0x00, sizeof(buf));

    buf[0x00]   = 0x40;
    buf[0x01]   = 0x01;
    buf[0x02]   = 0x00;
    buf[0x03]   = 0x00;
    buf[0x04]   = brightness & 0xFF;

    hid_write(dev, buf, 65);
}

void HyperXEve1800Controller::SetLEDsDirect(std::vector<RGBColor> colors)
{
    SendDirectInitialization();

    if(colors.empty())
    {
        RGBColor temp = 0x00000000;
        SendDirectColorPacket(&temp, 1);
    }
    else
    {
        SendDirectColorPacket(&colors[0], (unsigned int)colors.size());
    }
}

void HyperXEve1800Controller::SendDirectInitialization()
{
    unsigned char buf[65];

    memset(buf, 0x00, sizeof(buf));

    buf[0x00]   = 0x44;
    buf[0x01]   = 0x01;
    buf[0x02]   = 0x04;
    buf[0x03]   = 0x00;

    hid_write(dev, buf, 65);
}

void HyperXEve1800Controller::SendDirectColorPacket(RGBColor* color_data, unsigned int color_count)
{
    unsigned char buf[65];

    memset(buf, 0x00, sizeof(buf));

    buf[0x00]   = 0x44;
    buf[0x01]   = 0x02;
    buf[0x02]   = 0x00;
    buf[0x03]   = 0x00;

    /*-----------------------------------------------------*\
    | The Eve 1800 exposes 10 lighting zones, but this      |
    | report format can carry up to 20 RGB triplets.        |
    \*-----------------------------------------------------*/
    if(color_count > 20)
    {
        color_count = 20;
    }

    for(unsigned int color_idx = 0; color_idx < color_count; color_idx++)
    {
        buf[4 + (color_idx * 3)]     = RGBGetRValue(color_data[color_idx]);
        buf[4 + (color_idx * 3) + 1] = RGBGetGValue(color_data[color_idx]);
        buf[4 + (color_idx * 3) + 2] = RGBGetBValue(color_data[color_idx]);
    }

    hid_write(dev, buf, 65);
}
