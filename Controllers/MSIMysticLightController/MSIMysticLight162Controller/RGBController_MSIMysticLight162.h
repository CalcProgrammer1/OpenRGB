/*---------------------------------------------------------*\
| RGBController_MSIMysticLight162.h                         |
|                                                           |
|   RGBController for MSI Mystic Light 162-byte motherboard |
|                                                           |
|   T-bond                                      03 Apr 2020 |
|   Adam Honse                                  06 Mar 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "RGBController.h"
#include "MSIMysticLight162Controller.h"

class RGBController_MSIMysticLight162: public RGBController
{
public:
    RGBController_MSIMysticLight162(MSIMysticLight162Controller* controller_ptr);
    ~RGBController_MSIMysticLight162();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

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

    MSIMysticLight162Controller*    controller;
};
