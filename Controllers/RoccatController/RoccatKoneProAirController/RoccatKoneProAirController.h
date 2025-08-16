/*---------------------------------------------------------*\
| RoccatKoneProAirController.h                              |
|                                                           |
|   Driver for Roccat Kone Pro Air                          |
|                                                           |
|   Plunti                                      10 Jun 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <hidapi.h>
#include "RGBController.h"

#define ROCCAT_KONE_PRO_AIR_RGB_PACKET_LENGTH            16
#define ROCCAT_KONE_PRO_AIR_SETTINGS_WRITE_PACKET_LENGTH 25
#define ROCCAT_KONE_PRO_AIR_SETTINGS_READ_PACKET_LENGTH  42
#define ROCCAT_KONE_PRO_AIR_LED_COUNT                    2

enum
{
    ROCCAT_KONE_PRO_AIR_DIRECT_MODE_VALUE       = 0x09,
    ROCCAT_KONE_PRO_AIR_OFF_MODE_VALUE          = 0x00,
    ROCCAT_KONE_PRO_AIR_STATIC_MODE_VALUE       = 0x01,
    ROCCAT_KONE_PRO_AIR_RAINBOW_WAVE_MODE_VALUE = 0x06,
    ROCCAT_KONE_PRO_AIR_HEARTBEAT_MODE_VALUE    = 0x05,
    ROCCAT_KONE_PRO_AIR_BREATHING_MODE_VALUE    = 0x02,
    ROCCAT_KONE_PRO_AIR_FLASHING_MODE_VALUE     = 0x08,
    ROCCAT_KONE_PRO_AIR_BATTERY_MODE_VALUE      = 0x0A
};

enum
{
    ROCCAT_KONE_PRO_AIR_SPEED_MIN               = 0x01,
    ROCCAT_KONE_PRO_AIR_SPEED_MAX               = 0x0B,
    ROCCAT_KONE_PRO_AIR_SPEED_MID               = (ROCCAT_KONE_PRO_AIR_SPEED_MAX - ROCCAT_KONE_PRO_AIR_SPEED_MIN) / 2,
    ROCCAT_KONE_PRO_AIR_BRIGHTNESS_MIN          = 0x00,
    ROCCAT_KONE_PRO_AIR_BRIGHTNESS_MAX          = 0x64
};

class RoccatKoneProAirController
{
public:
    RoccatKoneProAirController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~RoccatKoneProAirController();

    std::string     GetDeviceLocation();
    std::string     GetNameString();
    std::string     GetSerialString();

    void            SendDirect(std::vector<RGBColor> colors);
    void            SetMode(std::vector<RGBColor> colors, unsigned char mode_value, unsigned char speed, unsigned char brightness, unsigned int mode_flags);

private:
    hid_device*     dev;
    std::string     location;
    std::string     name;

    void            SendRGB(bool direct, std::vector<RGBColor> colors, unsigned char mode_value, unsigned char speed, unsigned char brightness);
};
