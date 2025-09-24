/*---------------------------------------------------------*\
| RGBController_EVisionKeyboard.h                           |
|                                                           |
|   RGBController for EVision keyboard (Redragon, Glorious, |
|   Ajazz, Tecware, and many other brands)                  |
|                                                           |
|   Adam Honse (CalcProgrammer1)                25 Mar 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "EVisionKeyboardController.h"

class RGBController_EVisionKeyboard : public RGBController
{
public:
    RGBController_EVisionKeyboard(EVisionKeyboardController* controller_ptr);
    ~RGBController_EVisionKeyboard();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    EVisionKeyboardController*  controller;
};
