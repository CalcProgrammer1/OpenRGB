/*---------------------------------------------------------*\
| RGBController_AlienwareAW410K.h                           |
|                                                           |
|   RGBController for Alienware AW410K keyboard             |
|                                                           |
|   based on AW510K controller by Mohamad Sallal (msallal)  |
|   Dominik Mikolajczyk (dmiko)                 23 Oct 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AlienwareAW410KController.h"

class RGBController_AlienwareAW410K : public RGBController
{
public:
    RGBController_AlienwareAW410K(AlienwareAW410KController* controller_ptr);
    ~RGBController_AlienwareAW410K();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    AlienwareAW410KController*  controller;
    std::vector<RGBColor>       current_colors;
};
