/*---------------------------------------------------------*\
| RGBController_ZETEdgeAirPro.h                             |
|                                                           |
|   RGBController for ZET Edge Air Pro                      |
|                                                           |
|   Morgan Guimard (morg)                       29 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ZETEdgeAirProController.h"

class RGBController_ZETEdgeAirPro : public RGBController
{
public:
    RGBController_ZETEdgeAirPro(ZETEdgeAirProController* controller_ptr);
    ~RGBController_ZETEdgeAirPro();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    ZETEdgeAirProController* controller;
};
