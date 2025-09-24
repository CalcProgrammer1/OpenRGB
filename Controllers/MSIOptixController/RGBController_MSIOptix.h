/*---------------------------------------------------------*\
| RGBController_MSIOptix.h                                  |
|                                                           |
|   RGBController for MSI Optix                             |
|                                                           |
|   Morgan Guimard (morg)                       10 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "MSIOptixController.h"

class RGBController_MSIOptix : public RGBController
{
public:
    RGBController_MSIOptix(MSIOptixController* controller_ptr);
    ~RGBController_MSIOptix();

    void SetupZones();

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    MSIOptixController* controller;
};
