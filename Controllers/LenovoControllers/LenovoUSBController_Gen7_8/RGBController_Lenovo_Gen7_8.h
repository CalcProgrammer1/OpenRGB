/*---------------------------------------------------------*\
| RGBController_Lenovo_Gen7_8.h                             |
|                                                           |
|   RGBController for Lenovo Gen7 and Gen8 devices          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LenovoUSBController_Gen7_8.h"

enum
{
    LENOVO_LEGION_GEN7_8_MODE_SCREW_RAINBOW                  = 0x01,
    LENOVO_LEGION_GEN7_8_MODE_RAINBOW_WAVE                   = 0x02,
    LENOVO_LEGION_GEN7_8_MODE_COLOR_CHANGE                   = 0x03,
    LENOVO_LEGION_GEN7_8_MODE_COLOR_PULSE                    = 0x04,
    LENOVO_LEGION_GEN7_8_MODE_COLOR_WAVE                     = 0x05,
    LENOVO_LEGION_GEN7_8_MODE_SMOOTH                         = 0x06,
    LENOVO_LEGION_GEN7_8_MODE_RAIN                           = 0x07,
    LENOVO_LEGION_GEN7_8_MODE_RIPPLE                         = 0x08,
    LENOVO_LEGION_GEN7_8_MODE_AUDIO_BOUNCE                   = 0x09,
    LENOVO_LEGION_GEN7_8_MODE_AUDIO_RIPPLE                   = 0x0A,
    LENOVO_LEGION_GEN7_8_MODE_STATIC                         = 0x0B,
    LENOVO_LEGION_GEN7_8_MODE_TYPE                           = 0x0C,
    LENOVO_LEGION_GEN7_8_MODE_DIRECT                         = 0x0D,
};

class LenovoRGBController_Gen7_8 : public RGBController
{
public:
    LenovoRGBController_Gen7_8(LenovoGen7And8USBController* controller_ptr);
    ~LenovoRGBController_Gen7_8();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LenovoGen7And8USBController* controller;
    std::vector<led_group> GetLedGroups();
    void ReadDeviceSettings();
    std::unordered_map<unsigned int, size_t> led_id_to_index;
    int last_mode = 0;
    uint8_t brightness = 0x00;
    uint8_t profile_id = 0x01;
};
