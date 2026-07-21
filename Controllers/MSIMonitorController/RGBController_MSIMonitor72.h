/*---------------------------------------------------------*\
| RGBController_MSIMonitor72.h                              |
|                                                           |
|   RGBController for MSI monitors using the 0x72 dual      |
|   control block layout (MPG 322URX QD-OLED)               |
|                                                           |
|   Ken Sanislo                               19 Jul 2026   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "MSIMonitorController.h"

class RGBController_MSIMonitor72 : public RGBController
{
public:
    RGBController_MSIMonitor72(MSIMonitorController* controller_ptr, unsigned int count, unsigned int direction, bool dual);
    ~RGBController_MSIMonitor72();

    void SetupZones();

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();
    void DeviceSaveMode();

private:
    void ApplyMode(bool save);

    MSIMonitorController*                               controller;
    unsigned int                                        led_count;
    bool                                                reverse_order;
    bool                                                dual_block;
};
