/*---------------------------------------------------------*\
| GigabyteAorusPCCaseController.h                           |
|                                                           |
|   Driver for Gigabyte Aorus case                          |
|                                                           |
|   Denis Nazarov (nenderus)                    10 Feb 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

struct aorus_pc_case_mode_config
{
    RGBColor    color;
    uint8_t     speed;
    uint8_t     brightness;
};

enum
{
    AORUS_PC_CASE_MODE_OFF              = 0x00,
    AORUS_PC_CASE_MODE_CUSTOM           = 0x01,
    AORUS_PC_CASE_MODE_BREATHING        = 0x02,
    AORUS_PC_CASE_MODE_SPECTRUM_CYCLE   = 0x03,
    AORUS_PC_CASE_MODE_FLASHING         = 0x04,
    AORUS_PC_CASE_MODE_DOUBLE_FLASHING  = 0x05,
};

enum
{
    AORUS_PC_CASE_SPEED_SLOWEST         = 0x0A,
    AORUS_PC_CASE_SPEED_NORMAL          = 0x09,
    AORUS_PC_CASE_SPEED_FASTEST         = 0x06,
};

enum
{
    AORUS_PC_CASE_BRIGHTNESS_MIN        = 0x00,
    AORUS_PC_CASE_BRIGHTNESS_MAX        = 0x09,
};

class GigabyteAorusPCCaseController
{
public:
    GigabyteAorusPCCaseController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~GigabyteAorusPCCaseController();

    std::string     GetDeviceLocation();
    std::string     GetNameString();
    std::string     GetSerialString();

    void            SendColor(uint8_t red, uint8_t green, uint8_t blue);
    void            SendMode(uint8_t mode, uint8_t speed, uint8_t brightness);
    void            SendOk();

    void            SetMode(uint8_t mode, aorus_pc_case_mode_config zone_config);

private:
    hid_device*     dev;
    std::string     location;
    std::string     name;
};
