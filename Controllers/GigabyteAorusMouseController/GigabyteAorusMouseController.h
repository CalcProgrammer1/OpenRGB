/*---------------------------------------------------------*\
| GigabyteAorusMouseController.h                            |
|                                                           |
|   Driver for Gigabyte Aorus mouse                         |
|                                                           |
|   Morgan Guimard (morg)                       29 Dec 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define GIGABYTE_AORUS_MOUSE_REPORT_SIZE    8
#define GIGABYTE_AORUS_MOUSE_DIRECT_CMD     0xCD
#define GIGABYTE_AORUS_MOUSE_HARDWARE_CMD   0xCC

enum
{
    GIGABYTE_AORUS_MOUSE_DIRECT_MODE_VALUE          = 0x00,
    GIGABYTE_AORUS_MOUSE_STATIC_MODE_VALUE          = 0x01,
    GIGABYTE_AORUS_MOUSE_PULSE_MODE_VALUE           = 0x02,
    GIGABYTE_AORUS_MOUSE_COLOR_CYCLE_MODE_VALUE     = 0x03,
    GIGABYTE_AORUS_MOUSE_FLASH_MODE_VALUE           = 0x04,
    GIGABYTE_AORUS_MOUSE_DOUBLE_FLASH_MODE_VALUE    = 0x05,
};

enum
{
    GIGABYTE_AORUS_MOUSE_SPEED_MIN          = 0x16,
    GIGABYTE_AORUS_MOUSE_SPEED_MAX          = 0x00,
    GIGABYTE_AORUS_MOUSE_BRIGHTNESS_MIN     = 0x00,
    GIGABYTE_AORUS_MOUSE_BRIGHTNESS_MAX     = 0x64
};

class GigabyteAorusMouseController
{
public:
    GigabyteAorusMouseController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~GigabyteAorusMouseController();

    std::string GetDeviceLocation();
    std::string GetFirmwareVersion();
    std::string GetNameString();
    std::string GetSerialString();

    void        SetMode(RGBColor color, uint8_t mode_value, uint8_t brightness, uint8_t speed);
    void        SendDirect(RGBColor color);

private:
    hid_device* dev;

    std::string location;
    std::string name;
    std::string version;
};
