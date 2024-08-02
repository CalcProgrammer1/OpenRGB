/*---------------------------------------------------------*\
| RGBController_Sinowealth1007.h                            |
|                                                           |
|   RGBController for Sinowealth mice with PID 1007         |
|                                                           |
|   Moon_darker (Vaker)                         25 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SinowealthController1007.h"

class RGBController_Sinowealth1007 : public RGBController
{
public:
    RGBController_Sinowealth1007(SinowealthController1007* controller_ptr);
    ~RGBController_Sinowealth1007();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SinowealthController1007* controller;
};
