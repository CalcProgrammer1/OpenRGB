/*---------------------------------------------------------*\
| RGBController_DRGB.h                                      |
|                                                           |
|   Driver for DRGBmods                                     |
|                                                           |
|   Zhi Yan                                     25 Jun 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "DRGBController.h"

#define DRGBV4_VID              0x2486
#define DRGB_LED_V4_PID         0x3608
#define DRGB_ULTRA_V4F_PID      0x3616
#define DRGB_CORE_V4F_PID       0x3628
#define DRGB_SIG_V4F_PID        0x3636

#define DRGB_LED_V5_PID         0x3208
#define DRGB_ULTRA_V5_PID       0x3215
#define DRGB_ULTRA_V5F_PID      0x3217
#define DRGB_CORE_V5_PID        0x3228
#define DRGB_CORE_V5F_PID       0x3229
#define DRGB_SIG_V5F_PID        0x3232

#define DRGBV3_VID              0x2023
#define DRGB_LED_V3_PID         0x1209
#define DRGB_Ultra_V3_PID       0x1221
#define DRGB_CORE_V3_PID        0x1226
#define DRGB_ELITE_PID          0x1408
#define DM_10_PID               0x1410
#define JPU_12_PID              0x1412

#define DRGBV2_VID              0x2023
#define DRGB_LED_PID            0x1208
#define DRGB_ULTRA_PID          0x1220
#define DRGB_SIG_AB_PID         0x1210
#define DRGB_SIG_CD_PID         0x1211
#define DRGB_Strimer_PID        0x1215

#define YICO_VID                0x1368
#define YICO_8_PID              0x6077
#define YICO_08_PID             0x6078
#define YICO_08_1_PID           0x6079

class RGBController_DRGB : public RGBController
{
public:
    RGBController_DRGB(DRGBController* controller_ptr);
    ~RGBController_DRGB();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);
    void        DeviceUpdateMode();

private:
    DRGBController*     controller;
    std::vector<unsigned int>   leds_channel;
    std::vector<unsigned int>   zones_channel;
    unsigned int Version   = 4;
};
