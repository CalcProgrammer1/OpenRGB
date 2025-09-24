/*---------------------------------------------------------*\
| RGBController_TrustGXT114.h                               |
|                                                           |
|   RGBController for Trust GXT 114                         |
|                                                           |
|   Morgan Guimard (morg)                       24 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "TrustGXT114Controller.h"

class RGBController_TrustGXT114 : public RGBController
{
public:
    RGBController_TrustGXT114(TrustGXT114Controller* controller_ptr);
    ~RGBController_TrustGXT114();

    void SetupZones();

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    TrustGXT114Controller* controller;
};
