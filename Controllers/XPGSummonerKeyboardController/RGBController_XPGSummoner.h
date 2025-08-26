/*---------------------------------------------------------*\
| RGBController_XPGSummoner.h                               |
|                                                           |
|   RGBController for XPG Summoner keyboard                 |
|                                                           |
|   Erick Granados (eriosgamer)                             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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
    void        ResizeZone(int zone, int new_size);
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);
    void        DeviceUpdateMode();

private:
    XPGSummonerController* controller;
};
