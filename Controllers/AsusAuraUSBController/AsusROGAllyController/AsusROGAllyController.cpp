/*---------------------------------------------------------*\
| AsusROGAllyController.cpp                                 |
|                                                           |
|   Driver for ASUS ROG Ally                                |
|                                                           |
|   Adam Honse (CalcProgrammer1)                12 Jul 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "AsusROGAllyController.h"
#include "StringUtils.h"

ROGAllyController::ROGAllyController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;

    SendInitialization();
}

ROGAllyController::~ROGAllyController()
{
    hid_close(dev);
}

std::string ROGAllyController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string ROGAllyController::GetName()
{
    return(name);
}

std::string ROGAllyController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string ROGAllyController::GetVersion()
{
    return("");
}

void ROGAllyController::SendInitialization()
{
    unsigned char usb_buf[64];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x5D;
    usb_buf[0x01]   = 0xB9;

    hid_send_feature_report(dev, usb_buf, sizeof(usb_buf));

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x5D;
    usb_buf[0x01]   = 0x41;
    usb_buf[0x02]   = 0x53;
    usb_buf[0x03]   = 0x55;
    usb_buf[0x04]   = 0x53;
    usb_buf[0x05]   = 0x20;
    usb_buf[0x06]   = 0x54;
    usb_buf[0x07]   = 0x65;
    usb_buf[0x08]   = 0x63;
    usb_buf[0x09]   = 0x68;
    usb_buf[0x0A]   = 0x2E;
    usb_buf[0x0B]   = 0x49;
    usb_buf[0x0C]   = 0x6E;
    usb_buf[0x0D]   = 0x63;
    usb_buf[0x0E]   = 0x2E;

    hid_send_feature_report(dev, usb_buf, sizeof(usb_buf));
}

void ROGAllyController::UpdateBrightness
    (
    unsigned char           brightness
    )
{
    unsigned char usb_buf[64];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x5A;
    usb_buf[0x01]   = 0xBA;
    usb_buf[0x02]   = 0xC5;
    usb_buf[0x03]   = 0xC4;
    usb_buf[0x04]   = brightness;

    hid_send_feature_report(dev, usb_buf, sizeof(usb_buf));
}

void ROGAllyController::UpdateLeds
    (
    std::vector<RGBColor>    colors
    )
{
    unsigned char usb_buf[64];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x5A;
    usb_buf[0x01]   = 0xD1;
    usb_buf[0x02]   = 0x08;
    usb_buf[0x03]   = 0x0C;

    for(unsigned int color_idx = 0; color_idx < colors.size(); color_idx++)
    {
        usb_buf[color_idx * 3 + 4]  = RGBGetRValue(colors[color_idx]);
        usb_buf[color_idx * 3 + 5]  = RGBGetGValue(colors[color_idx]);
        usb_buf[color_idx * 3 + 6]  = RGBGetBValue(colors[color_idx]);
    }

    hid_send_feature_report(dev, usb_buf, sizeof(usb_buf));
}

void ROGAllyController::UpdateDevice
    (
    unsigned char           mode,
    std::vector<RGBColor>   colors,
    unsigned char           speed,
    unsigned char           direction
    )
{
    unsigned char usb_buf[64];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x5A;
    usb_buf[0x01]   = 0xB3;
    usb_buf[0x02]   = 0x00;
    usb_buf[0x03]   = mode;
    if(colors.size() > 0)
    {
        usb_buf[0x04]   = RGBGetRValue(colors[0]);
        usb_buf[0x05]   = RGBGetGValue(colors[0]);
        usb_buf[0x06]   = RGBGetBValue(colors[0]);
    }
    usb_buf[0x07]   = speed;
    usb_buf[0x08]   = direction;
    if(colors.size() > 1)
    {
        usb_buf[0x0A]   = RGBGetRValue(colors[1]);
        usb_buf[0x0B]   = RGBGetGValue(colors[1]);
        usb_buf[0x0C]   = RGBGetBValue(colors[1]);
    }

    hid_send_feature_report(dev, usb_buf, sizeof(usb_buf));

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x5A;
    usb_buf[0x01]   = 0xB5;

    hid_send_feature_report(dev, usb_buf, sizeof(usb_buf));
}

void ROGAllyController::SaveMode()
{
    unsigned char usb_buf[64];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x5A;
    usb_buf[0x01]   = 0xB4;

    hid_send_feature_report(dev, usb_buf, sizeof(usb_buf));
}
