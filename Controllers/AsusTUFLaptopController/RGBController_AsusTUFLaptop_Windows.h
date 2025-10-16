/*---------------------------------------------------------*\
| RGBController_AsusTUFLaptop_Windows.h                     |
|                                                           |
|   RGBController for ASUS TUF laptop                       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "AsusTUFLaptopController_Windows.h"
#include "RGBController.h"

class RGBController_AsusTUFLaptopWMI : public RGBController
{
public:
    RGBController_AsusTUFLaptopWMI(AsusTUFLaptopController* controller_ptr);
    virtual ~RGBController_AsusTUFLaptopWMI();

    void SetupZones();

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();
    void DeviceSaveMode();

private:
    AsusTUFLaptopController* controller;

    void ReadConfiguration();
    void ControllerSetMode(bool save);
};
