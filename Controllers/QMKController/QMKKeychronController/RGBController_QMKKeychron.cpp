/*---------------------------------------------------------*\
| RGBController_QMKKeychron.cpp                             |
|                                                           |
|   RGBController for Keychron QMK-based keyboards          |
|                                                           |
|   Amadej Kastelic                             21 Jun 2026 |
|   Adam Honse <calcprogrammer1@gmail.com>      22 Jun 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include "hsv.h"
#include "RGBController_QMKKeychron.h"
#include "QMKKeycodes.h"
#include "QMKKeychronController.h"

/**------------------------------------------------------------------*\
    @name QMK Keychron
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
} kc_effect;

static const kc_effect kc_effects[] =
{
    { "Direct",                     0xFFFF, MODE_FLAG_HAS_PER_LED_COLOR },
    { "Solid Color",                1,      MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_MANUAL_SAVE },
    { "Breathing",                  2,      MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE },
    { "Band Spiral",                3,      MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE },
    { "Cycle All",                  4,      MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE },
    { "Cycle Left Right",           5,      MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE },
    { "Cycle Up Down",              6,      MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE },
    { "Rainbow Moving Chevron",     7,      MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE },
    { "Cycle Out In",               8,      MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE },
    { "Cycle Out In Dual",          9,      MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE },
    { "Cycle Pinwheel",             10,     MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE },
    { "Cycle Spiral",               11,     MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE },
    { "Dual Beacon",                12,     MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE },
    { "Rainbow Beacon",             13,     MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE },
    { "Jellybean Raindrops",        14,     MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE },
    { "Pixel Rain",                 15,     MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE },
    { "Typing Heatmap",             16,     MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE },
    { "Digital Rain",               17,     MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE },
    { "Solid Reactive Simple",      18,     MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE },
    { "Solid Reactive Multiwide",   19,     MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE },
    { "Solid Reactive Multinexus",  20,     MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE },
    { "Splash",                     21,     MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE },
    { "Solid Splash",               22,     MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE },
};

RGBController_QMKKeychron::RGBController_QMKKeychron(QMKKeychronController* controller_ptr)
{
    controller                          = controller_ptr;

    name                                = controller->GetName();
    vendor                              = controller->GetVendor();
    type                                = DEVICE_TYPE_KEYBOARD;
    description                         = "QMK Keychron Device";
    location                            = controller->GetLocation();
    serial                              = controller->GetSerial();
    version                             = controller->GetVersion();

    for(const kc_effect& effect : kc_effects)
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
    /*-----------------------------------------------------*\
    | Build matrix map                                      |
    \*-----------------------------------------------------*/
    unsigned char max_col = 0;
    unsigned char max_row = 0;

    for(unsigned short led_index = 0; led_index < controller->GetLEDCount(); led_index++)
    {
        qmk_rgb_matrix_led_info info = controller->GetLEDInfo(led_index);

        if(info.col > max_col)
        {
            max_col = info.col;
        }

        if(info.row > max_row)
        {
            max_row = info.row;
        }
    }

    unsigned char height = max_row + 1;
    unsigned char width  = max_col + 1;

    unsigned int* matrix_map = new unsigned int[width * height];

    memset(matrix_map, 0xFF, (sizeof(unsigned int) * (width * height)));

    for(unsigned short led_index = 0; led_index < controller->GetLEDCount(); led_index++)
    {
        qmk_rgb_matrix_led_info info = controller->GetLEDInfo(led_index);

        matrix_map[(width * info.row) + info.col] = (unsigned int)led_index;
    }

    /*-----------------------------------------------------*\
    | Create keyboard zone                                  |
    \*-----------------------------------------------------*/
    zone keyboard;

    keyboard.name               = "Keyboard";
    keyboard.type               = ZONE_TYPE_MATRIX;
    keyboard.leds_min           = controller->GetLEDCount();
    keyboard.leds_max           = controller->GetLEDCount();
    keyboard.leds_count         = controller->GetLEDCount();
    keyboard.matrix_map         = new matrix_map_type;
    keyboard.matrix_map->height = height;
    keyboard.matrix_map->width  = width;
    keyboard.matrix_map->map    = matrix_map;

    zones.push_back(keyboard);

    /*-----------------------------------------------------*\
    | Create keyboard LEDs                                  |
    \*-----------------------------------------------------*/
    for(unsigned short led_idx = 0; led_idx < controller->GetLEDCount(); led_idx++)
    {
        led new_led;
        new_led.name = qmk_keynames[controller->GetKeycode(led_idx)];

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_QMKKeychron::ResizeZone(int /*zone*/, int /*new_size*/)
{
}

void RGBController_QMKKeychron::DeviceUpdateLEDs()
{
    controller->SendLEDs((unsigned char)colors.size(), colors.data());
}

void RGBController_QMKKeychron::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_QMKKeychron::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_QMKKeychron::DeviceUpdateMode()
{
    unsigned char hue = 0;
    unsigned char sat = 255;
    unsigned char val = modes[active_mode].brightness;

    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        hsv_t hsv_color;
        rgb2hsv(modes[active_mode].colors[0], &hsv_color);

        hue = (unsigned char)((float)hsv_color.hue * (256.0f / 360.0f));
        sat = hsv_color.saturation;
        val = hsv_color.value;
    }

    controller->SetMode(modes[active_mode].value, modes[active_mode].speed, hue, sat, val);
}

void RGBController_QMKKeychron::DeviceSaveMode()
{
    controller->SaveMode();
}
