/*---------------------------------------------------------*\
| RGBController_N5312A.h                                    |
|                                                           |
|   RGBController for N5312A                                |
|                                                           |
|   Morgan Guimard (morg)                       02 Apr 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "N5312AController.h"

class RGBController_N5312A : public RGBController
{
public:
    RGBController_N5312A(N5312AController* controller_ptr);
    ~RGBController_N5312A();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    N5312AController* controller;
};
