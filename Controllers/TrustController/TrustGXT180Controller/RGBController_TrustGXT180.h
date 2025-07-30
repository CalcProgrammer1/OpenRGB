/*---------------------------------------------------------*\
| RGBController_TrustGXT180.h                               |
|                                                           |
|   RGBController for Trust GXT 180                         |
|                                                           |
|   Morgan Guimard (morg)                       24 Mar 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "TrustGXT180Controller.h"

class RGBController_TrustGXT180 : public RGBController
{
public:
    RGBController_TrustGXT180(TrustGXT180Controller* controller_ptr);
    ~RGBController_TrustGXT180();

    void SetupZones();
    void DeviceResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    TrustGXT180Controller* controller;
};
