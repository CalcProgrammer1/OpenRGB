/*---------------------------------------------------------*\
| ClevoKeyboardController.h                                 |
|                                                           |
|   Driver for Clevo laptop per-key RGB keyboard (ITE 8291) |
|                                                           |
|   Kyle Cascade (kyle@cascade.family)          16 Jan 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>

/*-----------------------------------------------------*\
| ITE 8291 keyboard defines                             |
\*-----------------------------------------------------*/
#define CLEVO_KEYBOARD_REPORT_SIZE     8
#define CLEVO_KEYBOARD_ROW_DATA_SIZE   65

#define CLEVO_KEYBOARD_NUM_ROWS        6
#define CLEVO_KEYBOARD_NUM_COLS        21
#define CLEVO_KEYBOARD_NUM_LEDS        (CLEVO_KEYBOARD_NUM_ROWS * CLEVO_KEYBOARD_NUM_COLS)

#define CLEVO_KEYBOARD_BRIGHTNESS_MIN  0x00
#define CLEVO_KEYBOARD_BRIGHTNESS_MAX  0x32

#define CLEVO_KEYBOARD_SPEED_MIN       0x01
#define CLEVO_KEYBOARD_SPEED_MAX       0x0A

/*-----------------------------------------------------*\
| ITE 8291 modes                                        |
\*-----------------------------------------------------*/
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

/*-----------------------------------------------------*\
| Wave/reactive behaviour                               |
\*-----------------------------------------------------*/
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
    ClevoKeyboardController(hid_device* dev_handle, const hid_device_info& info);
    ~ClevoKeyboardController();

    std::string     GetDeviceLocation();
    std::string     GetSerialString();
    std::string     GetFirmwareVersion();

    void            TurnOff();
    void            SetMode(unsigned char mode, unsigned char brightness, unsigned char speed, unsigned char behaviour);
    void            SetModeColor(unsigned char color_idx, unsigned char red, unsigned char green, unsigned char blue);
    void            SendColors(unsigned char* color_data, unsigned char brightness);

private:
    hid_device*     dev;
    std::string     location;
    unsigned short  version;

    void            WriteControl(unsigned char* data);
    void            WriteRowData(unsigned char* data);
};
