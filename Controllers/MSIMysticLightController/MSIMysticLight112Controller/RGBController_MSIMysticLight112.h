/*---------------------------------------------------------*\
| RGBController_MSIMysticLight112.h                         |
|                                                           |
|   RGBController for MSI Mystic Light 112-byte motherboard |
|                                                           |
|   thombo                                      17 Dec 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "MSIMysticLight112Controller.h"

class RGBController_MSIMysticLight112: public RGBController
{
public:
    RGBController_MSIMysticLight112(MSIMysticLight112Controller* controller_ptr);
    ~RGBController_MSIMysticLight112();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    MSIMysticLight112Controller*    controller;

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
};
