/*---------------------------------------------------------*\
| RGBController_MSIClutchGM11.h                             |
|                                                           |
|   RGBController for MSI Clutch GM11 Gaming Mouse          |
|                                                           |
|   Anatoly U. (TTolikoLL)                      20 SEP 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "MSIClutchGM11Controller.h"

class RGBController_MSIClutchGM11 : public RGBController
{
public:
    RGBController_MSIClutchGM11(MSIClutchGM11Controller* controller_ptr);
    ~RGBController_MSIClutchGM11();

    void SetupZones();

    void DeviceUpdateLEDs(std::vector<RGBColor>& update_colors);
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    MSIClutchGM11Controller* controller;
};