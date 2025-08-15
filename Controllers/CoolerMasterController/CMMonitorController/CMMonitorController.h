/*---------------------------------------------------------*\
| CMMonitorController.h                                     |
|                                                           |
|   Driver for Cooler Master monitor                        |
|                                                           |
|   Morgan Guimard (morg)                       18 Sep 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define CM_MONITOR_PACKET_LENGTH     65
#define CM_MONITOR_COLOR_DATA_LENGTH 436

enum
{
    CM_MONITOR_DIRECT_MODE         = 0xFF,
    CM_MONITOR_CUSTOM_MODE         = 0xFE,
    CM_MONITOR_SPECTRUM_MODE       = 0x00,
    CM_MONITOR_RELOAD_MODE         = 0x01,
    CM_MONITOR_RECOIL_MODE         = 0x02,
    CM_MONITOR_BREATHING_MODE      = 0x03,
    CM_MONITOR_REFILL_MODE         = 0x04,
    CM_MONITOR_OFF_MODE            = 0x06
};

enum
{
    CM_MONITOR_BRIGHTNESS_MAX      = 0xFF,
    CM_MONITOR_BRIGHTNESS_MIN      = 0x00,
    CM_MONITOR_SPEED_MAX           = 0x04,
    CM_MONITOR_SPEED_MIN           = 0x00,
};

class CMMonitorController
{
public:
    CMMonitorController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~CMMonitorController();

    std::string     GetDeviceLocation();
    std::string     GetNameString();
    std::string     GetSerialString();

    void            SendDirect(const std::vector<RGBColor>& colors);
    void            SetMode(uint8_t mode_value, const RGBColor& color, uint8_t speed, uint8_t brightness);
    void            SetCustomMode(const std::vector<RGBColor>& colors, uint8_t brightnesss);

private:
    std::string     location;
    std::string     name;
    hid_device*     dev;
    bool            software_mode_enabled = false;
    void            SetSoftwareModeEnabled(bool value);
};
