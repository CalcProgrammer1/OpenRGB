/*---------------------------------------------------------*\
| CryorigH7QuadLumiController.h                             |
|                                                           |
|   Driver for Cryorig H7 Quad Lumi                         |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      15 Apr 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <hidapi.h>
#include "RGBController.h"

enum
{
    CRYORIG_H7_QUAD_LUMI_CHANNEL_ALL    = 0x00,     /* All channels                 */
    CRYORIG_H7_QUAD_LUMI_CHANNEL_1      = 0x01,     /* Channel 1                    */
    CRYORIG_H7_QUAD_LUMI_CHANNEL_2      = 0x02,     /* Channel 2                    */
    CRYORIG_H7_QUAD_LUMI_NUM_CHANNELS   = 0x02      /* Number of channels           */
};

enum
{
    CRYORIG_H7_QUAD_LUMI_SPEED_SLOWEST  = 0x00,     /* Slowest speed                */
    CRYORIG_H7_QUAD_LUMI_SPEED_SLOW     = 0x01,     /* Slow speed                   */
    CRYORIG_H7_QUAD_LUMI_SPEED_NORMAL   = 0x02,     /* Normal speed                 */
    CRYORIG_H7_QUAD_LUMI_SPEED_FAST     = 0x03,     /* Fast speed                   */
    CRYORIG_H7_QUAD_LUMI_SPEED_FASTEST  = 0x04,     /* Fastest speed                */
};

enum
{
    CRYORIG_H7_QUAD_LUMI_MODE_FIXED         = 0x00, /* Fixed colors mode            */
    CRYORIG_H7_QUAD_LUMI_MODE_FADING        = 0x01, /* Fading mode                  */
    CRYORIG_H7_QUAD_LUMI_MODE_SPECTRUM      = 0x02, /* Spectrum cycle mode          */
    CRYORIG_H7_QUAD_LUMI_MODE_MARQUEE       = 0x03, /* Marquee mode                 */
    CRYORIG_H7_QUAD_LUMI_MODE_COVER_MARQUEE = 0x04, /* Cover marquee mode           */
    CRYORIG_H7_QUAD_LUMI_MODE_ALTERNATING   = 0x05, /* Alternating mode             */
    CRYORIG_H7_QUAD_LUMI_MODE_BREATHING     = 0x06, /* Breathing mode               */
    CRYORIG_H7_QUAD_LUMI_MODE_PULSING       = 0x07, /* Pulsing mode                 */
};

class CryorigH7QuadLumiController
{
public:
    CryorigH7QuadLumiController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~CryorigH7QuadLumiController();

    std::string     GetFirmwareVersion();
    std::string     GetLocation();
    std::string     GetName();
    std::string     GetSerialString();

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

private:
    hid_device*     dev;

    char            firmware_version[16];
    std::string     location;
    std::string     name;

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

    void            SendFirmwareRequest();
};
