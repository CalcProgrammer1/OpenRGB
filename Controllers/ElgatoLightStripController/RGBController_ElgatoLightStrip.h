/*---------------------------------------------------------*\
| RGBController_ElgatoLightStrip.h                          |
|                                                           |
|   RGBController for Elgato Light Strip                    |
|                                                           |
|   Monks (@iamtherealestmonkey)                01 Nov 2021 |
|   DomePlaysHD                                 12 Mar 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ElgatoLightStripController.h"

class RGBController_ElgatoLightStrip : public RGBController
{
    public:
        RGBController_ElgatoLightStrip(ElgatoLightStripController* controller_ptr);
        ~RGBController_ElgatoLightStrip();

        void SetupZones();

        void DeviceUpdateLEDs();
        void DeviceUpdateZoneLEDs(int zone);
        void DeviceUpdateSingleLED(int led);

        void DeviceUpdateMode();

    private:
        ElgatoLightStripController* controller;
};
