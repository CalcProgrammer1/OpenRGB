/*---------------------------------------------------------*\
| RGBController_CorsairK95PlatinumXT.h                      |
|                                                           |
|   RGBController for Corsair K95 Platinum XT keyboard      |
|                                                           |
|   Morgan Guimard (morg)                       07 Jun 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CorsairK95PlatinumXTController.h"

class RGBController_CorsairK95PlatinumXT : public RGBController
{
public:
    RGBController_CorsairK95PlatinumXT(CorsairK95PlatinumXTController* controller_ptr);
    ~RGBController_CorsairK95PlatinumXT();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    CorsairK95PlatinumXTController*                              controller;
};
