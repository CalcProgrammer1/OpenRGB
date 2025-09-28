/*---------------------------------------------------------*\
| RGBController_DasKeyboard.h                               |
|                                                           |
|   RGBController for Das Keyboard keyboard                 |
|                                                           |
|   Frank Niessen (denk_mal)                    16 Dec 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "DasKeyboardController.h"

enum
{
    DAS_KEYBOARD_MODE_DIRECT         = 0x01,
    DAS_KEYBOARD_MODE_FLASHING       = 0x1F,
    DAS_KEYBOARD_MODE_BREATHING      = 0x08,
    DAS_KEYBOARD_MODE_SPECTRUM_CYCLE = 0x14
};

class RGBController_DasKeyboard : public RGBController
{
public:
    RGBController_DasKeyboard(DasKeyboardController* controller_ptr);
    ~RGBController_DasKeyboard();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    DasKeyboardController*  controller;

    std::vector<RGBColor>   double_buffer;
    bool                    updateDevice;
};
