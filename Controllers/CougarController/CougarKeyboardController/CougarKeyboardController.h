/*---------------------------------------------------------*\
| CougarKeyboardController.h                                |
|                                                           |
|   Driver for Cougar keyboard                              |
|                                                           |
|   Chris M (DrNo)                              05 Apr 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define COUGARKEYBOARDCONTROLLER_WRITE_PACKET_SIZE  65      //Buffer requires a prepended ReportID hence + 1
#define HID_MAX_STR                                 255

#define COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MIN     0
#define COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MAX     100
#define COUGARKEYBOARDCONTROLLER_MATRIX_WIDTH       23

static const uint8_t direction_map[6] =
{
    4,  0,  6,  2, 11, 12    //Left, Right, Up, Down, Horizontal, Vertical
};

enum Cougar_Keyboard_Controller_Modes
{
    COUGARKEYBOARDCONTROLLER_MODE_OFF           = 0x0C,     //Turn off - All leds off
    COUGARKEYBOARDCONTROLLER_MODE_DIRECT        = 0x0B,     //Customize Mode
    COUGARKEYBOARDCONTROLLER_MODE_STATIC        = 0x00,     //Steady Mode
    COUGARKEYBOARDCONTROLLER_MODE_BREATHING     = 0x01,     //Breathing Mode - Fades between fully off and fully on.
    COUGARKEYBOARDCONTROLLER_MODE_CIRCLE        = 0x02,
    COUGARKEYBOARDCONTROLLER_MODE_REACTIVE      = 0x03,     //Click Mode
    COUGARKEYBOARDCONTROLLER_MODE_WAVE          = 0x04,
    COUGARKEYBOARDCONTROLLER_MODE_RIPPLE        = 0x05,
    COUGARKEYBOARDCONTROLLER_MODE_STAR          = 0x06,
    COUGARKEYBOARDCONTROLLER_MODE_SCAN          = 0x07,
    COUGARKEYBOARDCONTROLLER_MODE_RHYTHM        = 0x08,
    COUGARKEYBOARDCONTROLLER_MODE_RAIN          = 0x09,
    COUGARKEYBOARDCONTROLLER_MODE_SNAKE         = 0x0A,
};

enum Cougar_Keyboard_Controller_Byte_Map
{
    COUGARKEYBOARDCONTROLLER_REPORT_BYTE        = 1,
    COUGARKEYBOARDCONTROLLER_COMMAND_BYTE       = 2,
    COUGARKEYBOARDCONTROLLER_MODE_BYTE          = 3,
    COUGARKEYBOARDCONTROLLER_SPEED_BYTE         = 5,
    COUGARKEYBOARDCONTROLLER_BRIGHTNESS_BYTE    = 6,
    COUGARKEYBOARDCONTROLLER_RANDOM_BYTE        = 7,
    COUGARKEYBOARDCONTROLLER_DIRECTION_BYTE     = 8,
    COUGARKEYBOARDCONTROLLER_DATA_BYTE          = 9,
};

enum Cougar_Keyboard_Controller_Speeds
{
    COUGARKEYBOARDCONTROLLER_SPEED_SLOWEST      = 0x0A,     // Slowest speed
    COUGARKEYBOARDCONTROLLER_SPEED_NORMAL       = 0x05,     // Normal speed
    COUGARKEYBOARDCONTROLLER_SPEED_FASTEST      = 0x01,     // Fastest speed
};

class CougarKeyboardController
{
public:
    CougarKeyboardController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~CougarKeyboardController();

    std::string     GetDeviceName();
    std::string     GetSerial();
    std::string     GetLocation();

    void            SetMode(uint8_t mode, uint8_t speed, uint8_t brightness, uint8_t direction, std::vector<RGBColor> colours, bool random_colours);
    void            SetLedsDirect(std::vector<RGBColor> colours);
    void            Save(uint8_t flag);
    void            SendProfile(uint8_t profile, uint8_t light);
private:
    std::string     serial;
    std::string     location;
    std::string     name;
    hid_device*     dev;
};
