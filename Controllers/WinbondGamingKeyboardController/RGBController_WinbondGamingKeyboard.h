/*---------------------------------------------------------*\
| RGBController_WinbondGamingKeyboard.h                     |
|                                                           |
|   RGBController for Winbond Gaming Keyboard               |
|                                                           |
|   Daniel Gibson                               03 Dec 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "WinbondGamingKeyboardController.h"

enum
{
    WINBOND_GK_MODE_STATIC    = 0,
    WINBOND_GK_MODE_BREATHE   = 1,
    WINBOND_GK_MODE_WAVE      = 2,
    WINBOND_GK_MODE_NEON      = 3,
    WINBOND_GK_MODE_LOGO_NEON = 4, // logo-only!
    WINBOND_GK_MODE_SNAKE     = 5,
    /*-------------------------------------------------------------------------*\
    | the following modes are ones that only show effects when a key is pressed |
    | and otherwise only a background color                                     |
    \*-------------------------------------------------------------------------*/
    WINBOND_GK_MODE_REACTIVE  = 6,
    WINBOND_GK_MODE_AURORA    = 7,
    WINBOND_GK_MODE_RIPPLE    = 8,
    WINBOND_GK_MODE_CUSTOM    = 10
};

class RGBController_WinbondGamingKeyboard : public RGBController
{
public:
    RGBController_WinbondGamingKeyboard(WinbondGamingKeyboardController* ctrl);
    ~RGBController_WinbondGamingKeyboard();

    void                SetupZones();

    void                DeviceUpdateLEDs();
    void                DeviceUpdateZoneLEDs(int zone);
    void                DeviceUpdateSingleLED(int led);

    void                DeviceUpdateMode();

private:
    WinbondGamingKeyboardController* controller;
};
