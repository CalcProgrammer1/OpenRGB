/*---------------------------------------------------------*\
| LGMonitorController.h                                     |
|                                                           |
|   Driver for LG monitor                                   |
|                                                           |
|   Morgan Guimard (morg)                       11 Oct 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define LG_MONITOR_LEDS                 48
#define LG_MONITOR_PACKET_SIZE          65
#define LG_MONITOR_READ_CMD             0x52 // R (Read)
#define LG_MONITOR_START_CMD_1          0x53 // S (Send)
#define LG_MONITOR_START_CMD_2          0x43 // C (Command)
#define LG_MONITOR_END_CMD_1            0x45 // E (End)
#define LG_MONITOR_END_CMD_2            0x44 // D (Data)
#define LG_MONITOR_READ_POWER_STATE     0xCE
#define LG_MONITOR_SET_POWER_STATE      0xCF
#define LG_MONITOR_SET_COLOR            0xCD
#define LG_MONITOR_SET_MODE             0xCA
#define LG_MONITOR_DIRECT_CTL           0xC1
#define LG_MONITOR_POWER_ON             0x01
#define LG_MONITOR_POWER_OFF            0x02
#define LG_MONITOR_MODE_CTL             0x03
#define LG_MONITOR_BRIGHTNESS_CTL       0x01

enum
{
    LG_MONITOR_DIRECT_MODE_VALUE            = 0x08,
    LG_MONITOR_STATIC_SLOT_1_MODE_VALUE     = 0x01,
    LG_MONITOR_SPECTRUM_CYCLE_MODE_VALUE    = 0x05,
    LG_MONITOR_RAINBOW_MODE_VALUE           = 0x06,
    LG_MONITOR_OFF_MODE_VALUE               = 0x00
};

class LGMonitorController
{
public:
    LGMonitorController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~LGMonitorController();

    std::string GetDeviceLocation();
    std::string GetFirmwareVersion();
    std::string GetNameString();
    std::string GetSerialString();

    void SetDirect(const std::vector<RGBColor> colors);
    void SetMode(uint8_t mode_value, uint8_t brightness, const std::vector<RGBColor> colors);

private:
    hid_device* dev;
    std::string description;
    std::string location;
    std::string name;
    std::string version;
    bool        on = false;
    bool        direct_mode_enabled = false;

    static uint8_t crc(const uint8_t data[], uint8_t start, uint8_t end);
    void SetBrightness(uint8_t value);
    void TurnOn(bool value);
    void EnableDirectMode();
    void EnableMode(uint8_t mode_value);
    void SetSlotColor(uint8_t slot, const RGBColor color);
};
