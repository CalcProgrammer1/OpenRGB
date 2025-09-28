/*---------------------------------------------------------*\
| RGBController_ASRockPolychromeUSB.h                       |
|                                                           |
|   RGBController for ASRock Polychrome USB motherboards    |
|                                                           |
|   Ed Kambulow (dredvard)                      20 Dec 2020 |
|   Shady Nawara (ShadyNawara)                  16 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ASRockPolychromeUSBController.h"

class RGBController_PolychromeUSB : public RGBController
{
public:
    RGBController_PolychromeUSB(PolychromeUSBController* controller_ptr);

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    PolychromeUSBController*        controller;
    std::vector<PolychromeZoneInfo> zones_info;

    unsigned char GetDeviceMode(unsigned char zone);
};
