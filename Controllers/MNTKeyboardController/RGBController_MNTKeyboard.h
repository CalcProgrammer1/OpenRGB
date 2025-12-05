/*---------------------------------------------------------*\
| RGBController_MNTKeyboard.h                               |
|                                                           |
|   Driver for the MNT Reform keyboards                     |
|                                                           |
|   Christian Heller <c.heller@plomlompom.de>  7 Aug 2024   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RGBControllerKeyNames.h"
#include "MNTKeyboardController.h"

#define NA 0xFFFFFFFF

class RGBController_MNTKeyboard : public RGBController
{
  public:
    ~RGBController_MNTKeyboard();
    void SetupZones();
    void DeviceUpdateLEDs();

    void ResizeZone(int, int);
    void UpdateZoneLEDs(int);
    void UpdateSingleLED(int);
    void DeviceUpdateMode();

  protected:
    const char **led_names;
    unsigned int *matrix_keys;
    void CommonInit();
    MNTKeyboardController *controller;
};
