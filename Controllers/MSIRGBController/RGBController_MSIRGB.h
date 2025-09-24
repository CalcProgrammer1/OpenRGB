/*---------------------------------------------------------*\
| RGBController_MSIRGB.h                                    |
|                                                           |
|   RGBController for MSI-RGB motherboard                   |
|                                                           |
|   Adam Honse (CalcProgrammer1)                14 Feb 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    MSIRGBController*       controller;
};
