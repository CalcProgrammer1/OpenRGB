/*---------------------------------------------------------*\
| RGBController_RedSquareKeyroxTKLClassic.h                 |
|                                                           |
|   RGBController for Red Square Keyrox TKL Classic         |
|                                                           |
|   vlack                                       03 May 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RGBControllerKeyNames.h"
#include "KeyboardLayoutManager.h"
#include "RedSquareKeyroxTKLClassicController.h"

#define KEYROX_TKL_CLASSIC_WIDTH    17
#define KEYROX_TKL_CLASSIC_HEIGHT   6

class RGBController_RedSquareKeyroxTKLClassic : public RGBController
{
public:
    RGBController_RedSquareKeyroxTKLClassic(RedSquareKeyroxTKLClassicController* controller_ptr);
    ~RGBController_RedSquareKeyroxTKLClassic();

    void                SetupZones();
    void                ResizeZone(int zone, int new_size);

    void                DeviceUpdateLEDs();
    void                UpdateZoneLEDs(int zone);
    void                UpdateSingleLED(int led);

    void                DeviceUpdateMode();

private:
    RedSquareKeyroxTKLClassicController* controller;
};
