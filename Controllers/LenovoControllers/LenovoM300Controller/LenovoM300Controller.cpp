/*---------------------------------------------------------*\
| LenovoM300Controller.cpp                                  |
|                                                           |
|   Driver for Lenovo Legion M300 mouse                     |
|                                                           |
|   Wayne Riordan                               09 Jan 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "LenovoM300Controller.h"

using namespace std;

LenovoM300Controller::LenovoM300Controller(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    device      = dev_handle;
    location    = info.path;
    name        = dev_name;
}

LenovoM300Controller::~LenovoM300Controller()
{
    hid_close(device);
}

std::string LenovoM300Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string LenovoM300Controller::GetDeviceName()
{
    return(name);
}

void LenovoM300Controller::SetMode(std::vector<RGBColor> colors, unsigned char mode_value, unsigned int brigthness)
{
    unsigned char usb_buf[M300_DATA_SIZE];
    memset(usb_buf, 0x00, M300_DATA_SIZE);

    usb_buf[0x01] = 0x25;
    usb_buf[0x03] = 0x01;
    usb_buf[0x04] = 0x01;
    usb_buf[0x05] = 0x0C;

    switch(mode_value)
    {
    case M300_MODE_RAINBOW:
        usb_buf[0x06] = 0x01;
        usb_buf[0x07] = 0x64;
        usb_buf[0x09] = 0x0A;
        usb_buf[0x40] = CalculateFinalByte(usb_buf, 0x0A);
        break;
    case M300_MODE_BREATHING:
        usb_buf[0x06] = 0x02;
        usb_buf[0x07] = 0x64;
        usb_buf[0x09] = 0x02;
        usb_buf[0x0A] = 0x03;
        usb_buf[0x0C] = RGBGetRValue(colors[0]);
        usb_buf[0x0D] = RGBGetGValue(colors[0]);
        usb_buf[0x0E] = RGBGetBValue(colors[0]);
        usb_buf[0x0F] = RGBGetRValue(colors[1]);
        usb_buf[0x10] = RGBGetGValue(colors[1]);
        usb_buf[0x11] = RGBGetBValue(colors[1]);
        usb_buf[0x40] = CalculateFinalByte(usb_buf, 0x12);
        break;
    case M300_MODE_STATIC:
        usb_buf[0x06] = 0x03;
        usb_buf[0x07] = brigthness;
        usb_buf[0x09] = RGBGetRValue(colors[0]);
        usb_buf[0x0A] = RGBGetGValue(colors[0]);
        usb_buf[0x0B] = RGBGetBValue(colors[0]);
        usb_buf[0x40] = CalculateFinalByte(usb_buf, 0x0C);
        break;
    case M300_MODE_OFF:
    default:
        usb_buf[0x06] = 0x03;
        usb_buf[0x40] = 0x36;
    }
    hid_write(device, usb_buf, M300_DATA_SIZE);
}

unsigned char LenovoM300Controller::CalculateFinalByte(unsigned char* ptr, int count)
{
    unsigned char final_byte = 0;
    for(int i = 0; i < count; i++)
    {
        final_byte += ptr[i];
    }
    return final_byte;
}
