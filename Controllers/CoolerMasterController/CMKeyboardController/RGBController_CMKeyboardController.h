/*---------------------------------------------------------*\
| RGBController_CMKeyboardController.h                      |
|                                                           |
|   RGBController for Cooler Master keyboards               |
|                                                           |
|   Tam D (too.manyhobbies)                     30 Nov 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CMKeyboardAbstractController.h"
#include "CMKeyboardV1Controller.h"
#include "CMKeyboardV2Controller.h"
#include "CMKeyboardDevices.h"

class RGBController_CMKeyboardController : public RGBController
{
public:
    RGBController_CMKeyboardController(CMKeyboardAbstractController* pController);
    ~RGBController_CMKeyboardController();

    void SetupZones();

    void DeviceUpdateLEDs();
    void DeviceUpdateSingleLED(int led);
    void DeviceUpdateZoneLEDs(int zone_idx);

    void SetCustomMode();
    void DeviceUpdateMode();

private:
    CMKeyboardAbstractController*   m_pController;;
    KeyboardLayoutManager*          m_pLayoutManager;
    KEYBOARD_LAYOUT                 m_keyboardLayout;
    layout_values                   m_layoutValues;
};
