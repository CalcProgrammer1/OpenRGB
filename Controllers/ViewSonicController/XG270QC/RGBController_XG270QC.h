/*---------------------------------------------------------*\
| RGBController_XG270QC.h                                   |
|                                                           |
|   RGBController for ViewSonic XG270QC                     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "VS_XG270QC_Controller.h"
#include "RGBController.h"

class RGBController_XG270QC : public RGBController
{
public:
    RGBController_XG270QC(VS_XG270QC_Controller* controller_ptr);

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    VS_XG270QC_Controller* controller;
};
