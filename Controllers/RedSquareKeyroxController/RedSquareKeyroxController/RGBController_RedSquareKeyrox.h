/*---------------------------------------------------------*\
| RGBController_RedSquareKeyrox.h                           |
|                                                           |
|   RGBController for Red Square Keyrox                     |
|                                                           |
|   cafeed28                                    03 Nov 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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
    void                ResizeZone(int zone, int new_size);

    void                DeviceUpdateLEDs();
    void                UpdateZoneLEDs(int zone);
    void                UpdateSingleLED(int led);

    void                DeviceUpdateMode();

private:
    RedSquareKeyroxController* controller;
};
