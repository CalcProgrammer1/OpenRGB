/*---------------------------------------------------------*\
| RGBController_RedSquareKeyrox.h                           |
|                                                           |
|   RGBController for Red Square Keyrox                     |
|                                                           |
|   cafeed28                                    03 Nov 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RedSquareKeyroxController.h"

class RGBController_RedSquareKeyrox : public RGBController
{
public:
    RGBController_RedSquareKeyrox(RedSquareKeyroxController* controller_ptr);
    ~RGBController_RedSquareKeyrox();

    void                SetupZones();

    void                DeviceUpdateLEDs();
    void                DeviceUpdateZoneLEDs(int zone);
    void                DeviceUpdateSingleLED(int led);

    void                DeviceUpdateMode();

private:
    RedSquareKeyroxController* controller;
};
