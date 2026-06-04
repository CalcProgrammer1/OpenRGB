/*---------------------------------------------------------*\
| RGBController_MSIMonitor.cpp                              |
|                                                           |
|   RGBController for MSI monitor (gaming controller)       |
|                                                           |
|   Andy Herbert                              2026 June 1   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define MSI_MONITOR_LEDS 9
#define MSI_MONITOR_PACKET_SIZE 78

enum
{
    MSI_MONITOR_OFF_MODE_VALUE       = 0x00,
    MSI_MONITOR_STATIC_MODE_VALUE    = 0x01,
    MSI_MONITOR_BREATHING_MODE_VALUE = 0x02,
    MSI_MONITOR_FLASHING_MODE_VALUE  = 0x03,
    MSI_MONITOR_LIGHTNING_MODE_VALUE = 0x05,
    MSI_MONITOR_MARQUEE_MODE_VALUE   = 0x06,
    MSI_MONITOR_METEOR_MODE_VALUE    = 0x08,
    MSI_MONITOR_RAINBOW_MODE_VALUE   = 0x1A,
    MSI_MONITOR_RANDOM_MODE_VALUE    = 0x1F
};

class MSIMonitorController
{
public:
    MSIMonitorController(hid_device *dev_handle, const hid_device_info &info, std::string dev_name);
    ~MSIMonitorController();

    std::string GetDeviceLocation();
    std::string GetFirmwareVersion();
    std::string GetNameString();
    std::string GetSerialString();

    void Set(uint8_t mode_value, const std::vector<RGBColor> colors, uint8_t last_bit);

private:
    hid_device *dev;
    std::string description;
    std::string location;
    std::string name;
    std::string version;
};
