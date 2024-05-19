/*---------------------------------------------------------*\
| RGBController_GigabyteAorusPCCase.h                       |
|                                                           |
|   RGBController for Gigabyte Aorus case                   |
|                                                           |
|   Denis Nazarov (nenderus)                    10 Feb 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GigabyteAorusPCCaseController.h"

class RGBController_GigabyteAorusPCCase : public RGBController
{
public:
    RGBController_GigabyteAorusPCCase(GigabyteAorusPCCaseController* controller_ptr);
    ~RGBController_GigabyteAorusPCCase();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    GigabyteAorusPCCaseController* controller;
};
