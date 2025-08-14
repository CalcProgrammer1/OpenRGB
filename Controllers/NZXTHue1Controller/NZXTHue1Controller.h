/*---------------------------------------------------------*\
| NZXTHue1Controller.h                                      |
|                                                           |
|   Driver for NZXT Hue 1 (Smart Device V1)                 |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      16 Apr 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <hidapi.h>
#include "RGBController.h"

enum
{
    HUE_1_ACCESSORY_STRIP       = 0x00, /* NZXT Hue+ LED Strip (10 LEDs)*/
    HUE_1_ACCESSORY_FAN         = 0x01  /* NZXT Aer RGB Fan (8 LEDs)    */
};

enum
{
    HUE_1_SPEED_SLOWEST         = 0x00, /* Slowest speed                */
    HUE_1_SPEED_SLOW            = 0x01, /* Slow speed                   */
    HUE_1_SPEED_NORMAL          = 0x02, /* Normal speed                 */
    HUE_1_SPEED_FAST            = 0x03, /* Fast speed                   */
    HUE_1_SPEED_FASTEST         = 0x04, /* Fastest speed                */
};

enum
{
    HUE_1_MODE_FIXED            = 0x00, /* Fixed colors mode            */
    HUE_1_MODE_FADING           = 0x01, /* Fading mode                  */
    HUE_1_MODE_SPECTRUM         = 0x02, /* Spectrum cycle mode          */
    HUE_1_MODE_MARQUEE          = 0x03, /* Marquee mode                 */
    HUE_1_MODE_COVER_MARQUEE    = 0x04, /* Cover marquee mode           */
    HUE_1_MODE_ALTERNATING      = 0x05, /* Alternating mode             */
    HUE_1_MODE_PULSING          = 0x06, /* Pulsing mode                 */
    HUE_1_MODE_BREATHING        = 0x07, /* Breathing mode               */
    HUE_1_MODE_ALERT            = 0x08, /* Alert mode                   */
    HUE_1_MODE_CANDLELIGHT      = 0x09, /* Candlelight mode             */
    HUE_1_MODE_WINGS            = 0x0C, /* Wings mode                   */
    HUE_1_MODE_WAVE             = 0x0D, /* Wave mode                    */
};

class NZXTHue1Controller
{
public:
    NZXTHue1Controller(hid_device* dev_handle, unsigned int fan_channels, const char* path, std::string dev_name);
    ~NZXTHue1Controller();

    std::string     GetFirmwareVersion();
    std::string     GetLocation();
    std::string     GetName();
    std::string     GetSerialString();

    unsigned int    GetAccessoryType();

    void            SetEffect
                        (
                        unsigned char   mode,
                        unsigned char   speed,
                        bool            direction,
                        RGBColor *      colors,
                        unsigned int    num_colors
                        );

    void            SetLEDs
                        (
                        RGBColor *      colors,
                        unsigned int    num_colors
                        );

    unsigned int    num_leds;

private:
    hid_device*     dev;

    char            firmware_version[16];
    std::string     location;
    std::string     name;
    unsigned int    accessory_type;

    void            Initialize();

    void            SendPacket
                        (
                        unsigned char   mode,
                        bool            direction,
                        unsigned char   color_idx,
                        unsigned char   speed,
                        unsigned char   color_count,
                        unsigned char*  color_data
                        );
};
