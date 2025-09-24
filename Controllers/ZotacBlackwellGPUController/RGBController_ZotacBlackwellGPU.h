/*---------------------------------------------------------*\
| RGBController_ZotacBlackwellGPU.h                         |
|                                                           |
|   RGBController for ZOTAC Blackwell (RTX 50 series) GPU   |
|                                                           |
|   Eder Sánchez                                27 Mar 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ZotacBlackwellGPUController.h"

class RGBController_ZotacBlackwellGPU : public RGBController
{
public:
    RGBController_ZotacBlackwellGPU(ZotacBlackwellGPUController* controller_ptr);
    ~RGBController_ZotacBlackwellGPU();

    void SetupZones();

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    ZotacBlackwellGPUController* controller;
};
