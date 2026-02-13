/*---------------------------------------------------------*\
| LianLiUniHubSLController.h                                |
|                                                           |
|   Driver for Lian Li Uni Hub - SL                         |
|                                                           |
|   Muhamad Visat                               26 Jul 2025 |
|   Original work by Luca Lovisa & Oliver P                 |
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
    UNIHUB_SL_REPORT_ID                     = 0xE0,
};

enum
{
    UNIHUB_SL_MAX_CHANNEL                   = 4,
    UNIHUB_SL_MAX_FAN_PER_CHANNEL           = 4,
    UNIHUB_SL_LED_PER_FAN                   = 16,
    UNIHUB_SL_MAX_LED_PER_CHANNEL           = 64,
};

enum
{
    UNIHUB_SL_LED_BRIGHTNESS_MIN            = 0x00, // 0%
    UNIHUB_SL_LED_BRIGHTNESS_MAX            = 0x04, // 100%
    UNIHUB_SL_LED_BRIGHTNESS_DEFAULT        = 0x04, // 100%

    UNIHUB_SL_LED_BRIGHTNESS_000            = 0x08, // Black (turned off)
    UNIHUB_SL_LED_BRIGHTNESS_025            = 0x03, // Dark
    UNIHUB_SL_LED_BRIGHTNESS_050            = 0x02, // Medium
    UNIHUB_SL_LED_BRIGHTNESS_075            = 0x01, // Bright
    UNIHUB_SL_LED_BRIGHTNESS_100            = 0x00, // Brightest
};

enum
{
    UNIHUB_SL_LED_SPEED_MIN                 = 0x00, // Slowest
    UNIHUB_SL_LED_SPEED_MAX                 = 0x04, // Fastest
    UNIHUB_SL_LED_SPEED_DEFAULT             = 0x00, // Slowest

    UNIHUB_SL_LED_SPEED_000                 = 0x02, // Slowest
    UNIHUB_SL_LED_SPEED_025                 = 0x01, // Slow
    UNIHUB_SL_LED_SPEED_050                 = 0x00, // Medium
    UNIHUB_SL_LED_SPEED_075                 = 0xff, // Fast
    UNIHUB_SL_LED_SPEED_100                 = 0xfe, // Fastest
};

enum
{
    UNIHUB_SL_LED_DIRECTION_LTR             = 0x00, // Left to right
    UNIHUB_SL_LED_DIRECTION_RTL             = 0x01, // Right to left
};

enum
{
    UNIHUB_SL_LED_MODE_RAINBOW              = 0x05,
    UNIHUB_SL_LED_MODE_RAINBOW_MORPH        = 0x04,
    UNIHUB_SL_LED_MODE_STATIC               = 0x01,
    UNIHUB_SL_LED_MODE_BREATHING            = 0x02,
    UNIHUB_SL_LED_MODE_COLOR_CYCLE          = 0x23,
    UNIHUB_SL_LED_MODE_RUNWAY               = 0x1c,
    UNIHUB_SL_LED_MODE_RUNWAY_MERGED        = 0x1c,
    UNIHUB_SL_LED_MODE_STAGGERED            = 0x18,
    UNIHUB_SL_LED_MODE_TIDE                 = 0x1a,
    UNIHUB_SL_LED_MODE_METEOR               = 0x24,
    UNIHUB_SL_LED_MODE_METEOR_MERGED        = 0x24,
    UNIHUB_SL_LED_MODE_MIXING               = 0x1e,
    UNIHUB_SL_LED_MODE_STACK                = 0x20,
    UNIHUB_SL_LED_MODE_STACK_MULTI_COLOR    = 0x21,
    UNIHUB_SL_LED_MODE_NEON                 = 0x22,
};

class LianLiUniHubSLController
{
public:
    LianLiUniHubSLController(hid_device *device, const char *path);
    ~LianLiUniHubSLController();

    std::string ReadVersion();
    std::string ReadSerial();

    std::string GetLocation() { return this->location; };

    void UpdateMode(const std::vector<zone> &zones, const mode &active);
    void UpdateZoneLEDs(size_t channel, const zone &z, const mode &active);

private:
    hid_device *device;
    std::string location;

    bool is_merged_mode;

    void SetPerLEDColor(size_t channel, const zone &z, const mode &active);
    void SetModeSpecificColor(size_t channel, const mode &active);
    void FillStaticColorBuffer(unsigned char *color_buf, const RGBColor *colors, size_t num_colors, float brightness_scale);
    void FillDynamicColorBuffer(unsigned char *color_buf, const RGBColor *colors, size_t num_colors, float brightness_scale);

    unsigned char ConvertBrightness(unsigned int brightness);
    unsigned char ConvertSpeed(unsigned int speed);
    unsigned char ConvertDirection(unsigned int direction);
    
    void SendActivate(size_t channel, unsigned char num_fans);
    void SendMerge();
    void SendColor(size_t channel, const unsigned char *color_buf, size_t color_buf_len);
    void SendMode(size_t channel, const mode &active);
    
    void LogBuffer(const char *operation, const unsigned char *buf, size_t len);
};
