/*---------------------------------------------------------*\
| RGBController_RoccatHordeAimo.h                           |
|                                                           |
|   RGBController for Roccat Horde Aimo                     |
|                                                           |
|   Morgan Guimard (morg)                       24 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RoccatHordeAimoController.h"

class RGBController_RoccatHordeAimo : public RGBController
{
public:
    RGBController_RoccatHordeAimo(RoccatHordeAimoController* controller_ptr);
    ~RGBController_RoccatHordeAimo();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RoccatHordeAimoController* controller;
};
