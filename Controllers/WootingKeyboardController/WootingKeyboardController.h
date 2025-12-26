/*---------------------------------------------------------*\
| WootingKeyboardController.h                               |
|                                                           |
|   Driver for Wooting keyboard                             |
|                                                           |
|   Chris M (Dr_No)                             09 Jul 2021 |
|   Diogo Trindade (diogotr7)                   25 Dec 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"
#include "LogManager.h"

#define WOOTING_COMMAND_SIZE            8
#define WOOTING_RAW_COLORS_REPORT       11
#define WOOTING_SINGLE_COLOR_COMMAND    30
#define WOOTING_SINGLE_RESET_COMMAND    31
#define WOOTING_RESET_ALL_COMMAND       32
#define WOOTING_COLOR_INIT_COMMAND      33


#define WOOTING_RGB_ROWS                6
#define WOOTING_RGB_COLUMNS             21

#define WOOTING_ONE_RGB_COLUMNS         17
#define WOOTING_TWO_RGB_COLUMNS         21
#define WOOTING_60_RGB_COLUMNS          14
#define WOOTING_3PAD_RGB_COLUMNS        7

/*---------------------------------------------------------*\
| Placeholder for compilation. Redefined by each subclass   |
\*---------------------------------------------------------*/
#define WOOTING_CONTROLLER_NAME         "[Wooting] "

enum WOOTING_DEVICE_TYPE
{
    WOOTING_KB_TKL      = 0,
    WOOTING_KB_FULL     = 1,
    WOOTING_KB_60PER    = 2,
    WOOTING_KB_3PAD     = 3,
    WOOTING_KB_80PER    = 4,
};

enum RGB_PARTS
{
    PART0,
    PART1,
    PART2,
    PART3,
    PART4
};

class WootingKeyboardController
{
public:
    WootingKeyboardController();
    virtual ~WootingKeyboardController();

    hid_device*         dev;
    std::string         name;
    std::string         vendor;
    std::string         description;
    std::string         location;
    WOOTING_DEVICE_TYPE wooting_type;

    std::string         GetName();
    std::string         GetVendor();
    std::string         GetDescription();
    std::string         GetLocation();
    std::string         GetSerial();
    WOOTING_DEVICE_TYPE GetWootingType();
    bool                wooting_usb_send_feature(uint8_t command, uint8_t param0,
                                                 uint8_t param1, uint8_t param2, uint8_t param3);

    virtual void        SendDirect(RGBColor* colors, uint8_t color_count)                           = 0;
    virtual void        SendInitialize();
};
