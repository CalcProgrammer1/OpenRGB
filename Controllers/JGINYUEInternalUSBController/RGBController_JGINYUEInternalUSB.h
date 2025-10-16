/*---------------------------------------------------------*\
| RGBController_JGINYUEInternalUSB.h                        |
|                                                           |
|   RGBController for JGINYUE USB motherboard               |
|                                                           |
|   Tong R (tcr020)                             09 Aug 2023 |
|   Liu ShiMeng(Moon dream stars)               09 Aug 2023 |
|   Dongguan Yonghang Electronic Technology Co., Ltd        |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "JGINYUEInternalUSBController.h"

class RGBController_JGINYUEInternalUSB : public RGBController
{
public:
    RGBController_JGINYUEInternalUSB(JGINYUEInternalUSBController* controller_ptr);

    void        SetupZones();

    void        DeviceResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    JGINYUEInternalUSBController*   controller;
};
