/*---------------------------------------------------------*\
| RGBController_KasaSmart.h                                 |
|                                                           |
|   RGBController for Kasa Smart bulbs                      |
|                                                           |
|   Devin Wendt (umbreon222)                    16 Feb 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "KasaSmartController.h"

class RGBController_KasaSmart : public RGBController
{
public:
    RGBController_KasaSmart(KasaSmartController* controller_ptr);
    ~RGBController_KasaSmart();

    void SetupZones();

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    KasaSmartController* controller;
};
