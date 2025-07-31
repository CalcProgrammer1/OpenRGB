/*---------------------------------------------------------*\
| RGBController_OKSKeyboard.h                               |
|                                                           |
|   RGBController for OKS keyboard                          |
|                                                           |
|   Merafour (OKS)                              24 Feb 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "OKSKeyboardController.h"

/*-----------------------------------------------------*\
| mode                                                  |
\*-----------------------------------------------------*/
enum user_param_view_mode_t
{
    UP_RGB_MODES_MASK  = 0x7F,
    UP_RGB_PAUSE__MASK = 0x80,
    UP_RGB_PAUSE_ON    = 0x80,
    UP_RGB_PAUSE_OFF   = 0x00,
    /*------------------------------------------------------------*\
    | direct mode,eg:OpenRGB+Artemis                               |
    \*------------------------------------------------------------*/
    UP_RGB_MODES_DIRECT = 0x00,
    /*------------------------------------------------------------*\
    | user define mode                                             |
    \*------------------------------------------------------------*/
    UP_RGB_MODES_UDEF1  = 0x01,
    UP_RGB_MODES_UDEF2  = 0x02,
    UP_RGB_MODES_UDEF3  = 0x03,
    UP_RGB_MODES_UDEF4  = 0x04,
    UP_RGB_MODES_UDEF5  = 0x05,
    UP_RGB_MODES_RECORD = 0x06,
    /*------------------------------------------------------------*\
    | default mode                                                 |
    \*------------------------------------------------------------*/
    UP_RGB_MODES_RACE       = 0x07,
    UP_RGB_MODES_BREAT      = 0x08,
    UP_RGB_MODES_RIPPLE     = 0x09,
    UP_RGB_MODES_DIFF       = 0x0A,
    UP_RGB_MODES_WAVE       = 0x0B,
    UP_RGB_MODES_CLICK      = 0x0C,
    UP_RGB_MODES_BRIGHT     = 0x0D,
    UP_RGB_MODES_LUMA_CYCLE = 0x0E,
    UP_RGB_MODES_ROTATE     = 0x0F,
    UP_RGB_MODES_HID_MENU   = 0x10,
    UP_RGB_MODES_NONE       = 0x12,
};

class RGBController_OKSKeyboard : public RGBController
{
public:
    RGBController_OKSKeyboard(OKSKeyboardController* controller_ptr);
    ~RGBController_OKSKeyboard();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    OKSKeyboardController* controller;
};
