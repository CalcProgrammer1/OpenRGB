/*---------------------------------------------------------*\
| MadCatzCyborgController.cpp                               |
|                                                           |
|   Driver for MadCatz Cyborg Gaming Light                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "MadCatzCyborgController.h"
#include "StringUtils.h"
#include <cstring>

MadCatzCyborgController::MadCatzCyborgController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

MadCatzCyborgController::~MadCatzCyborgController()
{
    if(dev != nullptr)
    {
        hid_close(dev);
        dev = nullptr;
    }
}

std::string MadCatzCyborgController::GetDeviceLocation()
{
    return(location);
}

std::string MadCatzCyborgController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void MadCatzCyborgController::Initialize()
{
    if(dev == nullptr)
    {
        return;
    }

    // Enable the device
    unsigned char enable_buf[2] = { CMD_ENABLE, 0x00 };
    hid_send_feature_report(dev, enable_buf, 2);
}

void MadCatzCyborgController::SetLEDColor(unsigned char red, unsigned char green, unsigned char blue)
{
    if(dev == nullptr)
    {
        return;
    }
    
    // Format: [CMD_COLOR][0x00][R][G][B][0x00][0x00][0x00][0x00]
    unsigned char usb_buf[9] = 
    { 
        CMD_COLOR, 
        0x00, 
        red, 
        green, 
        blue, 
        0x00, 
        0x00, 
        0x00, 
        0x00 
    };
    
    hid_send_feature_report(dev, usb_buf, 9);
}

void MadCatzCyborgController::SetIntensity(unsigned char intensity)
{
    if(dev == nullptr)
    {
        return;
    }
    
    // Clamp intensity to 0-100
    if(intensity > 100)
    {
        intensity = 100;
    }
    
    // Format: [CMD_INTENSITY][0x00][intensity_value]
    unsigned char usb_buf[3] = { CMD_INTENSITY, 0x00, intensity };
    hid_send_feature_report(dev, usb_buf, 3);
}
