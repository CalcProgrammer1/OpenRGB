/*---------------------------------------------------------*\
| RoccatKoneAimoController.h                                |
|                                                           |
|   Driver for Roccat Kone Aimo                             |
|                                                           |
|   Thibaud M (enlight3d)                       17 Nov 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <array>
#include <map>
#include <string>
#include <vector>
#include <hidapi.h>
#include "RGBController.h"

#define HID_MAX_STR         255
#define NUM_LEDS            11

#define R_OFFSET            0
#define G_OFFSET            1
#define B_OFFSET            2

#define USB_COLOR_BUFF_LEN  46

enum ROCCAT_KONE_AIMO_CHANNEL
{
    SCROLL_WHEEL          = 0,
    STRIP_LEFT            = 1,
    STRIP_RIGHT           = 5,
    LOWER_LEFT            = 9,
    LOWER_RIGHT           = 10
};

class RoccatKoneAimoController
{
public:
    RoccatKoneAimoController(hid_device* dev_handle, char *_path, std::string dev_name);
    ~RoccatKoneAimoController();

    std::string     GetLocation();
    std::string     GetName();
    std::string     GetSerial();

    void            SetChannelColors(ROCCAT_KONE_AIMO_CHANNEL channel, RGBColor * colors, unsigned int num_colors);
    void            SendUpdate();

private:
    std::string     location;
    std::string     name;
    hid_device*     dev;
    unsigned char   usb_colors_buf[USB_COLOR_BUFF_LEN]; // USB buffer to be sent everytime we update mouse's LEDs

    void            SendInit();
};
