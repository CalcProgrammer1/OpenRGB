/*---------------------------------------------------------*\
| RGBController_CougarRevengerST.h                          |
|                                                           |
|   RGBController for Cougar Revenger ST                    |
|                                                           |
|   Morgan Guimard (morg)                       17 Mar 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CougarRevengerSTController.h"

class RGBController_CougarRevengerST : public RGBController
{
public:
    RGBController_CougarRevengerST(CougarRevengerSTController* controller_ptr);
    ~RGBController_CougarRevengerST();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    CougarRevengerSTController* controller;
};
