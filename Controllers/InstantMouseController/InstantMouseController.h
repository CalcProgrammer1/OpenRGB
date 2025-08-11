/*---------------------------------------------------------*\
| InstantMouseController.h                                  |
|                                                           |
|   Driver for Instant mouse                                |
|                                                           |
|   Morgan Guimard (morg)                       19 Jan 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define INSTANT_MOUSE_REPORT_ID                 0x07
#define INSTANT_MOUSE_REPORT_SIZE               8
#define INSTANT_MOUSE_SET_MODE                  0x13
#define INSTANT_MOUSE_SET_COLOR                 0x14

enum
{
    INSTANT_MOUSE_DIRECT_MODE                   = 0x0A,
    INSTANT_MOUSE_OFF_MODE                      = 0xFF,
    INSTANT_MOUSE_MULTICOLOR_BREATHING_MODE     = 0x01,
    INSTANT_MOUSE_FILL_DRAIN_MODE               = 0x03,
    INSTANT_MOUSE_LOOP_MODE                     = 0x04,
    INSTANT_MOUSE_SPECTRUM_CYCLE_MODE           = 0x06,
    INSTANT_MOUSE_RAINBOW_WAVE_MODE             = 0x07,
    INSTANT_MOUSE_BREATHING_MODE                = 0x08,
    ANT_MOUSE_BREATHING_MODE                    = 0x09,
    INSTANT_MOUSE_ENRAPTURED_MODE               = 0xBB,
    INSTANT_MOUSE_FLICKER_MODE                  = 0xB8,
    INSTANT_MOUSE_RIPPLE_MODE                   = 0xBA,
    INSTANT_MOUSE_STARTRECK_MODE                = 0xB9,

};

enum
{
    INSTANT_MOUSE_SPEED_MIN                     = 0x00,
    INSTANT_MOUSE_SPEED_MAX                     = 0x05,
    INSTANT_MOUSE_BRIGHTNESS_MIN                = 0x00,
    INSTANT_MOUSE_BRIGHTNESS_MAX                = 0x07
};

class InstantMouseController
{
public:
    InstantMouseController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~InstantMouseController();

    std::string GetNameString();
    std::string GetSerialString();
    std::string GetDeviceLocation();
    uint16_t GetPID();

    void SetMode(uint8_t mode_value, uint8_t speed, uint8_t brightness, uint8_t direction);
    void SendColor(RGBColor color);

private:
    hid_device* dev;
    std::string location;
    std::string name;
    uint16_t    pid;
};
