/*---------------------------------------------------------*\
| RGBController_BlinkyTape.h                                |
|                                                           |
|   RGBController for BlinkyTape                            |
|                                                           |
|   Matt Mets (matt@blinkinlabs.com)            01 Jul 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "serial_port.h"
#include "BlinkyTapeController.h"

class RGBController_BlinkyTape : public RGBController
{
public:
    RGBController_BlinkyTape(BlinkyTapeController* controller_ptr);
    ~RGBController_BlinkyTape();

    void        SetupZones();

    void        DeviceResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    BlinkyTapeController*   controller;
};
