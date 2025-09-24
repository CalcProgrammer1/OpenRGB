/*---------------------------------------------------------*\
| RGBController_ZotacTuringGPU.h                            |
|                                                           |
|   RGBController for Zotac Turing GPU                      |
|                                                           |
|   David Henry                                 07 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    ZotacTuringGPUController* controller;
};
