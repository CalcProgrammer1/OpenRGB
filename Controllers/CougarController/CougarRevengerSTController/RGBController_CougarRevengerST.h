/*---------------------------------------------------------*\
| RGBController_CougarRevengerST.h                          |
|                                                           |
|   RGBController for Cougar Revenger ST                    |
|                                                           |
|   Morgan Guimard (morg)                       17 Mar 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    CougarRevengerSTController* controller;
};
