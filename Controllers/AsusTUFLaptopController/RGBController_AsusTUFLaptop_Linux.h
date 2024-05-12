/*---------------------------------------------------------*\
| RGBController_AsusTUFLaptop_Linux.h                       |
|                                                           |
|   RGBController for ASUS TUF laptop                       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AsusTUFLaptopController_Linux.h"

class RGBController_AsusTUFLaptopLinux : public RGBController
{
public:
    RGBController_AsusTUFLaptopLinux(AsusTUFLaptopLinuxController* controller_ptr);

    void SetupZones() override;

    void ResizeZone(int zone, int new_size) override;

    void DeviceUpdateLEDs() override;
    void UpdateZoneLEDs(int zone) override;
    void UpdateSingleLED(int led) override;

    void DeviceUpdateMode() override;

private:
    AsusTUFLaptopLinuxController* controller;
};
