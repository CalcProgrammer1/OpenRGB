/*---------------------------------------------------------*\
| RGBController_MountainKeyboard.h                          |
|                                                           |
|   RGBController for Mountain keyboard                     |
|                                                           |
|   O'D.Sæzl                                       Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <cstring>
#include "RGBController.h"
#include "Mountain60KeyboardController.h"

#define MOUNTAIN60_KEYBOARD_BRIGHTNESS_MIN           0
#define MOUNTAIN60_KEYBOARD_BRIGHTNESS_MAX           4

#define MOUNTAIN60_KEYBOARD_SPEED_MIN                0
#define MOUNTAIN60_KEYBOARD_SPEED_MAX                4
#define MOUNTAIN60_KEYBOARD_SPEED_DEFAULT            2
#define MOUNTAIN60_KEEP_LIVE_PERIOD              500ms


class RGBController_Mountain60Keyboard : public RGBController
{
public:
    RGBController_Mountain60Keyboard(Mountain60KeyboardController* controller_ptr);
    ~RGBController_Mountain60Keyboard();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();
    void        UpdateMountain();

private:
    Mountain60KeyboardController*           controller;
    int                                     current_mode_value;
    std::thread*                            mountain_thread;
    std::atomic<bool>                       mountain_thread_running;
};
