/*---------------------------------------------------------*\
| RGBController_ValkyrieKeyboard.h                          |
|                                                           |
|   RGBController for Valkyrie keyboard                     |
|                                                           |
|   Nollie (Nuonuo)                             06 Dec 2023 |
|   Bartholomew Ho (imnotmental)                01 Feb 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ValkyrieKeyboardController.h"

enum ValkyrieKeyboardMappingLayoutType
{
    NORMAL_LAYOUT,
    PRO_LAYOUT,
};

class RGBController_ValkyrieKeyboard : public RGBController
{
public:
    RGBController_ValkyrieKeyboard(ValkyrieKeyboardController* controller_ptr);
    ~RGBController_ValkyrieKeyboard();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);
    void        DeviceUpdateMode();

private:
    ValkyrieKeyboardController*         controller;
};
