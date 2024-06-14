/*---------------------------------------------------------*\
| RGBController_RoccatKova.h                                |
|                                                           |
|   RGBController for Roccat Kova                           |
|                                                           |
|   Gustash                                     01 Dec 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RoccatKovaController.h"

class RGBController_RoccatKova : public RGBController
{
public:
    RGBController_RoccatKova(RoccatKovaController *controller_ptr);
    ~RGBController_RoccatKova();

    void SetupZones();

    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    RoccatKovaController *controller;
};
