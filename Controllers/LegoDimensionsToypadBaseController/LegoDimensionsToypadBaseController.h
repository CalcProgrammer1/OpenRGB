/*---------------------------------------------------------*\
| LegoDimensionsToypadBaseController.h                      |
|                                                           |
|   Driver for Lego Dimensions Toypad Base                  |
|                                                           |
|   Morgan Guimard (morg)                       02 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define LEGO_DIMENSIONS_TOYPAD_BASE_REPORT_ID       0x55
#define LEGO_DIMENSIONS_TOYPAD_BASE_PACKET_LENGTH   32

enum
{
    LEGO_DIMENSIONS_TOYPAD_BASE_ACTIVATE_VALUE      = 0xB0,
    LEGO_DIMENSIONS_TOYPAD_BASE_DIRECT_MODE_VALUE   = 0xC0,
    LEGO_DIMENSIONS_TOYPAD_BASE_FLASH_MODE_VALUE    = 0xC3,
    LEGO_DIMENSIONS_TOYPAD_BASE_FADE_MODE_VALUE     = 0xC2
};

enum
{
    LEGO_DIMENSIONS_TOYPAD_BASE_SPEED_MIN      = 0x00,
    LEGO_DIMENSIONS_TOYPAD_BASE_SPEED_MAX      = 0xFF
};

class LegoDimensionsToypadBaseController
{
public:
    LegoDimensionsToypadBaseController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~LegoDimensionsToypadBaseController();

    std::string GetDeviceLocation();
    std::string GetNameString();
    std::string GetSerialString();

    void        SetDirect(unsigned char zone, RGBColor color);
    void        SetMode(unsigned char zone, unsigned char mode_value, uint8_t speed, RGBColor color);

protected:
    hid_device* dev;

private:
    std::string location;
    std::string name;
    std::string version;

    void Activate();
};
