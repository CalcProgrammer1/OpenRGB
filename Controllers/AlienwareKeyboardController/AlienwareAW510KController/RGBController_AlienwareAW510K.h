/*---------------------------------------------------------*\
| RGBController_AlienwareAW510K.h                           |
|                                                           |
|   RGBController for Alienware AW510K keyboard             |
|                                                           |
|   Mohamad Sallal (msallal)                    22 May 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AlienwareAW510KController.h"

class RGBController_AlienwareAW510K : public RGBController
{
public:
    RGBController_AlienwareAW510K(AlienwareAW510KController* controller_ptr);
    ~RGBController_AlienwareAW510K();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    AlienwareAW510KController*  controller;
    std::vector<RGBColor>       current_colors;
};
