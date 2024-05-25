/*---------------------------------------------------------*\
| RGBController_Ionico.h                                    |
|                                                           |
|   RGBController for Ionico-II-17                          |
|                                                           |
|   Lucas Strafe                                31 Dec 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "IonicoController.h"

#define IONICO_KB_PID   0xCE00
#define IONICO_FB_PID   0x6005

class RGBController_Ionico : public RGBController
{
public:
    RGBController_Ionico(IonicoController* controller_ptr);
    ~RGBController_Ionico();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void SetSingleLED();
    void UpdateSingleLED(int led);
    void DeviceSaveMode();

    void DeviceUpdateMode();


private:
    IonicoController* controller;
};
