/*---------------------------------------------------------*\
| RGBController_AlienwareAW3423DWF.h                        |
|                                                           |
|   RGBController for the Alienware AW3423DWF monitor       |
|                                                           |
|   Ferréol DUBOIS COLI (Fefe_du_973)           23 Jan 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "AlienwareAW3423DWFController.h"
#include "RGBController.h"

class RGBController_AlienwareAW3423DWF : public RGBController
{
public:
    explicit RGBController_AlienwareAW3423DWF(AlienwareAW3423DWFController* controller_ptr);
    ~RGBController_AlienwareAW3423DWF();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    AlienwareAW3423DWFController* controller;
};
