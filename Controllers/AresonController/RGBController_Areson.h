/*---------------------------------------------------------*\
| RGBController_Areson.h                                    |
|                                                           |
|   RGBController for Areson mice                           |
|                                                           |
|   Morgan Guimard (morg)                       29 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AresonController.h"

class RGBController_Areson : public RGBController
{
public:
    RGBController_Areson(AresonController* controller_ptr);
    ~RGBController_Areson();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    AresonController* controller;
};
