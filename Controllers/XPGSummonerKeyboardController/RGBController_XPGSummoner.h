/*---------------------------------------------------------*\
| RGBController_XPGSummoner.h                               |
|                                                           |
|   RGBController for XPG Summoner keyboard                 |
|                                                           |
|   Erick Granados (eriosgamer)                             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "XPGSummonerController.h"

class RGBController_XPGSummoner : public RGBController
{
public:
    RGBController_XPGSummoner(XPGSummonerController* controller_ptr);
    ~RGBController_XPGSummoner();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    XPGSummonerController* controller;
};
