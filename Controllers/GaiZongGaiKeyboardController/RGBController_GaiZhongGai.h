/*---------------------------------------------------------*\
| RGBController_GaiZhongGai.h                               |
|                                                           |
|   RGBController for GaiZhongGai keyboard                  |
|                                                           |
|   An Yang                                     24 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once
#include "RGBController.h"
#include "GaiZhongGaiController.h"

class RGBController_GaiZhongGaiKeyboard : public RGBController
{
public:
    RGBController_GaiZhongGaiKeyboard(GaiZhongGaiKeyboardController* controller_ptr);
    ~RGBController_GaiZhongGaiKeyboard();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    GaiZhongGaiKeyboardController*    controller;
};
