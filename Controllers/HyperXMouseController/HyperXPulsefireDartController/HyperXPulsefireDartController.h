/*---------------------------------------------------------*\
| HyperXPulsefireDartController.h                           |
|                                                           |
|   Driver for HyperX Pulsefire Dart                        |
|                                                           |
|   Santeri Pikarinen (santeri3700)             26 Dec 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

enum
{
    HYPERX_PULSEFIRE_DART_PACKET_ID_DIRECT = 0xd2, /* Direct control packet */
    HYPERX_PULSEFIRE_DART_PACKET_SIZE      = 65,   /* Report ID padding + 64 byte payload */

    HYPERX_PULSEFIRE_DART_MODE_STATIC      = 0x00, /* Static color mode */
    HYPERX_PULSEFIRE_DART_MODE_CYCLE       = 0x12, /* Spectrum cycle mode */
    HYPERX_PULSEFIRE_DART_MODE_BREATHING   = 0x20, /* Single color breathing mode */
    HYPERX_PULSEFIRE_DART_MODE_REACTIVE    = 0x30, /* Reactive/Trigger fade mode */

    HYPERX_PULSEFIRE_DART_SPEED_MIN        = 0x64,
    HYPERX_PULSEFIRE_DART_SPEED_MAX        = 0x00,
    HYPERX_PULSEFIRE_DART_SPEED_MED        = 0x32,
    HYPERX_PULSEFIRE_DART_SPEED_NONE       = 0x00, /* For static color mode */

    HYPERX_PULSEFIRE_DART_BRIGHTNESS_MIN   = 0x00,
    HYPERX_PULSEFIRE_DART_BRIGHTNESS_MAX   = 0x64,

    HYPERX_PULSEFIRE_DART_LED_LOGO         = 0x00,
    HYPERX_PULSEFIRE_DART_LED_SCROLL       = 0x10,
    HYPERX_PULSEFIRE_DART_LED_ALL          = 0x20
};

class HyperXPulsefireDartController
{
public:
    HyperXPulsefireDartController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~HyperXPulsefireDartController();

    std::string GetDeviceLocation();
    std::string GetNameString();
    std::string GetSerialString();

    void SendDirect
            (
            RGBColor        color_data,
            int             led,
            int             mode,
            int             brightness,
            int             speed
            );

    void Save();

private:
    hid_device*             dev;
    std::string             location;
    std::string             name;
};
