/*---------------------------------------------------------*\
| NZXTHue2Controller.h                                      |
|                                                           |
|   Driver for NZXT Hue 2                                   |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      29 Dec 2019 |
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
    HUE_2_CHANNEL_ALL       = 0x00,     /* All channels                 */
    HUE_2_CHANNEL_1         = 0x01,     /* Channel 1                    */
    HUE_2_CHANNEL_2         = 0x02,     /* Channel 2                    */
    HUE_2_CHANNEL_3         = 0x03,     /* Channel 3                    */
    HUE_2_CHANNEL_4         = 0x04,     /* Channel 4                    */
    HUE_2_CHANNEL_5         = 0x05,     /* Channel 5                    */
    HUE_2_CHANNEL_6         = 0x06,     /* Channel 6                    */
    HUE_2_NUM_CHANNELS      = 0x06      /* Number of channels           */
};

enum
{
    HUE_2_CHANNEL_1_IDX     = 0x00,     /* Channel 1 array index        */
    HUE_2_CHANNEL_2_IDX     = 0x01,     /* Channel 2 array index        */
    HUE_2_CHANNEL_3_IDX     = 0x01,     /* Channel 3 array index        */
    HUE_2_CHANNEL_4_IDX     = 0x01,     /* Channel 4 array index        */
};

enum
{
    HUE_2_SPEED_SLOWEST         = 0x00,     /* Slowest speed                */
    HUE_2_SPEED_SLOW            = 0x01,     /* Slow speed                   */
    HUE_2_SPEED_NORMAL          = 0x02,     /* Normal speed                 */
    HUE_2_SPEED_FAST            = 0x03,     /* Fast speed                   */
    HUE_2_SPEED_FASTEST         = 0x04,     /* Fastest speed                */
};

enum
{
    HUE_2_MODE_FIXED            = 0x00,     /* Fixed colors mode            */
    HUE_2_MODE_FADING           = 0x01,     /* Fading mode                  */
    HUE_2_MODE_SPECTRUM         = 0x02,     /* Spectrum cycle mode          */
    HUE_2_MODE_MARQUEE          = 0x03,     /* Marquee mode                 */
    HUE_2_MODE_COVER_MARQUEE    = 0x04,     /* Cover marquee mode           */
    HUE_2_MODE_ALTERNATING      = 0x05,     /* Alternating mode             */
    HUE_2_MODE_PULSING          = 0x06,     /* Pulsing mode                 */
    HUE_2_MODE_BREATHING        = 0x07,     /* Breathing mode               */
    HUE_2_MODE_CANDLE           = 0x08,     /* Candle Mode                  */
    HUE_2_MODE_STARRY_NIGHT     = 0x09,     /* Starry Night mode            */
    HUE_2_MODE_RAINBOW_FLOW     = 0x0b,     /* Rainbow Flow mode            */
    HUE_2_MODE_SUPER_RAINBOW    = 0x0c,     /* Super Rainbow mode           */
    HUE_2_MODE_RAINBOW_PULSE    = 0x0d,     /* Rainbow Pulse mode           */
    HUE_2_NUM_MODES                         /* Number of Hue 2 modes        */
};

class NZXTHue2Controller
{
public:
    NZXTHue2Controller(hid_device* dev_handle, unsigned int rgb_channels, unsigned int fan_channels, const char* path, std::string dev_name);
    ~NZXTHue2Controller();

    std::string     GetFirmwareVersion();
    std::string     GetLocation();
    std::string     GetName();
    std::string     GetSerialString();

    unsigned char   GetFanCommand
                        (
                        unsigned char   fan_channel
                        );

    unsigned short  GetFanRPM
                        (
                        unsigned char   fan_channel
                        );

    unsigned int    GetNumFanChannels();

    unsigned int    GetNumRGBChannels();

    void            SendFan
                        (
                            unsigned char       port,
                            unsigned char       mode,
                            unsigned char       speed
                        );

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

    void            UpdateDeviceList();

    void            UpdateStatus();

    unsigned int    channel_leds[HUE_2_NUM_CHANNELS];
    unsigned int    channel_dev_ids[HUE_2_NUM_CHANNELS][6];
    unsigned int    channel_dev_szs[HUE_2_NUM_CHANNELS][6];

private:
    hid_device*     dev;

    std::vector<unsigned char>  fan_cmd;
    std::vector<unsigned short> fan_rpm;

    char            firmware_version[16];
    std::string     location;
    std::string     name;
    unsigned int    num_fan_channels;
    unsigned int    num_rgb_channels;

    void            SendApply
                        (
                        unsigned char   channel
                        );

    void            SendDirect
                        (
                        unsigned char   channel,
                        unsigned char   group,
                        unsigned char   color_count,
                        unsigned char*  color_data
                        );

    void            SendEffect
                        (
                        unsigned char   channel,
                        unsigned char   mode,
                        unsigned char   speed,
                        bool            direction,
                        unsigned char   color_count,
                        unsigned char*  color_data
                        );

    void            SendFirmwareRequest();
};
