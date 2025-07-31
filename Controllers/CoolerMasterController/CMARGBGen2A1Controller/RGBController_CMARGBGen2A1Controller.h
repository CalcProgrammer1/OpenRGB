/*---------------------------------------------------------*\
| RGBController_CMARGBGen2A1Controller.h                    |
|                                                           |
|   Driver for Cooler Master ARGB Gen 2 A1 controller       |
|                                                           |
|   Morgan Guimard (morg)                       26 Jun 2022 |
|   Fabian R (kderazorback)                     11 Aug 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "RGBController.h"
#include "CMARGBGen2A1Controller.h"

class RGBController_CMARGBGen2A1Controller : public RGBController
{
public:
    RGBController_CMARGBGen2A1Controller(CMARGBGen2A1controller* controller_ptr);
    ~RGBController_CMARGBGen2A1Controller();

    void SetupZones();
    void ResizeZone(int zone, int new_size);
    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSegmentLEDs(int zone, int subchannel);
    void UpdateSingleLED(int led);
    void DeviceUpdateMode();
    void DeviceSaveMode();

private:
    CMARGBGen2A1controller*   controller;
};
