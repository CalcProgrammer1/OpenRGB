/*---------------------------------------------------------*\
| RGBController_KlipschLumina.h                             |
|                                                           |
|   RGBController for Klipsch ProMedia Lumina speakers      |
|                                                           |
|   wlof                                        26 Oct 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "KlipschLuminaController.h"

class RGBController_KlipschLumina : public RGBController
{
public:
    RGBController_KlipschLumina(KlipschLuminaController* controller_ptr);
    ~RGBController_KlipschLumina();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    KlipschLuminaController*    controller;
};
