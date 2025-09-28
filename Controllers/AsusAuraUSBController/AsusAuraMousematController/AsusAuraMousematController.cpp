/*---------------------------------------------------------*\
| AsusAuraMousematController.cpp                            |
|                                                           |
|   Driver for ASUS Aura mousemat                           |
|                                                           |
|   Adam Honse (CalcProgrammer1)                10 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include "AsusAuraMousematController.h"
#include "StringUtils.h"

AuraMousematController::AuraMousematController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

AuraMousematController::~AuraMousematController()
{
    hid_close(dev);
}

std::string AuraMousematController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string AuraMousematController::GetName()
{
    return(name);
}

std::string AuraMousematController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string AuraMousematController::GetVersion()
{
    unsigned char usb_buf[65];
    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[0x00]   = 0xEE;
    usb_buf[0x01]   = 0x12;
    usb_buf[0x02]   = 0x00;

    hid_write(dev, usb_buf, 65);

    unsigned char usb_buf_out[65];
    hid_read(dev, usb_buf_out, 65);

    char version[9];
    snprintf(version, 9, "%X.%02X.%02X", usb_buf_out[6], usb_buf_out[7], usb_buf_out[8]);
    return std::string(version);
}

void AuraMousematController::UpdateLeds
    (
    std::vector<RGBColor>    colors
    )
{
    unsigned char usb_buf[65];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0xEE;
    usb_buf[0x01]   = 0xC0;
    usb_buf[0x02]   = 0x81;
    usb_buf[0x03]   = 0x00;
    usb_buf[0x04]   = 0x00;

    for(unsigned int i = 0; i < 60; i += 4)
    {
        usb_buf[5 + i]   = 0x00;
        usb_buf[6 + i]   = RGBGetRValue(colors[i / 4]);
        usb_buf[7 + i]   = RGBGetGValue(colors[i / 4]);
        usb_buf[8 + i]   = RGBGetBValue(colors[i / 4]);
    }

    hid_write(dev, usb_buf, 65);
}

void AuraMousematController::UpdateDevice
    (
    unsigned char           mode,
    std::vector<RGBColor>   colors,
    unsigned char           speed,
    unsigned char           brightness,
    unsigned char           pattern
    )
{
    unsigned char usb_buf[65];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0xEE;
    usb_buf[0x01]   = 0x51;
    usb_buf[0x02]   = 0x28;
    usb_buf[0x03]   = 0x00;
    usb_buf[0x04]   = 0x00;
    usb_buf[0x05]   = mode;
    usb_buf[0x06]   = speed;
    usb_buf[0x07]   = brightness;
    usb_buf[0x08]   = pattern;
    usb_buf[0x09]   = 0x00;

    for(unsigned int i = 0; i < colors.size(); i++)
    {
        usb_buf[0x0a + i * 3]   = RGBGetRValue(colors[i]);
        usb_buf[0x0b + i * 3]   = RGBGetGValue(colors[i]);
        usb_buf[0x0c + i * 3]   = RGBGetBValue(colors[i]);
    }

    hid_write(dev, usb_buf, 65);
}

void AuraMousematController::SaveMode()
{
    unsigned char usb_save_buf[65];

    memset(usb_save_buf, 0x00, sizeof(usb_save_buf));

    usb_save_buf[0x00]   = 0xEE;
    usb_save_buf[0x01]   = 0x50;
    usb_save_buf[0x02]   = 0x03;

    hid_write(dev, usb_save_buf, 65);
}
