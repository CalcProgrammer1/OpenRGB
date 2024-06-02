/*---------------------------------------------------------*\
| NZXTHuePlusController.h                                   |
|                                                           |
|   Driver for NZXT Hue Plus                                |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      27 Aug 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "RGBController.h"
#include "serial_port.h"

#ifndef TRUE
#define TRUE true
#define FALSE false
#endif

#ifndef WIN32
#define LPSTR           char *
#define strtok_s        strtok_r
#endif

#define HUE_PLUS_BAUD           256000
#define HUE_PLUS_PACKET_SIZE    125

enum
{
    HUE_PLUS_CHANNEL_BOTH   = 0x00,     /* Both channels                */
    HUE_PLUS_CHANNEL_1      = 0x01,     /* Channel 1                    */
    HUE_PLUS_CHANNEL_2      = 0x02,     /* Channel 2                    */
    HUE_PLUS_NUM_CHANNELS   = 0x02      /* Number of channels           */
};

enum
{
    HUE_PLUS_CHANNEL_1_IDX  = 0x00,     /* Channel 1 array index        */
    HUE_PLUS_CHANNEL_2_IDX  = 0x01,     /* Channel 2 array index        */
};

enum
{
    HUE_PLUS_SPEED_SLOWEST      = 0x00, /* Slowest speed                */
    HUE_PLUS_SPEED_SLOW         = 0x01, /* Slow speed                   */
    HUE_PLUS_SPEED_NORMAL       = 0x02, /* Normal speed                 */
    HUE_PLUS_SPEED_FAST         = 0x03, /* Fast speed                   */
    HUE_PLUS_SPEED_FASTEST      = 0x04, /* Fastest speed                */
};

enum
{
    HUE_PLUS_MODE_FIXED         = 0x00, /* Fixed colors mode            */
    HUE_PLUS_MODE_FADING        = 0x01, /* Fading mode                  */
    HUE_PLUS_MODE_SPECTRUM      = 0x02, /* Spectrum cycle mode          */
    HUE_PLUS_MODE_MARQUEE       = 0x03, /* Marquee mode                 */
    HUE_PLUS_MODE_COVER_MARQUEE = 0x04, /* Cover marquee mode           */
    HUE_PLUS_MODE_ALTERNATING   = 0x05, /* Alternating mode             */
    HUE_PLUS_MODE_PULSING       = 0x06, /* Pulsing mode                 */
    HUE_PLUS_MODE_BREATHING     = 0x07, /* Breathing mode               */
    HUE_PLUS_MODE_ALERT         = 0x08, /* Alert mode                   */
    HUE_PLUS_MODE_CANDLELIGHT   = 0x09, /* Candlelight mode             */
    HUE_PLUS_MODE_WINGS         = 0x0C, /* Wings mode                   */
    HUE_PLUS_MODE_WAVE          = 0x0D, /* Wave mode                    */
    HUE_PLUS_MODE_DIRECT        = 0x0E, /* Direct mode                  */
};

class HuePlusController
{
public:
    HuePlusController();
    ~HuePlusController();

    void            Initialize(char* port);
    std::string     GetLocation();
    unsigned int    GetLEDsOnChannel(unsigned int channel);

    void            SetChannelEffect
                        (
                        unsigned char   channel,
                        unsigned char   mode,
                        unsigned char   speed,
                        bool            direction,
                        RGBColor *      colors,
                        unsigned int    num_colors
                        );

    void            SetChannelLEDs
                        (
                        unsigned char   channel,
                        RGBColor *      colors,
                        unsigned int    num_colors
                        );

    unsigned int    channel_leds[HUE_PLUS_NUM_CHANNELS];

private:
    std::string     port_name;
    serial_port     *serialport = nullptr;

    void            SendPacket
                        (
                        unsigned char   channel,
                        unsigned char   mode,
                        bool            direction,
                        unsigned char   color_idx,
                        unsigned char   speed,
                        unsigned char   color_count,
                        unsigned char*  color_data
                        );
};
