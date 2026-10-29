/*---------------------------------------------------------*\
| RGBController_ClevoKeyboard.h                             |
|                                                           |
|   RGBController for Clevo per-key RGB keyboard            |
|   Supports ITE 8291 (PID 0x600B) and ITE 829x (0x8910)    |
|                                                           |
|   Kyle Cascade (kyle@cascade.family)          16 Jan 2026 |
|   Valentin Lobstein (balgogan@protonmail.com) 27 Mar 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ClevoKeyboardController.h"
#include "ClevoKeyboardDevices.h"

class RGBController_ClevoKeyboard : public RGBController
{
public:
    explicit RGBController_ClevoKeyboard(ClevoKeyboardController* controller_ptr);
    ~RGBController_ClevoKeyboard();

    void        SetupZones();

    void        DeviceUpdateLEDs(std::vector<RGBColor>& update_colors);
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

    void        DeviceConfigureDevice() override;
    void        DeviceUpdateDeviceSpecificConfiguration() override;

private:
    ClevoKeyboardController*        controller;
    std::vector<RGBColor*>          buffer_map;
    std::vector<unsigned int>       default_led_values;
    RGBColor                        null_color;

    void        InitDeviceSpecificConfiguration();
    void        ApplyLEDMap();
    void        BuildBufferMap();
};
