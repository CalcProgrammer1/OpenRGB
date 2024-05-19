/*---------------------------------------------------------*\
| RGBController_GigabyteSuperIORGB.h                        |
|                                                           |
|   RGBController for Gigabyte Aorus Super IO motherboard   |
|                                                           |
|   Ryan Frankcombe (422gRdHuX5uk)              11 Sep 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GigabyteSuperIORGBController.h"

class RGBController_GigabyteSuperIORGB : public RGBController
{
public:
    RGBController_GigabyteSuperIORGB(GigabyteSuperIORGBController* controller_ptr);
    ~RGBController_GigabyteSuperIORGB();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    GigabyteSuperIORGBController*       controller;
};
