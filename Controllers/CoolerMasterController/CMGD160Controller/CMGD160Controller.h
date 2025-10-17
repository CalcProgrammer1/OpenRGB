/*---------------------------------------------------------*\
| CMGD160Controller.h                                       |
|                                                           |
|   Driver for Cooler Master GD160 ARGB Gaming Desk         |
|                                                           |
|   Logan Phillips (Eclipse)                    16 Oct 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   Adapted from CMMonitor controller code                  |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define CM_GD160_PACKET_LENGTH          65
#define CM_GD160_COLOR_DATA_LENGTH      872
#define CM_GD160_SIDE_DATA_LENGTH       436  // 96 LEDs * 3 bytes + header = 436 bytes per side
#define CM_GD160_LEDS_PER_SIDE          96
#define CM_GD160_FIRST_PACKET_DATA_SIZE 58   // CM_GD160_PACKET_LENGTH - 7 (header bytes)
#define CM_GD160_PACKET_DATA_SIZE       63   // CM_GD160_PACKET_LENGTH - 2 (header bytes)

enum
{
    CM_GD160_DIRECT_MODE         = 0xFF,
    CM_GD160_CUSTOM_MODE         = 0xFE,
    CM_GD160_SPECTRUM_MODE       = 0x00,
    CM_GD160_RELOAD_MODE         = 0x01,
    CM_GD160_RECOIL_MODE         = 0x02,
    CM_GD160_BREATHING_MODE      = 0x03,
    CM_GD160_REFILL_MODE         = 0x04,
    CM_GD160_OFF_MODE            = 0x06
};

enum
{
    CM_GD160_BRIGHTNESS_MAX      = 0xFF,
    CM_GD160_BRIGHTNESS_MIN      = 0x00,
    CM_GD160_SPEED_MAX           = 0x04,
    CM_GD160_SPEED_MIN           = 0x00,
};

class CMGD160Controller
{
public:
    CMGD160Controller(hid_device* dev_handle, const hid_device_info& info, const std::string& name);
    ~CMGD160Controller();

    std::string     GetDeviceName();
    std::string     GetSerialString();
    std::string     GetDeviceLocation();

    void            SetMode(uint8_t mode_value, uint8_t speed, uint8_t brightness, const RGBColor& color);
    void            SendColorData(const std::vector<RGBColor>& colors, uint8_t command, uint8_t mode_byte, uint8_t brightness, bool enable_software_mode);

private:
    std::string     device_name;
    std::string     serial_number;
    std::string     location;
    hid_device*     dev;
    bool            is_software_mode_enabled = false;
    void            SetControlMode(bool value);
    void            ResetDevice();
};
