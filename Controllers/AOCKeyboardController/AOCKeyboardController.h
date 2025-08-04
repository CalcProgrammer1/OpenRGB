/*---------------------------------------------------------*\
| AOCKeyboardController.h                                   |
|                                                           |
|   Driver for AOC keyboard                                 |
|                                                           |
|   Adam Honse (CalcProgrammer1)                10 May 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

/*-----------------------------------------*\
| AOC Keyboard Modes                        |
\*-----------------------------------------*/
enum
{
    AOC_KEYBOARD_MODE_STATIC                    = 0x00,     /* Static mode                  */
    AOC_KEYBOARD_MODE_BREATHING                 = 0x01,     /* Breathing mode               */
    AOC_KEYBOARD_MODE_REACT                     = 0x02,     /* React mode                   */
    AOC_KEYBOARD_MODE_RIPPLE                    = 0x04,     /* Ripple mode                  */
    AOC_KEYBOARD_MODE_RADAR                     = 0x05,     /* Radar mode                   */
    AOC_KEYBOARD_MODE_FIREWORKS                 = 0x06,     /* Fireworks mode               */
    AOC_KEYBOARD_MODE_BLINK                     = 0x07,     /* Blink mode                   */
    AOC_KEYBOARD_MODE_WAVE                      = 0x08,     /* Wave mode                    */
    AOC_KEYBOARD_MODE_CUSTOM                    = 0x09,     /* Custom mode                  */
    AOC_KEYBOARD_MODE_CONCENTRIC_CIRCLES        = 0x0A,     /* Concentric Circles mode      */
    AOC_KEYBOARD_MODE_W_WAVE                    = 0x0B,     /* W Wave mode                  */
};

enum
{
    AOC_KEYBOARD_SPEED_SLOW                     = 0x03,     /* Slowest speed                */
    AOC_KEYBOARD_SPEED_MEDIUM                   = 0x02,     /* Medium speed                 */
    AOC_KEYBOARD_SPEED_FAST                     = 0x01,     /* Fastest speed                */
};

enum
{
    AOC_KEYBOARD_BRIGHTNESS_OFF                 = 0x00,     /* Lowest brightness (off)      */
    AOC_KEYBOARD_BRIGHTNESS_LOW                 = 0x01,     /* Low brightness               */
    AOC_KEYBOARD_BRIGHTNESS_MEDIUM              = 0x02,     /* Medium brightness            */
    AOC_KEYBOARD_BRIGHTNESS_HIGH                = 0x03,     /* Highest brightness           */
};

enum
{
    AOC_KEYBOARD_SINGLE_COLOR                   = 0x00,     /* Single color mode            */
    AOC_KEYBOARD_RANDOM                         = 0x01,     /* Random color mode            */
};

enum
{
    AOC_KEYBOARD_DIRECTION_CLOCKWISE            = 0x00,     /* Clockwise direction          */
    AOC_KEYBOARD_DIRECTION_COUNTERCLOCKWISE     = 0x01,     /* Counter-clockwise direction  */
};


class AOCKeyboardController
{
public:
    AOCKeyboardController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~AOCKeyboardController();

    std::string GetDeviceLocation();
    std::string GetDeviceName();
    std::string GetSerialString();

    void SetLightingConfig
            (
            unsigned char   mode,
            unsigned char   random,
            unsigned char   brightness,
            unsigned char   speed,
            unsigned char   direction,
            RGBColor*       color_data
            );

    void SetCustom
            (
            RGBColor*       color_data
            );

private:
    hid_device*             dev;
    std::string             location;
    std::string             name;

    void SendStartPacket();
    void SendEndPacket();

    void SendCustomPacket
            (
            RGBColor*       color_data
            );

    void SendLightingConfigPacket
            (
            unsigned char   mode,
            unsigned char   random,
            unsigned char   brightness,
            unsigned char   speed,
            unsigned char   direction,
            RGBColor*       color_data
            );
};
