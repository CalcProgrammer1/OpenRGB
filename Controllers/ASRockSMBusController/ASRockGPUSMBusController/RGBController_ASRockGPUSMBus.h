/*---------------------------------------------------------*\
| RGBController_ASRockGPUSMBus.h                            |
|                                                           |
|   RGBController for ASRock GPU RGB controllers            |
|                                                           |
|   Based on SignalRGB ASRock GPU.js plugin                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ASRockGPUSMBusController.h"

class RGBController_ASRockGPUSMBus : public RGBController
{
public:
    RGBController_ASRockGPUSMBus(ASRockGPUSMBusController* controller_ptr);
    ~RGBController_ASRockGPUSMBus();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs(std::vector<RGBColor>& update_colors);
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    ASRockGPUSMBusController* controller;
};
