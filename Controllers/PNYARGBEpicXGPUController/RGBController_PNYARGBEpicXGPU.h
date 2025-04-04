/*---------------------------------------------------------*\
| RGBController_PNYARGBEpicXGPU.h                           |
|                                                           |
|   RGBController for PNY ARGB Epic-X GPU                   |
|                                                           |
|   Peter Berendi                               27 Apr 2025 |
|   Adam Honse <calcprogrammer1@gmail.com>      01 Aug 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "RGBController.h"
#include "PNYARGBEpicXGPUController.h"

class RGBController_PNYARGBEpicXGPU : public RGBController
{
public:
    RGBController_PNYARGBEpicXGPU(PNYARGBEpicXGPUController* controller_ptr);
    ~RGBController_PNYARGBEpicXGPU();

    void SetupZones();

    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

protected:
    PNYARGBEpicXGPUController*  controller;
    std::vector<unsigned char>  zone_led_idx;
};
