/*---------------------------------------------------------*\
| MSIMysticLight185Controller.cpp                           |
|                                                           |
|   RGBController for MSI Mystic Light 761-byte motherboard |
|                                                           |
|                                                           |
|   rom4ster                                    11 Jun 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "RGBController.h"
#include "MSIMysticLight761Controller.h"

class RGBController_MSIMysticLight761: public RGBController
{
public:
    RGBController_MSIMysticLight761(MSIMysticLight761Controller* controller_ptr);
    ~RGBController_MSIMysticLight761();

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

    MSIMysticLight761Controller*    controller;
    MSI_ZONE                        last_resizable_zone;
};
