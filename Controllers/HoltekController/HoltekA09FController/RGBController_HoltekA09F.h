/*---------------------------------------------------------*\
| RGBController_HoltekA09F.h                                |
|                                                           |
|   RGBController for Holtek A09F RGB gaming mouse          |
|   (E-Signal LUOM G10, VID 04D9 PID A09F)                  |
|                                                           |
|   Juampe                                         2026     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "HoltekA09FController.h"

class RGBController_HoltekA09F : public RGBController
{
public:
    RGBController_HoltekA09F(HoltekA09FController* controller_ptr);
    ~RGBController_HoltekA09F();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    HoltekA09FController* controller;
};
