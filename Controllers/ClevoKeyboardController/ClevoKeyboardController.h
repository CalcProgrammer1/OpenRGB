/*---------------------------------------------------------*\
| ClevoKeyboardController.h                                 |
|                                                           |
|   Driver for Clevo per-key RGB keyboard                   |
|   Supports ITE 8291 (PID 0x600B) and ITE 829x (0x8910)    |
|                                                           |
|   Kyle Cascade (kyle@cascade.family)          16 Jan 2026 |
|   Valentin Lobstein (balgogan@protonmail.com) 27 Mar 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <hidapi.h>
#include "RGBController.h"

/*---------------------------------------------------------*\
| Device type enumeration                                   |
\*---------------------------------------------------------*/
enum clevo_keyboard_type
{
    CLEVO_KB_ITE8291,
    CLEVO_KB_ITE829X,
};

/*---------------------------------------------------------*\
| ITE 8291 keyboard defines (PID 0x600B)                    |
| 8 bytes, no report ID, row-based color data               |
\*---------------------------------------------------------*/
#define CLEVO_8291_REPORT_SIZE         8
#define CLEVO_8291_ROW_DATA_SIZE       65
#define CLEVO_8291_NUM_ROWS            6
#define CLEVO_8291_NUM_COLS            21
#define CLEVO_8291_NUM_LEDS            (CLEVO_8291_NUM_ROWS * CLEVO_8291_NUM_COLS)
#define CLEVO_8291_BRIGHTNESS_MAX      0x32
#define CLEVO_8291_CMD_MODE            0x08
#define CLEVO_8291_CMD_COLOR           0x14
#define CLEVO_8291_CMD_ROW             0x16
#define CLEVO_8291_POWER_OFF           0x01
#define CLEVO_8291_POWER_ON            0x02
#define CLEVO_8291_MODE_FLAG           0x08

/*---------------------------------------------------------*\
| ITE 829x keyboard defines (PID 0x8910)                    |
| 6 bytes with report ID 0xCC, per-key color commands       |
| Protocol: [0xCC, cmd, d0, d1, d2, d3]                     |
| LED ID: ((row & 0x07) << 5) | (col & 0x1F)                |
\*---------------------------------------------------------*/
#define CLEVO_829X_REPORT_ID           0xCC
#define CLEVO_829X_REPORT_SIZE         6
#define CLEVO_829X_NUM_ROWS            6
#define CLEVO_829X_NUM_COLS            20
#define CLEVO_829X_NUM_LEDS            (CLEVO_829X_NUM_ROWS * CLEVO_829X_NUM_COLS)
#define CLEVO_829X_BRIGHTNESS_MAX      0x0A
#define CLEVO_829X_LED_ID(row, col)    (unsigned char)((((row) & 0x07) << 5) | ((col) & 0x1F))

#define CLEVO_829X_CMD_SET_LED         0x01
#define CLEVO_829X_CMD_BRIGHTNESS      0x09
#define CLEVO_829X_CMD_ANIM            0x00
#define CLEVO_829X_CMD_CLEAR           0x0C
#define CLEVO_829X_CMD_WAVE_COLOR      0x15
#define CLEVO_829X_CMD_SNAKE_COLOR     0x16
#define CLEVO_829X_CMD_SCAN_COLOR      0x17
#define CLEVO_829X_CMD_RANDOM_COLOR    0x18
#define CLEVO_829X_COLOR_CUSTOM        0xAA
#define CLEVO_829X_COLOR_SLOT_BASE     0xA1
#define CLEVO_829X_PRESET_SLOT_BASE    0x71
#define CLEVO_829X_CMD_SAVE_BIOS       0x20
#define CLEVO_829X_CMD_STOP_ANIM       0x03

/*---------------------------------------------------------*\
| Shared defines                                            |
\*---------------------------------------------------------*/
#define CLEVO_KEYBOARD_BRIGHTNESS_MIN  0x00
#define CLEVO_KEYBOARD_SPEED_MIN       0x00
#define CLEVO_KEYBOARD_SPEED_MAX       0x0A
#define CLEVO_KEYBOARD_SPEED_DEFAULT   0x00

/*---------------------------------------------------------*\
| ITE 8291 modes                                            |
\*---------------------------------------------------------*/
enum
{
    CLEVO_KEYBOARD_MODE_DIRECT     = 0x33,
    CLEVO_KEYBOARD_MODE_BREATH     = 0x02,
    CLEVO_KEYBOARD_MODE_WAVE       = 0x03,
    CLEVO_KEYBOARD_MODE_REACTIVE   = 0x04,
    CLEVO_KEYBOARD_MODE_RAINBOW    = 0x05,
    CLEVO_KEYBOARD_MODE_RIPPLE     = 0x06,
    CLEVO_KEYBOARD_MODE_MARQUEE    = 0x09,
    CLEVO_KEYBOARD_MODE_RAINDROP   = 0x0A,
    CLEVO_KEYBOARD_MODE_AURORA     = 0x0E,
    CLEVO_KEYBOARD_MODE_SPARK      = 0x11,
};

/*---------------------------------------------------------*\
| ITE 829x firmware modes                                   |
| Two command styles:                                       |
|   cmd=0x00, d0=mode_id  (animation select)                |
|   cmd=mode_id, d0=0x00  (standalone effects)              |
\*---------------------------------------------------------*/
enum
{
    CLEVO_829X_MODE_DIRECT          = 0xFD,
    CLEVO_829X_MODE_WAVE            = 0xA0,
    CLEVO_829X_MODE_WAVE_COLOR      = 0xB0,
    CLEVO_829X_MODE_BREATH          = 0xA1,
    CLEVO_829X_MODE_BREATH_COLOR    = 0xB1,
    CLEVO_829X_MODE_BLINK           = 0xA2,
    CLEVO_829X_MODE_BLINK_COLOR     = 0xB2,
    CLEVO_829X_MODE_RANDOM          = 0xA3,
    CLEVO_829X_MODE_RANDOM_COLOR    = 0xA9,
    CLEVO_829X_MODE_SCAN            = 0xA4,
    /* Ripple (0x07) requires Insyde BIOS EC config, not supported */
    CLEVO_829X_MODE_SNAKE           = 0xA6,
    CLEVO_829X_MODE_SNAKE_COLOR     = 0xB6,
    CLEVO_829X_MODE_RAINBOW_CYCLE   = 0xA7,
    CLEVO_829X_MODE_RAINBOW_GRAD    = 0xA8,
    CLEVO_829X_MODE_FN_COLOR        = 0xAF,
    CLEVO_829X_MODE_OFF             = 0xFF,
};

/*---------------------------------------------------------*\
| Wave/reactive behaviour                                   |
\*---------------------------------------------------------*/
enum
{
    CLEVO_KEYBOARD_DIRECTION_LEFT      = 0x01,
    CLEVO_KEYBOARD_DIRECTION_RIGHT     = 0x02,
    CLEVO_KEYBOARD_DIRECTION_UP        = 0x03,
    CLEVO_KEYBOARD_DIRECTION_DOWN      = 0x04,
};

enum
{
    CLEVO_KEYBOARD_REACTIVE_KEYPRESS   = 0x00,
    CLEVO_KEYBOARD_REACTIVE_AUTO       = 0x01,
};

class ClevoKeyboardController
{
public:
    ClevoKeyboardController(hid_device* dev_handle, const hid_device_info& info, clevo_keyboard_type kb_type);
    ~ClevoKeyboardController();

    clevo_keyboard_type GetType();
    int             GetNumRows();
    int             GetNumCols();
    int             GetNumLEDs();
    unsigned char   GetBrightnessMax();

    std::string     GetDeviceLocation();
    std::string     GetSerialString();
    std::string     GetFirmwareVersion();

    void            TurnOff();
    void            SetMode(unsigned char mode, unsigned char brightness, unsigned char speed, unsigned char behaviour);
    void            SetModeColor(unsigned char color_idx, RGBColor color);
    void            SendColors(const unsigned char* color_data, unsigned char brightness, unsigned char speed);
    void            SetEffect829x(unsigned char mode_value, const std::vector<RGBColor>& colors, unsigned int direction = 0, unsigned int color_mode = 0);
    void            SetBrightnessSpeed829x(unsigned char brightness, unsigned char speed);
    void            SaveToBios829x();
    void            ClearBiosSave829x();
    void            StopAnimation829x();

private:
    hid_device*             dev;
    clevo_keyboard_type     type;
    std::string             location;
    unsigned short          version;
    unsigned char           current_mode_829x = 0xFF;
    unsigned char           current_brightness_829x = 0xFF;
    unsigned char           current_speed_829x = 0xFF;

    void            WriteControl8291(unsigned char* data);
    void            WriteRowData(unsigned char* data);
    void            SendCommand829x(unsigned char cmd, unsigned char d0 = 0, unsigned char d1 = 0, unsigned char d2 = 0, unsigned char d3 = 0);
    void            SendColorSlot829x(unsigned char cmd, unsigned char slot, RGBColor color);
};
