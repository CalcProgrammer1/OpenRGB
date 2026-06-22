/*---------------------------------------------------------*\
| RGBController_QMKKeychron.cpp                             |
|                                                           |
|   RGBController for Keychron QMK-based keyboards          |
|   (Q1 HE and other KEYCHRON_RGB-enabled models)           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <chrono>
#include <cstring>
#include <thread>
#include <cmath>
#include "RGBControllerKeyNames.h"
#include "RGBController_QMKKeychron.h"
#include "QMKKeychronController.h"

#define NA      0xFFFFFFFF

/**------------------------------------------------------------------*\
    @name Keychron Q1 HE
    @category Keyboard
    @type USB
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectQMKKeychronController
    @comment
\*-------------------------------------------------------------------*/

typedef struct
{
    std::string name;
    int         value;
    int         flags;
} keychron_qhe_effect;

static const keychron_qhe_effect qhe_effects[] =
{
    { "Direct",                         -1, MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS },
    { "Solid Color",                     0, MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE },
    { "Breathing",                       1, MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE },
    { "Band Spiral",                     2, MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE },
    { "Cycle All",                       3, MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE },
    { "Cycle Left Right",                4, MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE },
    { "Cycle Up Down",                   5, MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE },
    { "Cycle Out In",                    6, MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE },
    { "Cycle Out In Dual",              7, MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE },
    { "Cycle Pinwheel",                  8, MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE },
    { "Cycle Spiral",                    9, MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE },
    { "Dual Beacon",                    10, MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE },
    { "Rainbow Beacon",                 11, MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE },
    { "Rainbow Moving Chevron",        12, MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE },
    { "Jellybean Raindrops",           13, MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE },
    { "Pixel Rain",                    14, MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE },
    { "Typing Heatmap",                15, MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE },
    { "Digital Rain",                  16, MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE },
    { "Solid Reactive Simple",         17, MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE },
    { "Solid Reactive Multiwide",      18, MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE },
    { "Solid Reactive Multinexus",     19, MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE },
    { "Splash",                        20, MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE },
    { "Solid Splash",                  21, MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE },
};

RGBController_QMKKeychron::RGBController_QMKKeychron(QMKKeychronController* controller_ptr)
{
    controller                          = controller_ptr;

    name                                = controller->GetNameString();
    vendor                              = "Keychron";
    type                                = DEVICE_TYPE_KEYBOARD;
    description                         = name;
    location                            = controller->GetDeviceLocation();
    serial                              = controller->GetSerialString();

    for(const keychron_qhe_effect& effect : qhe_effects)
    {
        mode m;
        m.name   = effect.name;
        m.value  = effect.value;
        m.flags  = effect.flags;

        if(m.flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR)
        {
            m.color_mode = MODE_COLORS_MODE_SPECIFIC;
            m.colors_min = 1;
            m.colors_max = 1;
            m.colors.resize(1);
        }
        else if(m.flags & MODE_FLAG_HAS_PER_LED_COLOR)
        {
            m.color_mode = MODE_COLORS_PER_LED;
            m.colors_min = 0;
            m.colors_max = 0;
            m.colors.resize(0);
        }
        else
        {
            m.color_mode = MODE_COLORS_NONE;
            m.colors_min = 0;
            m.colors_max = 0;
            m.colors.resize(0);
        }

        if(m.flags & MODE_FLAG_HAS_SPEED)
        {
            m.speed_min = KEYCHRON_QHE_MIN_SPEED;
            m.speed_max = KEYCHRON_QHE_MAX_SPEED;
            m.speed     = KEYCHRON_QHE_MAX_SPEED / 2;
        }

        if(m.flags & MODE_FLAG_HAS_BRIGHTNESS)
        {
            m.brightness_min = KEYCHRON_QHE_MIN_BRIGHTNESS;
            m.brightness_max = KEYCHRON_QHE_MAX_BRIGHTNESS;
            m.brightness     = KEYCHRON_QHE_MAX_BRIGHTNESS;
        }

        modes.push_back(m);
    }

    SetupZones();
}

RGBController_QMKKeychron::~RGBController_QMKKeychron()
{
    delete controller;
}

void RGBController_QMKKeychron::SetupZones()
{
    unsigned int led_count = controller->GetLedCount();

    zone keyboard_zone;
    keyboard_zone.name               = ZONE_EN_KEYBOARD;
    keyboard_zone.type               = ZONE_TYPE_MATRIX;

    if(led_count > 0)
    {
        keyboard_zone.matrix_map         = new matrix_map_type;
        keyboard_zone.matrix_map->height = 6;
        keyboard_zone.matrix_map->width  = 16;

        keyboard_zone.matrix_map->map    = new unsigned int[6 * 16];
        memset(keyboard_zone.matrix_map->map, 0xFF, 6 * 16 * sizeof(unsigned int));

        std::vector<std::vector<int>> led_map = controller->GetAllLedNumbers(6);

        unsigned int led_idx = 0;
        for(unsigned int h = 0; h < 6 && h < led_map.size(); h++)
        {
            for(unsigned int w = 0; w < 16 && w < led_map[h].size(); w++)
            {
                int val = led_map[h][w];
                keyboard_zone.matrix_map->map[h * 16 + w] = (val >= 0) ? (unsigned int)val : NA;

                if(val >= 0)
                {
                    led new_led;
                    new_led.name = "Key: " + std::to_string(val);
                    leds.push_back(new_led);
                    led_idx++;
                }
            }
        }

        keyboard_zone.leds_min   = led_idx;
        keyboard_zone.leds_max   = led_idx;
        keyboard_zone.leds_count = led_idx;
    }
    else
    {
        keyboard_zone.matrix_map         = nullptr;
        keyboard_zone.leds_min           = 0;
        keyboard_zone.leds_max           = 0;
        keyboard_zone.leds_count         = 0;
    }

    zones.push_back(keyboard_zone);

    SetupColors();
}

void RGBController_QMKKeychron::ResizeZone(int /*zone*/, int /*new_size*/)
{
}

void RGBController_QMKKeychron::DeviceUpdateLEDs()
{
    UpdateZoneLEDs(0);
}

void RGBController_QMKKeychron::UpdateZoneLEDs(int /*zone*/)
{
    if(active_mode == 0)
    {
        controller->SetRgbMatrixMode(KEYCHRON_QHE_PER_KEY_RGB_EFFECT);
        controller->SetPerKeyRgbType(KEYCHRON_PER_KEY_RGB_SOLID);

        unsigned int total_leds = leds.size();
        unsigned char max_per_packet = 9;

        for(unsigned char start = 0; start < total_leds; start += max_per_packet)
        {
            unsigned char count = (unsigned char)((total_leds - start) > max_per_packet ? max_per_packet : (total_leds - start));

            std::vector<unsigned char> hsv_data;
            hsv_data.reserve(count * 3);

            for(unsigned char i = 0; i < count; i++)
            {
                RGBColor color = colors[start + i];
                unsigned char r = RGBGetRValue(color);
                unsigned char g = RGBGetGValue(color);
                unsigned char b = RGBGetBValue(color);
                unsigned char h, s, v;

                RGBToHSV(r, g, b, h, s, v);

                hsv_data.push_back(h);
                hsv_data.push_back(s);
                hsv_data.push_back(v);
            }

            controller->SetPerKeyRgbColor(start, count, hsv_data);
        }

        controller->SetBrightness(modes[active_mode].brightness);
    }
    else
    {
        int effect_id = modes[active_mode].value;

        controller->SetRgbMatrixMode((unsigned char)effect_id);
        controller->SetBrightness(modes[active_mode].brightness);
        controller->SetSpeed(modes[active_mode].speed);

        if(modes[active_mode].flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR)
        {
            RGBColor color = modes[active_mode].colors[0];
            unsigned char r = RGBGetRValue(color);
            unsigned char g = RGBGetGValue(color);
            unsigned char b = RGBGetBValue(color);
            unsigned char h, s, v;

            RGBToHSV(r, g, b, h, s, v);

            controller->SetColorHSV(h, s);
        }

        controller->SaveLedConf();
    }
}

void RGBController_QMKKeychron::UpdateSingleLED(int /*led*/)
{
    UpdateZoneLEDs(0);
}

void RGBController_QMKKeychron::DeviceUpdateMode()
{
    UpdateZoneLEDs(0);
}

void RGBController_QMKKeychron::RGBToHSV(unsigned char r, unsigned char g, unsigned char b, unsigned char& h, unsigned char& s, unsigned char& v)
{
    double rd = r / 255.0;
    double gd = g / 255.0;
    double bd = b / 255.0;

    double max_val = rd > gd ? (rd > bd ? rd : bd) : (gd > bd ? gd : bd);
    double min_val = rd < gd ? (rd < bd ? rd : bd) : (gd < bd ? gd : bd);
    double delta   = max_val - min_val;

    v = (unsigned char)(max_val * 255.0);

    if(delta < 0.00001)
    {
        h = 0;
        s = 0;
        return;
    }

    s = (unsigned char)((max_val > 0.0) ? (delta / max_val) * 255.0 : 0.0);

    double hd;
    if(max_val == rd)
    {
        hd = (gd - bd) / delta;
        if(gd < bd) hd += 6.0;
    }
    else if(max_val == gd)
    {
        hd = (bd - rd) / delta + 2.0;
    }
    else
    {
        hd = (rd - gd) / delta + 4.0;
    }

    h = (unsigned char)((hd / 6.0) * 255.0);
}
