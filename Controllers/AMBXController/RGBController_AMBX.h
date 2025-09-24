/*---------------------------------------------------------*\
| RGBController_AMBX.h                                      |
|                                                           |
|   RGB Controller for Philips amBX Gaming lights           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AMBXController.h"

class RGBController_AMBX : public RGBController
{
public:
    RGBController_AMBX(AMBXController* controller_ptr);
    ~RGBController_AMBX();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    AMBXController* controller;
};
