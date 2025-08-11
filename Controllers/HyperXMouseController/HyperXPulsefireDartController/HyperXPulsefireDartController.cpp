/*---------------------------------------------------------*\
| HyperXPulsefireDartController.cpp                         |
|                                                           |
|   Driver for HyperX Pulsefire Dart                        |
|                                                           |
|   Santeri Pikarinen (santeri3700)             26 Dec 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "HyperXPulsefireDartController.h"
#include "StringUtils.h"

HyperXPulsefireDartController::HyperXPulsefireDartController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

HyperXPulsefireDartController::~HyperXPulsefireDartController()
{
    hid_close(dev);
}

std::string HyperXPulsefireDartController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string HyperXPulsefireDartController::GetNameString()
{
    return(name);
}

std::string HyperXPulsefireDartController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void HyperXPulsefireDartController::SendDirect
    (
    RGBColor        color,
    int             led,
    int             mode,
    int             brightness,
    int             speed
    )
{
    unsigned char buf[HYPERX_PULSEFIRE_DART_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Direct Mode packet                             |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x00;
    buf[0x01]   = HYPERX_PULSEFIRE_DART_PACKET_ID_DIRECT;
    buf[0x02]   = led;
    buf[0x03]   = mode;
    buf[0x04]   = 0x08; // 8 bytes after buffer index 0x04

    buf[0x05]   = RGBGetRValue(color);
    buf[0x06]   = RGBGetGValue(color);
    buf[0x07]   = RGBGetBValue(color);

    buf[0x08]   = RGBGetRValue(color);
    buf[0x09]   = RGBGetGValue(color);
    buf[0x0a]   = RGBGetBValue(color);

    buf[0x0b]   = brightness;
    buf[0x0c]   = speed;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, (unsigned char *)buf, sizeof(buf));
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
}

void HyperXPulsefireDartController::Save()
{
    /*-----------------------------------------------------*\
    | Save current settings to the on-board memory          |
    \*-----------------------------------------------------*/
    unsigned char buf[HYPERX_PULSEFIRE_DART_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Save packet                                    |
    \*-----------------------------------------------------*/
    buf[0x00] = 0x00;
    buf[0x01] = 0xde;
    buf[0x02] = 0xff;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, (unsigned char *)buf, sizeof(buf));
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
}
