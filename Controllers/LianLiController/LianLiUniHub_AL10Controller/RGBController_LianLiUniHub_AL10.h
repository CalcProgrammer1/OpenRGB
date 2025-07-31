/*---------------------------------------------------------*\
| RGBController_LianLiUniHub_AL10.h                         |
|                                                           |
|   RGBController for Lian Li AL Uni Hub                    |
|                                                           |
|   Oliver P                                    05 May 2022 |
|   Credit to Luca Lovisa for original work                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <cstdint>
#include <vector>
#include "LianLiUniHub_AL10Controller.h"
#include "RGBController.h"

class RGBController_LianLiUniHub_AL10 : public RGBController
{
public:
    RGBController_LianLiUniHub_AL10(LianLiUniHub_AL10Controller* controller_ptr);

    void SetupZones();

    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    uint8_t convertAnyFanCount(uint8_t count);
    uint8_t convertLedSpeed(uint8_t speed);
    uint8_t convertLedDirection(uint8_t direction);
    uint8_t convertLedBrightness(uint8_t brightness);

    uint8_t convertLedCountToFanCount(uint8_t count);

private:
    LianLiUniHub_AL10Controller* controller;
    bool                    initializedMode;
};
