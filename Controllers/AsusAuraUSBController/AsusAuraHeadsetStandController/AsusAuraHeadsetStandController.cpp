/*---------------------------------------------------------*\
| AsusAuraHeadsetStandController.cpp                        |
|                                                           |
|   Driver for ASUS Aura headset stand                      |
|                                                           |
|   Mola19                                      06 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "AsusAuraHeadsetStandController.h"
#include "StringUtils.h"

AuraHeadsetStandController::AuraHeadsetStandController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

AuraHeadsetStandController::~AuraHeadsetStandController()
{
    hid_close(dev);
}

std::string AuraHeadsetStandController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string AuraHeadsetStandController::GetName()
{
    return(name);
}

std::string AuraHeadsetStandController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string AuraHeadsetStandController::GetVersion()
{
    unsigned char usb_buf[65];
    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x12;
    usb_buf[0x02]   = 0x00;

    hid_write(dev, usb_buf, 65);

    unsigned char usb_buf_out[65];
    hid_read(dev, usb_buf_out, 65);

    char version[5];
    snprintf(version, 5, "%04X", (usb_buf_out[6] << 8) | usb_buf_out[7]);
    return std::string(version);
}

void AuraHeadsetStandController::UpdateLeds
    (
    std::vector<RGBColor>    colors
    )
{
    unsigned char usb_buf_0[365];

    memset(usb_buf_0, 0x00, sizeof(usb_buf_0));

    usb_buf_0[0x00]   = 0x00;
    usb_buf_0[0x01]   = 0xC0;
    usb_buf_0[0x02]   = 0x81;
    usb_buf_0[0x03]   = 0x00;
    usb_buf_0[0x04]   = 0x00;

    for(unsigned int i = 0; i < 60; i += 4)
    {
        usb_buf_0[5 + i]   = 0x00;
        usb_buf_0[6 + i]   = RGBGetRValue(colors[i / 4]);
        usb_buf_0[7 + i]   = RGBGetGValue(colors[i / 4]);
        usb_buf_0[8 + i]   = RGBGetBValue(colors[i / 4]);
    }

    hid_write(dev, usb_buf_0, 65);

    unsigned char usb_buf_1[65];

    memset(usb_buf_1, 0x00, sizeof(usb_buf_1));

    usb_buf_1[0x00]   = 0x00;
    usb_buf_1[0x01]   = 0xC0;
    usb_buf_1[0x02]   = 0x81;
    usb_buf_1[0x03]   = 0x01;
    usb_buf_1[0x04]   = 0x00;

    for(unsigned int i = 0; i < 12; i += 4)
    {
        usb_buf_1[5 + i]   = 0x00;
        usb_buf_1[6 + i]   = RGBGetRValue(colors[(i / 4) + 15]);
        usb_buf_1[7 + i]   = RGBGetGValue(colors[(i / 4) + 15]);
        usb_buf_1[8 + i]   = RGBGetBValue(colors[(i / 4) + 15]);
    }

    hid_write(dev, usb_buf_1, 65);
}

void AuraHeadsetStandController::UpdateDevice
    (
    unsigned char   mode,
    unsigned char   red,
    unsigned char   grn,
    unsigned char   blu,
    unsigned char   speed,
    unsigned char   brightness
    )
{
    unsigned char usb_buf[65];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x51;
    usb_buf[0x02]   = 0x28;
    usb_buf[0x03]   = 0x00;
    usb_buf[0x04]   = 0x00;
    usb_buf[0x05]   = mode;
    usb_buf[0x06]   = speed;
    usb_buf[0x07]   = brightness;
    usb_buf[0x08]   = 0x00;
    usb_buf[0x09]   = 0x00;
    usb_buf[0x0a]   = red;
    usb_buf[0x0b]   = grn;
    usb_buf[0x0c]   = blu;
    hid_write(dev, usb_buf, 65);

}

void AuraHeadsetStandController::SaveMode()
{
    unsigned char usb_save_buf[65];

    memset(usb_save_buf, 0x00, sizeof(usb_save_buf));

    usb_save_buf[0x00]   = 0x00;
    usb_save_buf[0x01]   = 0x50;
    usb_save_buf[0x02]   = 0x55;

    hid_write(dev, usb_save_buf, 65);
}
