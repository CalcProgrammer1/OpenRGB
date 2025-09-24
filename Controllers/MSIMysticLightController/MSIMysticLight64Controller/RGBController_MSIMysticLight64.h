/*---------------------------------------------------------*\
| RGBController_MSIMysticLight64.h                          |
|                                                           |
|   RGBController for MSI Mystic Light 64-byte motherboard  |
|                                                           |
|   T-bond                                      03 Apr 2020 |
|   Adam Honse                                  06 Mar 2021 |
|   Elchanan Haas                               23 Aug 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "MSIMysticLight64Controller.h"

class RGBController_MSIMysticLight64 : public RGBController
{
public:
    RGBController_MSIMysticLight64(MSIMysticLight64Controller* controller_ptr);
    ~RGBController_MSIMysticLight64();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    MSIMysticLight64Controller*       controller;
    void        SetupModes();
    void        SetupMode
                (
                const char      *name,
                MSI_64_MODE      mode,
                unsigned int     flags
                );
};
