/*---------------------------------------------------------*\
| RGBController_MintakaKeyboard.h                           |
|                                                           |
|   Driver for VSG Mintaka Devices keyboard lighting        |
|   Based on KeychronKeyboardController                     |
|                                                           |
|   Federico Scodelaro (pudymody)               08 Oct 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/
#pragma once

#include "RGBController.h"
#include "MintakaKeyboardController.h"

class RGBController_MintakaKeyboard : public RGBController
{
public:
    RGBController_MintakaKeyboard(MintakaKeyboardController* controller_ptr);
    ~RGBController_MintakaKeyboard();

    void SetupZones();
    void DeviceResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    MintakaKeyboardController* controller;
};
