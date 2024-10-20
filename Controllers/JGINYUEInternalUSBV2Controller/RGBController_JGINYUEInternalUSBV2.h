/*---------------------------------------------------------*\
| RGBController_JGINYUEInternalUSBV2.h                      |
|                                                           |
|   RGBController for JGINYUE USB motherboard V2            |
|                                                           |
|   Tong R (tcr020)                            03 July 2023 |
|   Liu ShiMeng(Moon dream stars)               06 Aug 2024 |
|   Dongguan Yonghang Electronic Technology Co., Ltd        |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "JGINYUEInternalUSBV2Controller.h"

class RGBController_JGINYUEInternalUSBV2 : public RGBController
{
public:
    RGBController_JGINYUEInternalUSBV2(JGINYUEInternalUSBV2Controller* controller_ptr);

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceUpdateZoneMode(int zone);

private:
    JGINYUEInternalUSBV2Controller*   controller;
    void        InitZones();
};
