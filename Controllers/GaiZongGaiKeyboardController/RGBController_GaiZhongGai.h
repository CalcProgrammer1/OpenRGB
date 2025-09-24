/*---------------------------------------------------------*\
| RGBController_GaiZhongGai.h                               |
|                                                           |
|   RGBController for GaiZhongGai keyboard                  |
|                                                           |
|   An Yang                                     24 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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
    void DeviceResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    GaiZhongGaiKeyboardController*    controller;
};
