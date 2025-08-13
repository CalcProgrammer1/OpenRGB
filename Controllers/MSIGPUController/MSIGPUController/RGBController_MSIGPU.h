/*---------------------------------------------------------*\
| RGBController_MSIGPU.h                                    |
|                                                           |
|   RGBController for MSI GPU                               |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "MSIGPUController.h"

class RGBController_MSIGPU : public RGBController
{
public:
    RGBController_MSIGPU(MSIGPUController* controller_ptr);
    ~RGBController_MSIGPU();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    MSIGPUController* controller;
    std::chrono::time_point<std::chrono::steady_clock> last_commit_time;

    bool       TimeToSend();
    int        GetDeviceMode();
    int        GetModeSpeed();
};
