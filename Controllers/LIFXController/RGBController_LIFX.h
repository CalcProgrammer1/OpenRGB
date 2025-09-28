/*---------------------------------------------------------*\
| RGBController_LIFX.cpp                                    |
|                                                           |
|   RGBController for LIFX                                  |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      05 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LIFXController.h"

class RGBController_LIFX : public RGBController
{
public:
    RGBController_LIFX(LIFXController* controller_ptr);
    ~RGBController_LIFX();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LIFXController* controller;
};
