/*---------------------------------------------------------*\
| RGBController_WitmodKeyboard.h                             |
|                                                           |
|   RGBController for Witmod keyboards                      |
|                                                           |
|   vlack                                       03 May 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RGBControllerKeyNames.h"
#include "KeyboardLayoutManager.h"
#include "WitmodKeyboardController.h"

/*---------------------------------------------------------*\
| Both sizes share the same six row hardware matrix.  The   |
| GK8120 adds the number pad in the four columns to the     |
| right of the GK8110 keys.                                 |
\*---------------------------------------------------------*/
#define GK8110_WIDTH            17
#define GK8120_WIDTH            21
#define WITMOD_MATRIX_HEIGHT    6

class RGBController_WitmodKeyboard : public RGBController
{
public:
    RGBController_WitmodKeyboard(WitmodKeyboardController* controller_ptr);
    ~RGBController_WitmodKeyboard();

    void                SetupZones();

    void                DeviceUpdateLEDs();
    void                DeviceUpdateZoneLEDs(int zone);
    void                DeviceUpdateSingleLED(int led);

    void                DeviceUpdateMode();

private:
    WitmodKeyboardController* controller;
};
