/*---------------------------------------------------------*\
| RGBController_NvidiaESA.h                                 |
|                                                           |
|   RGBController for NVIDIA ESA                            |
|                                                           |
|   Morgan Guimard (morg)                       18 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "NvidiaESAController.h"

class RGBController_NvidiaESA : public RGBController
{
public:
    RGBController_NvidiaESA(NvidiaESAController* controller_ptr);
    ~RGBController_NvidiaESA();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    NvidiaESAController* controller;
};
