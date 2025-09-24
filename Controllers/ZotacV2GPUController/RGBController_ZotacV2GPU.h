/*---------------------------------------------------------*\
| RGBController_ZotacV2GPU.h                                |
|                                                           |
|   RGBController for Zotac V2 GPU                          |
|                                                           |
|   Krzysztof Haładyn (krzys_h)                 16 Mar 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ZotacV2GPUController.h"

class RGBController_ZotacV2GPU : public RGBController
{
public:
    RGBController_ZotacV2GPU(ZotacV2GPUController* controller_ptr);
    ~RGBController_ZotacV2GPU();

    void SetupInitialValues();
    void SetupZones();

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

    ZotacV2GPUConfig config;

private:
    ZotacV2GPUController* controller;

    int FindSynchronizedZoneNum(int syncMode);
};
