/*---------------------------------------------------------*\
| RGBController_XG270QG.h                                   |
|                                                           |
|   RGBController for ViewSonic XG270QG                     |
|                                                           |
|   Lanzaa                                      23 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "VS_XG270QG_Controller.h"
#include "RGBController.h"

class RGBController_XG270QG : public RGBController
{
public:
    RGBController_XG270QG(VS_XG270QG_Controller* controller_ptr);

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    VS_XG270QG_Controller* controller;
};
