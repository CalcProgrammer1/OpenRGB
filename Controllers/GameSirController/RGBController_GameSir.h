/*---------------------------------------------------------*\
| RGBController_GameSir.h                                   |
|                                                           |
|   GameSir RGB Device                                      |
|                                                           |
|   Added by OpenRGB Community                  08 Aug 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GameSirController.h"

class RGBController_GameSir : public RGBController
{
public:
    RGBController_GameSir(GameSirController* controller_ptr);
    ~RGBController_GameSir();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);
    void        DeviceUpdateMode();

private:
    GameSirController* controller;
};
