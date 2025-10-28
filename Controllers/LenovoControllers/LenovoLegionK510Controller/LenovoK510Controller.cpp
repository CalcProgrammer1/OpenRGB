/*---------------------------------------------------------*\
| LenovoK510Controller.cpp                                  |
|                                                           |
|   Driver for Lenovo Legion K510 keyboard                  |
|                                                           |
|   Bnyro                                       27 Oct 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include <vector>
#include "LenovoK510Controller.h"

using namespace std;

LenovoK510Controller::LenovoK510Controller(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    device      = dev_handle;
    location    = info.path;
    name        = dev_name;
}

LenovoK510Controller::~LenovoK510Controller()
{
    hid_close(device);
}

std::string LenovoK510Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string LenovoK510Controller::GetDeviceName()
{
    return(name);
}

void LenovoK510Controller::SetMode(unsigned int color_mode, RGBColor color, unsigned char mode_value, unsigned int brigthness, unsigned int speed, unsigned int direction)
{
    unsigned char usb_buf[K510_DATA_SIZE];
    memset(usb_buf, 0x00, K510_DATA_SIZE);
    usb_buf[0x00] = 0x04; // ReportID

    // magic bytes to trigger an LED update
    usb_buf[0x03] = 0x06;
    usb_buf[0x04] = 0x38;

    usb_buf[0x09] = mode_value;
    usb_buf[0x0A] = static_cast<unsigned char>(brigthness);
    // speed behaves contrary to normal expectations: the lower the value, the higher the speed
    usb_buf[0x0B] = static_cast<unsigned char>(speed);

    if(direction == MODE_DIRECTION_UP || direction == MODE_DIRECTION_LEFT)
    {
        // 0x01 reverses the direction of the animation
        usb_buf[0x0C] = 0x01;
    }

    if(color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        usb_buf[0x0D] = 0x00;
        usb_buf[0x0E] = static_cast<unsigned char>(RGBGetRValue(color));
        usb_buf[0x0F] = static_cast<unsigned char>(RGBGetGValue(color));
        usb_buf[0x10] = static_cast<unsigned char>(RGBGetBValue(color));
    }
    else
    {
        // sets color to automatic
        usb_buf[0x0D] = 0x01;
    }

    hid_write(device, usb_buf, K510_DATA_SIZE);
}

mode LenovoK510Controller::GetCurrentState()
{
    unsigned char usb_buf[K510_DATA_SIZE];
    memset(usb_buf, 0x00, K510_DATA_SIZE);
    usb_buf[0x00] = 0x04; // ReportID

    // magic bytes to get a response containing the current configuration
    usb_buf[0x03] = 0x05;
    usb_buf[0x04] = 0x38;

    hid_write(device, usb_buf, K510_DATA_SIZE);

    unsigned char res_buf[K510_DATA_SIZE];
    hid_read_timeout(device, res_buf, K510_DATA_SIZE, 50);

    mode current_mode;
    current_mode.value = res_buf[0x09];
    current_mode.brightness = res_buf[0x0A];
    current_mode.speed = res_buf[0x0B];
    current_mode.direction = res_buf[0x0C];
    current_mode.color_mode = res_buf[0x0D] ? MODE_COLORS_RANDOM : MODE_COLORS_MODE_SPECIFIC;
    current_mode.colors.push_back(ToRGBColor(res_buf[0x0E], res_buf[0x0F], res_buf[0x10]));

    return(current_mode);
}
