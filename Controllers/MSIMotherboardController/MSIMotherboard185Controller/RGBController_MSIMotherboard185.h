/*---------------------------------------------------------*\
| RGBController_MSIMotherboard185.h                         |
|                                                           |
|   RGBController for MSI Mystic Light 185-byte motherboard |
|                                                           |
|   T-bond                                      03 Apr 2020 |
|   Adam Honse                                  06 Mar 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "RGBController.h"
#include "MSIMotherboard185Controller.h"

class RGBController_MSIMotherboard185: public RGBController
{
public:
    RGBController_MSIMotherboard185(MSIMotherboard185Controller* controller_ptr);
    ~RGBController_MSIMotherboard185();

    void        SetupZones();
    void        DeviceConfigureZone(int zone_idx);

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    void        SetupModes();
    void        UpdateLed
                    (
                    int zone,
                    int led
                    );
    void        SetupMode
                    (
                    const char      *name,
                    MSI_MODE        mode,
                    unsigned int    flags
                    );
    int         GetDeviceMode();
    void        GetDeviceConfig();

    MSIMotherboard185Controller*    controller;
    MSI_ZONE                        last_resizable_zone;
};
