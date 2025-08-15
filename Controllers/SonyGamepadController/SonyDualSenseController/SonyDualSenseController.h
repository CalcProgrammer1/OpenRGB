/*---------------------------------------------------------*\
| SonyDualSenseController.h                                 |
|                                                           |
|   Driver for Sony DualSense                               |
|                                                           |
|   Flora Aubry                                 01 Jul 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <hidapi.h>
#include "RGBController.h"

#define  SONY_DUALSENSE_LIGHTBAR_LED_COUNT 1
#define  SONY_DUALSENSE_PLAYER_LED_COUNT   5
#define  SONY_DUALSENSE_BT_PACKET_SIZE     78
#define  SONY_DUALSENSE_USB_PACKET_SIZE    48

enum
{
    SONY_DUALSENSE_DIRECT_MODE_VALUE      = 0x01,
    SONY_DUALSENSE_MIC_OFF_MODE_VALUE     = 0x00,
    SONY_DUALSENSE_MIC_PULSE_MODE_VALUE   = 0x02,
    SONY_DUALSENSE_BRIGHTNESS_MIN         = 0x00,
    SONY_DUALSENSE_BRIGHTNESS_MAX         = 0x02,
    SONY_DUALSENSE_DEFAULT_BRIGHTNESS     = 0x01
};

class SonyDualSenseController
{
public:
    SonyDualSenseController(hid_device * device_handle, const char * device_path, bool is_bluetooth, std::string dev_name);
    ~SonyDualSenseController();

    std::string     GetLocation();
    std::string     GetName();
    std::string     GetSerialString();

    void            SetColors(std::vector<RGBColor> colors, unsigned char brightness, unsigned char mode_value);
    bool            IsBluetooth();

private:
    hid_device*     dev;
    std::string     location;
    std::string     name;
    bool            is_bluetooth;
};
