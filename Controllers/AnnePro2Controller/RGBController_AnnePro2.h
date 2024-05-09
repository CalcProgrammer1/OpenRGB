/*---------------------------------------------------------*\
| RGBController_AnnePro2.h                                  |
|                                                           |
|   RGBController for Obins Lab AnnePro2 keyboard           |
|                                                           |
|   Sergey Gavrilov (DrZlo13)                   06 Jun 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AnnePro2Controller.h"

class RGBController_AnnePro2 : public RGBController
{
public:
    RGBController_AnnePro2(AnnePro2Controller* controller_ptr);
    ~RGBController_AnnePro2();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    AnnePro2Controller*   controller;
};
