/*---------------------------------------------------------*\
| AsusTUFLaptopController_Linux.h                           |
|                                                           |
|   Driver for ASUS TUF laptop                              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"

#define ASUS_KBD_BACKLIGHT_BRIGHTNESS_MIN   0
#define ASUS_KBD_BACKLIGHT_BRIGHTNESS_MAX   3
#define ASUS_KBD_BACKLIGHT_BRIGHTNESS       3

#define ASUS_KBD_BACKLIGHT_SPEED_MIN        0
#define ASUS_KBD_BACKLIGHT_SPEED_MAX        2
#define ASUS_KBD_BACKLIGHT_SPEED            1

#define ASUS_KBD_BACKLIGHT_BASE_PATH        "/sys/devices/platform/asus-nb-wmi/leds/asus::kbd_backlight"
#define ASUS_KBD_BACKLIGHT_MODE_PATH        "/kbd_rgb_mode"
#define ASUS_KBD_BACKLIGHT_BRIGHTNESS_PATH  "/brightness"

class AsusTUFLaptopLinuxController
{
public:
    void    SendBrightness
            (
            unsigned char   brightness
            );

    void    SendUpdate
            (
            unsigned char   mode,
            unsigned char   speed,
            unsigned char   save,
            unsigned char   red,
            unsigned char   green,
            unsigned char   blue
            );
};
