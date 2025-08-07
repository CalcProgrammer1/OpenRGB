/*---------------------------------------------------------*\
| RGBController_EVGAGP102.h                                 |
|                                                           |
|   RGBController for EVGA GP102 GPU                        |
|                                                           |
|   Fabricio Murta (avengerx)                   31 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "EVGAGP102Controller.h"

class RGBController_EVGAGP102 : public RGBController
{
public:
    RGBController_EVGAGP102(std::vector<EVGAGP102Controller*> controller_list);
    ~RGBController_EVGAGP102();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    std::vector<EVGAGP102Controller*> controllers;
};
