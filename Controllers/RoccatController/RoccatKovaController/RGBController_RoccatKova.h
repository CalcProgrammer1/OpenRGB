/*---------------------------------------------------------*\
| RGBController_RoccatKova.h                                |
|                                                           |
|   RGBController for Roccat Kova                           |
|                                                           |
|   Gustash                                     01 Dec 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    RoccatKovaController *controller;
};
