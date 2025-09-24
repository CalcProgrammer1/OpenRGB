/*---------------------------------------------------------*\
| RGBController_EpomakerController.h                        |
|                                                           |
|   RGBController for Epomaker keyboard                     |
|                                                           |
|   Alvaro Munoz (alvaromunoz)                  05 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "EpomakerController.h"

class RGBController_EpomakerController : public RGBController
{
public:
    RGBController_EpomakerController(EpomakerController* controller_ptr);
    ~RGBController_EpomakerController();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    EpomakerController* controller;
};
