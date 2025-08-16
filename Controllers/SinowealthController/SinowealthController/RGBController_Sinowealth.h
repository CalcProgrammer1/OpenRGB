/*---------------------------------------------------------*\
| RGBController_Sinowealth.h                                |
|                                                           |
|   RGBController for Sinowealth mice, including Glorious   |
|                                                           |
|   Niels Westphal (crashniels)                 20 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SinowealthController.h"

class RGBController_Sinowealth : public RGBController
{
public:
    RGBController_Sinowealth(SinowealthController* controller_ptr);
    ~RGBController_Sinowealth();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SinowealthController* controller;
};
