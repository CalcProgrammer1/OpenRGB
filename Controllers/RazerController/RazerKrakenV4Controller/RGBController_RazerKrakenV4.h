/*---------------------------------------------------------*\
| RGBController_RazerKrakenV4.h                             |
|                                                           |
|   RGBController for Razer 64-byte devices                 |
|                                                           |
|   Adam Honse (CalcProgrammer1)                21 Oct 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RazerKrakenV4Controller.h"

enum
{
    RAZER_KRAKEN_V4_MODE_DIRECT,
    RAZER_KRAKEN_V4_MODE_WAVE,
};

class RGBController_RazerKrakenV4 : public RGBController
{
public:
    RGBController_RazerKrakenV4(RazerKrakenV4Controller* controller_ptr);
    ~RGBController_RazerKrakenV4();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RazerKrakenV4Controller*  controller;
};
