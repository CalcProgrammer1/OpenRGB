/*---------------------------------------------------------*\
| RGBController_NuPhyKeyboard.h                             |
|                                                           |
|   RGBController for NuPhy QMK/VIA RGB Matrix keyboards    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "NuPhyKeyboardController.h"
#include "RGBController.h"

class RGBController_NuPhyKeyboard : public RGBController
{
public:
    RGBController_NuPhyKeyboard(NuPhyKeyboardController* controller_ptr);
    ~RGBController_NuPhyKeyboard();

    void SetupZones();
    void ResizeZone(int zone, int new_size);
    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);
    void DeviceUpdateMode();
    void DeviceSaveMode();

private:
    NuPhyKeyboardController* controller;
};
