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
    ~RGBController_XG270QC();

    void SetupZones();

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    VS_XG270QC_Controller* controller;
};
