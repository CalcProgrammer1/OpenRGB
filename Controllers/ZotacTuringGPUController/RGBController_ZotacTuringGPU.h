/*---------------------------------------------------------*\
| RGBController_ZotacTuringGPU.h                            |
|                                                           |
|   RGBController for Zotac Turing GPU                      |
|                                                           |
|   David Henry                                 07 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ZotacTuringGPUController.h"

class RGBController_ZotacTuringGPU : public RGBController
{
public:
    RGBController_ZotacTuringGPU(ZotacTuringGPUController* controller_ptr);
    ~RGBController_ZotacTuringGPU();

    void SetupInitialValues();
    void SetupZones();

    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    ZotacTuringGPUController* controller;
};
