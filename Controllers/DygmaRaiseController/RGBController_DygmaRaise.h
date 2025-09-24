/*---------------------------------------------------------*\
| RGBController_DygmaRaise.h                                |
|                                                           |
|   RGBController for Dygma Raise keyboard                  |
|                                                           |
|   Timo Schlegel (@eispalast)                  Dec 12 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "DygmaRaiseController.h"

class RGBController_DygmaRaise : public RGBController
{
public:
    RGBController_DygmaRaise(DygmaRaiseController* controller_ptr);
    ~RGBController_DygmaRaise();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    DygmaRaiseController*   controller;
};
