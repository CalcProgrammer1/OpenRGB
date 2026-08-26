/*---------------------------------------------------------*\
| RGBController_LenovoK500.h                                |
|                                                           |
|   Driver for Lenovo Legion K500 keyboard                  |
|                                                           |
|   MX1D                                        10 Aug 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once
#include "RGBController.h"
#include "LenovoK500Controller.h"

class RGBController_LenovoK500 : public RGBController
{
public:
    RGBController_LenovoK500(LenovoK500Controller* controller_ptr);
    ~RGBController_LenovoK500();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LenovoK500Controller*       controller;
    std::vector<RGBColor>       last_colors;
};
