/*---------------------------------------------------------*\
| RGBController_GigabyteAorusMouse.h                        |
|                                                           |
|   RGBController for Gigabyte Aorus mouse                  |
|                                                           |
|   Morgan Guimard (morg)                       29 Dec 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GigabyteAorusMouseController.h"

class RGBController_GigabyteAorusMouse : public RGBController
{
public:
    RGBController_GigabyteAorusMouse(GigabyteAorusMouseController* controller_ptr);
    ~RGBController_GigabyteAorusMouse();

    void SetupZones();

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    GigabyteAorusMouseController* controller;
};
