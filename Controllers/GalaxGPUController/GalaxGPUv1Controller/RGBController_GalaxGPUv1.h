/*---------------------------------------------------------*\
| RGBController_GalaxGPUv1.h                                |
|                                                           |
|   RGBController for Galax/KFA2 GPU                        |
|                                                           |
|   Niels Westphal (crashniels)                 12 Jul 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GalaxGPUv1Controller.h"

class RGBController_GalaxGPUv1 : public RGBController
{
public:
    RGBController_GalaxGPUv1(GalaxGPUv1Controller* controller_ptr);
    ~RGBController_GalaxGPUv1();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    GalaxGPUv1Controller* controller;

    int        GetDeviceMode();
};
