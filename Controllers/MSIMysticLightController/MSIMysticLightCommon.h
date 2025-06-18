/*---------------------------------------------------------*\
| MSIMysticLightCommon.h                                    |
|                                                           |
|   Common definitions for MSI Mystic Light motherboards    |
|                                                           |
|   Adam Honse                                  06 Mar 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>

enum MSI_ZONE
{
    MSI_ZONE_NONE                   = 0,
    MSI_ZONE_J_RGB_1                = 1,
    MSI_ZONE_J_RGB_2                = 2,
    MSI_ZONE_J_PIPE_1               = 3,
    MSI_ZONE_J_PIPE_2               = 4,
    MSI_ZONE_J_RAINBOW_1            = 5,
    MSI_ZONE_J_RAINBOW_2            = 6,
    MSI_ZONE_J_RAINBOW_3            = 7,
    MSI_ZONE_J_CORSAIR              = 8,
    MSI_ZONE_J_CORSAIR_OUTERLL120   = 9,
    MSI_ZONE_ON_BOARD_LED_0         = 10,
    MSI_ZONE_ON_BOARD_LED_1         = 11,
    MSI_ZONE_ON_BOARD_LED_2         = 12,
    MSI_ZONE_ON_BOARD_LED_3         = 13,
    MSI_ZONE_ON_BOARD_LED_4         = 14,
    MSI_ZONE_ON_BOARD_LED_5         = 15,
    MSI_ZONE_ON_BOARD_LED_6         = 16,
    MSI_ZONE_ON_BOARD_LED_7         = 17,
    MSI_ZONE_ON_BOARD_LED_8         = 18,
    MSI_ZONE_ON_BOARD_LED_9         = 19,
    MSI_ZONE_ON_BOARD_LED_10        = 20,
    MSI_ZONE_JAF                    = 21,
    MSI_ZONE_JARGB_1                = 22,
    MSI_ZONE_JARGB_2                = 23,
    MSI_ZONE_JARGB_3                = 24,
};

enum MSI_MODE
{
    MSI_MODE_DISABLE                    = 0,
    MSI_MODE_STATIC                     = 1,
    MSI_MODE_BREATHING                  = 2,
    MSI_MODE_FLASHING                   = 3,
    MSI_MODE_DOUBLE_FLASHING            = 4,
    MSI_MODE_LIGHTNING                  = 5,
    MSI_MODE_MSI_MARQUEE                = 6,
    MSI_MODE_METEOR                     = 7,
    MSI_MODE_WATER_DROP                 = 8,
    MSI_MODE_MSI_RAINBOW                = 9,
    MSI_MODE_POP                        = 10,
    MSI_MODE_RAP                        = 11,
    MSI_MODE_JAZZ                       = 12,
    MSI_MODE_PLAY                       = 13,
    MSI_MODE_MOVIE                      = 14,
    MSI_MODE_COLOR_RING                 = 15,
    MSI_MODE_PLANETARY                  = 16,
    MSI_MODE_DOUBLE_METEOR              = 17,
    MSI_MODE_ENERGY                     = 18,
    MSI_MODE_BLINK                      = 19,
    MSI_MODE_CLOCK                      = 20,
    MSI_MODE_COLOR_PULSE                = 21,
    MSI_MODE_COLOR_SHIFT                = 22,
    MSI_MODE_COLOR_WAVE                 = 23,
    MSI_MODE_MARQUEE                    = 24,
    MSI_MODE_RAINBOW                    = 25,
    MSI_MODE_RAINBOW_WAVE               = 26,
    MSI_MODE_VISOR                      = 27,
    MSI_MODE_JRAINBOW                   = 28,
    MSI_MODE_RAINBOW_FLASHING           = 29,
    MSI_MODE_RAINBOW_DOUBLE_FLASHING    = 30,
    MSI_MODE_RANDOM                     = 31,
    MSI_MODE_FAN_CONTROL                = 32,
    MSI_MODE_DISABLE_2                  = 33,
    MSI_MODE_COLOR_RING_FLASHING        = 34,
    MSI_MODE_COLOR_RING_DOUBLE_FLASHING = 35,
    MSI_MODE_STACK                      = 36,
    MSI_MODE_CORSAIR_QUE                = 37,
    MSI_MODE_FIRE                       = 38,
    MSI_MODE_LAVA                       = 39,
    MSI_MODE_DIRECT_DUMMY               = 100
};

enum MSI_SPEED
{
    MSI_SPEED_LOW                       = 0,
    MSI_SPEED_MEDIUM                    = 1,
    MSI_SPEED_HIGH                      = 2,
};

enum MSI_FAN_TYPE
{
    MSI_FAN_TYPE_SP                     = 0,
    MSI_FAN_TYPE_HD                     = 1,
    MSI_FAN_TYPE_LL                     = 2,
};

enum MSI_BRIGHTNESS
{
    MSI_BRIGHTNESS_OFF                  = 0,
    MSI_BRIGHTNESS_LEVEL_10             = 1,
    MSI_BRIGHTNESS_LEVEL_20             = 2,
    MSI_BRIGHTNESS_LEVEL_30             = 3,
    MSI_BRIGHTNESS_LEVEL_40             = 4,
    MSI_BRIGHTNESS_LEVEL_50             = 5,
    MSI_BRIGHTNESS_LEVEL_60             = 6,
    MSI_BRIGHTNESS_LEVEL_70             = 7,
    MSI_BRIGHTNESS_LEVEL_80             = 8,
    MSI_BRIGHTNESS_LEVEL_90             = 9,
    MSI_BRIGHTNESS_LEVEL_100            = 10,
};

#define NUMOF_PER_LED_MODE_LEDS                  240
#define NUM_LEDS_761                             720

#define SYNC_SETTING_ONBOARD        0x01
#define SYNC_SETTING_JRAINBOW1      0x02
#define SYNC_SETTING_JRAINBOW2      0x04
#define SYNC_SETTING_JCORSAIR       0x08
#define SYNC_SETTING_JPIPE1         0x10
#define SYNC_SETTING_JPIPE2         0x20
#define SYNC_SETTING_JRGB           0x80

#define MSI_64_MAX_COLORS           7

struct Color
{
    unsigned char R;
    unsigned char G;
    unsigned char B;
};

struct CorsairZoneData
{
    unsigned char effect                    = MSI_MODE_STATIC;
    Color         color                     { 0, 0, 0 };
    unsigned char fan_flags                 = 40;
    unsigned char corsair_quantity          = 0;
    unsigned char padding[4]                = { 0, 0, 0, 0 };
    unsigned char is_individual             = 0;
};

struct ZoneData
{
    unsigned char effect                    = MSI_MODE_STATIC;
    Color         color                     { 0, 0, 0 };
    unsigned char speedAndBrightnessFlags   = 0;
    Color         color2                    { 0, 0, 0 };
    unsigned char colorFlags                = 0;
    unsigned char padding                   = 0;
};

struct RainbowZoneData : ZoneData
{
    unsigned char cycle_or_led_num          = 100;
};

struct FeaturePacket_64
{
    const unsigned char report_id                 = 0x02; // Report ID
    const unsigned char second_byte               = 0x00;
    unsigned char       mode                      = 0x00;
    unsigned char       speed                     = 0x00;
    unsigned char       brightness                = 0x00;
    unsigned char       num_colors                = 0x00;
    Color               colors[MSI_64_MAX_COLORS] = {};
    const unsigned char padding[37]               = {}; //pad to make the packet size 64 bytes
};

struct FeaturePacket_112
{
    const unsigned char report_id           = 0x52; // Report ID
    ZoneData            j_rgb_1;                    // 1
    ZoneData            j_rainbow_1;                // 11
    ZoneData            j_corsair_1;                // 21
    ZoneData            j_corsair_outerll120;       // 31
    ZoneData            on_board_led;               // 41
    ZoneData            on_board_led_1;             // 51
    ZoneData            on_board_led_2;             // 61
    ZoneData            on_board_led_3;             // 71
    ZoneData            on_board_led_4;             // 81
    ZoneData            on_board_led_5;             // 91
    ZoneData            on_board_led_6;             // 101
    unsigned char       save_data           = 0;    // 111
};

struct FeaturePacket_162
{
    const unsigned char report_id           = 0x52; // Report ID
    ZoneData            j_rgb_1;                    // 1
    ZoneData            j_rainbow_1;                // 11
    ZoneData            j_corsair_1;                // 21
    ZoneData            j_corsair_outerll120;       // 31
    ZoneData            on_board_led;               // 41
    ZoneData            on_board_led_1;             // 51
    ZoneData            on_board_led_2;             // 61
    ZoneData            on_board_led_3;             // 71
    ZoneData            on_board_led_4;             // 81
    ZoneData            on_board_led_5;             // 91
    ZoneData            on_board_led_6;             // 101
    ZoneData            on_board_led_7;             // 111
    ZoneData            on_board_led_8;             // 121
    ZoneData            on_board_led_9;             // 131
    ZoneData            on_board_led_10;            // 141
    ZoneData            j_rgb_2;                    // 151
    unsigned char       save_data           = 0;    // 161
};

struct FeaturePacket_185
{
    const unsigned char report_id           = 0x52; // Report ID
    ZoneData            j_rgb_1;                    // 1
    ZoneData            j_pipe_1;                   // 11
    ZoneData            j_pipe_2;                   // 21
    RainbowZoneData     j_rainbow_1;                // 31
    RainbowZoneData     j_rainbow_2;                // 42
    CorsairZoneData     j_corsair;                  // 53
    ZoneData            j_corsair_outerll120;       // 64
    ZoneData            on_board_led;               // 74
    ZoneData            on_board_led_1;             // 84
    ZoneData            on_board_led_2;             // 94
    ZoneData            on_board_led_3;             // 104
    ZoneData            on_board_led_4;             // 114
    ZoneData            on_board_led_5;             // 124
    ZoneData            on_board_led_6;             // 134
    ZoneData            on_board_led_7;             // 144
    ZoneData            on_board_led_8;             // 154
    ZoneData            on_board_led_9;             // 164
    ZoneData            j_rgb_2;                    // 174
    unsigned char       save_data           = 0;    // 184
};

struct FeaturePacket_PerLED_185
{
    unsigned char report_id                              = 0x53;      // Report ID
    unsigned char hdr0                                   = 0x25;      // header byte 0
    unsigned char hdr1                                   = 0x06;      // header byte 1
    unsigned char hdr2                                   = 0x00;      // header byte 2
    unsigned char hdr3                                   = 0x00;      // header byte 3
    Color         leds[NUMOF_PER_LED_MODE_LEDS];
};

struct FeaturePacket_PerLED_761
{
    unsigned char report_id                              = 0x51;      // Report ID
    unsigned char fixed1                                 = 0x09;      // Always 9?
    unsigned char hdr0;                                               // 0x08 for ez 0x04 for ARGB 0x06 for LED
    unsigned char hdr1;                                               // 0 for LED/EZ and argb num for ARGB?
    unsigned char fixed2                                 = 0x00;      // IDK what this is
    unsigned char fixed3                                 = 0x00;      // IDK what this is
    unsigned char hdr2;                                               // Led Count
    unsigned char colors [NUM_LEDS_761];
};

struct FeaturePacket_Zone_761
{
    MSI_ZONE zone;
    FeaturePacket_PerLED_761 packet;
};

struct FeaturePacket_761
{
    FeaturePacket_Zone_761 jargb1;
    FeaturePacket_Zone_761 jargb2;
    FeaturePacket_Zone_761 jargb3;
    FeaturePacket_Zone_761 jaf;
};

#define MSI_USB_PID_COMMON  0x0076      // Common PID for a certain set of 185-byte boards
