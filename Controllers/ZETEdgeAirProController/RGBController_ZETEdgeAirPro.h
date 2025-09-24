/*---------------------------------------------------------*\
| RGBController_ZETEdgeAirPro.h                             |
|                                                           |
|   RGBController for ZET Edge Air Pro                      |
|                                                           |
|   Morgan Guimard (morg)                       29 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    ZETEdgeAirProController* controller;
};
