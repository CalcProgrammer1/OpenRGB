/*---------------------------------------------------------*\
| AsusStrixClawController.cpp                               |
|                                                           |
|   Driver for ASUS Strix Claw mouse                        |
|                                                           |
|   Mola19                                      06 Aug 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include <string>
#include "AsusStrixClawController.h"
#include "StringUtils.h"

StrixClawController::StrixClawController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

StrixClawController::~StrixClawController()
{
    hid_close(dev);
}

std::string StrixClawController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string StrixClawController::GetDeviceName()
{
    return(name);
}

std::string StrixClawController::GetSerialString()
{
    wchar_t serial_string[HID_MAX_STR];
    int ret = hid_get_serial_number_string(dev, serial_string, HID_MAX_STR);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string StrixClawController::GetVersion()
{

    // asking the device to prepare version information
    unsigned char usb_buf_out[9];

    memset(usb_buf_out, 0x00, 9);

    usb_buf_out[0x00] = 0x07;
    usb_buf_out[0x01] = 0x80;

    hid_send_feature_report(dev, usb_buf_out, 9);

    // retrieving the version information
    unsigned char usb_buf_in[9];

    memset(usb_buf_in, 0x00, 9);

    usb_buf_in[0x00] = 0x07;

    hid_get_feature_report(dev, usb_buf_in, 9);

    return (std::to_string(usb_buf_in[1]) + std::to_string(usb_buf_in[2]));
}

void StrixClawController::SetScrollWheelLED(bool OnOff)
{
    unsigned char usb_buf[9];

    memset(usb_buf, 0x00, 9);

    usb_buf[0x00] = 0x07;
    usb_buf[0x01] = 0x07;
    usb_buf[0x02] = 0x01;
    usb_buf[0x03] = OnOff;
    hid_send_feature_report(dev, usb_buf, 9);
}

void StrixClawController::SetLogoLED(uint8_t brightness)
{
    unsigned char usb_buf[9];

    memset(usb_buf, 0x00, 9);

    usb_buf[0x00] = 0x07;
    usb_buf[0x01] = 0x0a;
    usb_buf[0x02] = 0x01;
    usb_buf[0x03] = 0x00;
    usb_buf[0x04] = brightness;
    hid_send_feature_report(dev, usb_buf, 9);
}
