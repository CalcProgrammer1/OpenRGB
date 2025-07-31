/*---------------------------------------------------------*\
| RGBController_AMBX.h                                      |
|                                                           |
|   RGB Controller for Philips amBX Gaming lights           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    AMBXController* controller;
};
