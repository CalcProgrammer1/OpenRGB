/*---------------------------------------------------------*\
| RGBController_MSIKeyboard1565.h                           |
|                                                           |
|   Driver for MSI Mystic Light MS-1565 keyboard leds       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "MSIKeyboard1565Controller.h"

class RGBController_MSIKeyboard1565 : public RGBController
{
public:
    RGBController_MSIKeyboard1565(MSIKeyboard1565Controller* controller_ptr);
    ~RGBController_MSIKeyboard1565();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    MSIKeyboard1565Controller* controller;

    void        SetupModes();
    void        SetupMode
                (
                const std::string name,
                MS_1565_MODE     mode,
                unsigned int      flags,
                MS_1565_ZONE      zone
                );
    void        SetupZonesMode
                (
                const std::string name,
                MS_1565_MODE mod,
                unsigned int flags
                );
};
