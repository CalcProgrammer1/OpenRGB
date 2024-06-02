/*---------------------------------------------------------*\
| RGBController_MSIRGB.h                                    |
|                                                           |
|   RGBController for MSI-RGB motherboard                   |
|                                                           |
|   Adam Honse (CalcProgrammer1)                14 Feb 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "MSIRGBController.h"

class RGBController_MSIRGB : public RGBController
{
public:
    RGBController_MSIRGB(MSIRGBController* controller_ptr);
    ~RGBController_MSIRGB();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    MSIRGBController*       controller;
};
