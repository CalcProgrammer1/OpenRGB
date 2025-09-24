/*---------------------------------------------------------*\
| RGBController_RealtekBridge.h                             |
|                                                           |
|   Controller for Realtek USB to SSD Bridge ICs            |
|                                                           |
|   Jerry Fan (JerryFan0612)                    13 Mar 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RealtekBridgeController.h"

class RGBController_RealtekBridge : public RGBController
{
public:
    RGBController_RealtekBridge(RealtekBridgeController* controller_ptr);
    ~RGBController_RealtekBridge();

    void        SetupModes();
    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RealtekBridgeController* controller;
};
