/*---------------------------------------------------------*\
| SteelSeriesRival5Controller.h                             |
|                                                           |
|  SteelSeriesRival5Controller for SteelSeries Rival 5      |
|                                                           |
|  A Ingram (Woodsy900)                        20 Jul 2026  |
|                                                           |
|  This file is part of the OpenRGB project                 |
|  SPDX-License-Identifier: GPL-2.0-or-later                |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <hidapi.h>
#include <cstring>
#include "RGBController.h"
#include "SteelSeriesDevices.h"
#include "SteelSeriesMouseController.h"

#define STEELSERIES_RIVAL5_NAME "SteelSeries Rival 5"

static const steelseries_mouse rival_5 =
{
    {
        0x04, // Direct
        0x07, // Reactive
        0x08  // Rainbow (for Rival 5, this is a static rainbow effect that cannot be changed)
    },

    {
        { "Scroll Wheel",  0 },
        { "Body Left 1",   1 },
        { "Body Right 1",  2 },
        { "Body Left 2",   3 },
        { "Body Right 2",  4 },
        { "Body Left 3",   5 },
        { "Body Right 3",  6 },
        { "Body Left 4",   7 },
        { "Body Right 4",  8 },
        { "Logo",          9 }
    }
};

class SteelSeriesRival5Controller : public SteelSeriesMouseController
{

    public:
        SteelSeriesRival5Controller(
            hid_device* dev_handle,
            steelseries_type proto_type,
            const char* path,
            std::string dev_name
        );

        ~SteelSeriesRival5Controller();

    std::string GetFirmwareVersion() override;
    steelseries_mouse GetMouse() override;

    void Save() override;
    void SetLightEffectAll(uint8_t effect) override;
    void SetRainbowEffect(bool enable);
    void SetBrightness(uint8_t brightness);
    void SetColor(
        unsigned char zone_id,
        unsigned char red,
        unsigned char green,
        unsigned char blue,
        unsigned char brightness
    ) override;
    void SetReactiveColor(
        bool on,
        uint8_t r,
        uint8_t g,
        uint8_t b
    );
    void UpdateLEDs(const std::vector<RGBColor>& colors);

private:
    uint8_t zone_colors[10][3];
    uint8_t current_brightness = 0xFF;
};