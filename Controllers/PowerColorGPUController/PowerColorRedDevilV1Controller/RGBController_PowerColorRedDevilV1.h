/*---------------------------------------------------------*\
| RGBController_PowerColorRedDevilV1.cpp                    |
|                                                           |
|   Driver for PowerColor Red Devil GPU                     |
|                                                           |
|   Jana Rettig (SapphicKitten)                 14 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "PowerColorRedDevilV1Controller.h"

class RGBController_PowerColorRedDevilV1 : public RGBController
{
public:
    RGBController_PowerColorRedDevilV1(PowerColorRedDevilV1Controller* controller_ptr);
    ~RGBController_PowerColorRedDevilV1();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    PowerColorRedDevilV1Controller* controller;
};
