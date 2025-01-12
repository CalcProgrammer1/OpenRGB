/*---------------------------------------------------------*\
| AOCHeadsetController.h                                    |
|                                                           |
|   Driver for AOC headset                                  |
|                                                           |
|   Adam Honse (CalcProgrammer1)                12 Jan 2025 |
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
    AOC_HEADSET_MODE_STATIC_SINGLE_COLOR       = 0x00,     /* Static mode                  */
    AOC_HEADSET_MODE_BLINKING                  = 0x01,     /* Blinking mode                */
    AOC_HEADSET_MODE_BREATHING                 = 0x02,     /* Breathing mode               */
    AOC_HEADSET_MODE_MUSIC                     = 0x03,     /* Music mode                   */
};

enum
{
    AOC_HEADSET_SPEED_SLOW                     = 0x03,     /* Slowest speed                */
    AOC_HEADSET_SPEED_MEDIUM                   = 0x02,     /* Medium speed                 */
    AOC_HEADSET_SPEED_FAST                     = 0x01,     /* Fastest speed                */
};

class AOCHeadsetController
{
public:
    AOCHeadsetController(hid_device* dev_handle, const char* path);
    ~AOCHeadsetController();

    std::string GetDeviceLocation();
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

    void SendStartPacket();

    void SendModePacket(unsigned char mode);
    void SendBeginPacket();

    void SendColorPacket(RGBColor* color_data);

    void SendApplyPacket();
};
