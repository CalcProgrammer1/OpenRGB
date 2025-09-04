/*---------------------------------------------------------*\
| RGBController_PowerColorRedDevilV2.h                      |
|                                                           |
|   Driver for PowerColor Red Devil GPU                     |
|                                                           |
|   Nexrem                                      15 Aug 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "PowerColorRedDevilV2Controller.h"

class RGBController_PowerColorRedDevilV2 : public RGBController
{
public:
    RGBController_PowerColorRedDevilV2(PowerColorRedDevilV2Controller* controller_ptr);
    ~RGBController_PowerColorRedDevilV2();

    void        SetupZones();

    void        ResizeZone(int, int);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int);
    void        UpdateSingleLED(int);

    void        DeviceUpdateMode();

private:
    PowerColorRedDevilV2Controller      *controller;
    /*------------------------------------------------------------------*\
    | To optimize color writes we store a copy of the colors in order to |
    | later only write changed colors                                    |
    \*------------------------------------------------------------------*/
    std::vector<RGBColor>               colors_copy;

    void        ReadConfig();
};
