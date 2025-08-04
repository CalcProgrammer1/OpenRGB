/*---------------------------------------------------------*\
| AOCMouseController.h                                      |
|                                                           |
|   Driver for AOC mouse                                    |
|                                                           |
|   Adam Honse (CalcProgrammer1)                20 Apr 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

/*-----------------------------------------*\
| AOC Mousemat Modes                        |
|   Note: The 0x80 bit is the random flag   |
\*-----------------------------------------*/
enum
{
    AOC_MOUSE_MODE_STATIC_SINGLE_COLOR          = 0x00,     /* Static single color mode     */
    AOC_MOUSE_MODE_STATIC_RANDOM                = 0x80,     /* Static random color mode     */
    AOC_MOUSE_MODE_BREATHING_SINGLE_COLOR       = 0x01,     /* Breathing single color mode  */
    AOC_MOUSE_MODE_BREATHING_RANDOM             = 0x81,     /* Breathing random color mode  */
    AOC_MOUSE_MODE_BLINK_SINGLE_COLOR           = 0x02,     /* Blink single color mode      */
    AOC_MOUSE_MODE_BLINK_RANDOM                 = 0x82,     /* Blink random color mode      */
    AOC_MOUSE_MODE_WAVE_SINGLE_COLOR            = 0x03,     /* Wave single color mode       */
    AOC_MOUSE_MODE_WAVE_RANDOM                  = 0x83,     /* Wave random color mode       */
    AOC_MOUSE_MODE_DPI                          = 0x04,     /* DPI mode                     */
};

enum
{
    AOC_MOUSE_SPEED_SLOW                        = 0x03,     /* Slowest speed                */
    AOC_MOUSE_SPEED_MEDIUM                      = 0x02,     /* Medium speed                 */
    AOC_MOUSE_SPEED_FAST                        = 0x01,     /* Fastest speed                */
};

enum
{
    AOC_MOUSE_BRIGHTNESS_OFF                    = 0x00,     /* Lowest brightness (off)      */
    AOC_MOUSE_BRIGHTNESS_LOW                    = 0x01,     /* Low brightness               */
    AOC_MOUSE_BRIGHTNESS_MEDIUM                 = 0x02,     /* Medium brightness            */
    AOC_MOUSE_BRIGHTNESS_HIGH                   = 0x03,     /* Highest brightness           */
};

enum
{
    AOC_MOUSE_DIRECTION_CLOCKWISE               = 0x00,     /* Clockwise direction          */
    AOC_MOUSE_DIRECTION_COUNTERCLOCKWISE        = 0x01,     /* Counter-clockwise direction  */
};

class AOCMouseController
{
public:
    AOCMouseController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~AOCMouseController();

    std::string GetDeviceLocation();
    std::string GetDeviceName();
    std::string GetSerialString();

    void SendDirect
            (
            RGBColor*       color_data
            );

    void SendPacket
            (
            unsigned char   mode,
            unsigned char   brightness,
            unsigned char   speed,
            unsigned char   direction,
            RGBColor*       color_data
            );

private:
    hid_device*             dev;
    std::string             location;
    std::string             name;
};
