/*---------------------------------------------------------*\
| RGBController_QMKVialRGB.cpp                              |
|                                                           |
|   RGBController for VialRGB QMK Keyboard Protocol         |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com)      29 Sep 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "hsv.h"
#include "QMKKeycodes.h"
#include "RGBController_QMKVialRGB.h"

typedef struct
{
    unsigned short  value;
    std::string     name;
    bool            has_speed;
} vialrgb_mode;

static const vialrgb_mode vialrgb_modes[] =
{
    { VIALRGB_EFFECT_OFF,                       "Off",                          false },
    { VIALRGB_EFFECT_DIRECT,                    "Direct",                       false },
    { VIALRGB_EFFECT_SOLID_COLOR,               "Static",                       false },
    { VIALRGB_EFFECT_ALPHAS_MODS,               "Alphas Mods",                  true  },
    { VIALRGB_EFFECT_GRADIENT_UP_DOWN,          "Gradient Up Down",             true  },
    { VIALRGB_EFFECT_GRADIENT_LEFT_RIGHT,       "Gradient Left Right",          true  },
    { VIALRGB_EFFECT_BREATHING,                 "Breathing",                    true  },
    { VIALRGB_EFFECT_BAND_SAT,                  "Band Sat",                     true  },
    { VIALRGB_EFFECT_BAND_VAL,                  "Band Val",                     true  },
    { VIALRGB_EFFECT_BAND_PINWHEEL_SAT,         "Band Pinwheel Sat",            true  },
    { VIALRGB_EFFECT_BAND_PINWHEEL_VAL,         "Band Pinwheel Val",            true  },
    { VIALRGB_EFFECT_BAND_SPIRAL_SAT,           "Band Spiral Sat",              true  },
    { VIALRGB_EFFECT_BAND_SPIRAL_VAL,           "Band Spiral Val",              true  },
    { VIALRGB_EFFECT_CYCLE_ALL,                 "Cycle All",                    true  },
    { VIALRGB_EFFECT_CYCLE_LEFT_RIGHT,          "Cycle Left Right",             true  },
    { VIALRGB_EFFECT_CYCLE_UP_DOWN,             "Cycle Up Down",                true  },
    { VIALRGB_EFFECT_RAINBOW_MOVING_CHEVRON,    "Rainbow Moving Chevron",       true  },
    { VIALRGB_EFFECT_CYCLE_OUT_IN,              "Cycle Out In",                 true  },
    { VIALRGB_EFFECT_CYCLE_OUT_IN_DUAL,         "Cycle Out In Dual",            true  },
    { VIALRGB_EFFECT_CYCLE_PINWHEEL,            "Cycle Pinwheel",               true  },
    { VIALRGB_EFFECT_CYCLE_SPIRAL,              "Cycle Spiral",                 true  },
    { VIALRGB_EFFECT_DUAL_BEACON,               "Dual Beacon",                  true  },
    { VIALRGB_EFFECT_RAINBOW_BEACON,            "Rainbow Beacon",               true  },
    { VIALRGB_EFFECT_RAINBOW_PINWHEELS,         "Rainbow Pinwheels",            true  },
    { VIALRGB_EFFECT_RAINDROPS,                 "Raindrops",                    true  },
    { VIALRGB_EFFECT_JELLYBEAN_RAINDROPS,       "Jellybean Raindrops",          true  },
    { VIALRGB_EFFECT_HUE_BREATHING,             "Hue Breathing",                true  },
    { VIALRGB_EFFECT_HUE_PENDULUM,              "Hue Pendulum",                 true  },
    { VIALRGB_EFFECT_HUE_WAVE,                  "Hue Wave",                     true  },
    { VIALRGB_EFFECT_TYPING_HEATMAP,            "Typing Heatmap",               true  },
    { VIALRGB_EFFECT_DIGITAL_RAIN,              "Digital Rain",                 true  },
    { VIALRGB_EFFECT_SOLID_REACTIVE_SIMPLE,     "Solid Reactive Simple",        true  },
    { VIALRGB_EFFECT_SOLID_REACTIVE,            "Solid Reactive",               true  },
    { VIALRGB_EFFECT_SOLID_REACTIVE_WIDE,       "Solid Reactive Wide",          true  },
    { VIALRGB_EFFECT_SOLID_REACTIVE_MULTIWIDE,  "Solid Reactive Multiwide",     true  },
    { VIALRGB_EFFECT_SOLID_REACTIVE_CROSS,      "Solid Reactive Cross",         true  },
    { VIALRGB_EFFECT_SOLID_REACTIVE_MULTICROSS, "Solid Reactive Multicross",    true  },
    { VIALRGB_EFFECT_SOLID_REACTIVE_NEXUS,      "Solid Reactive Nexus",         true  },
    { VIALRGB_EFFECT_SOLID_REACTIVE_MULTINEXUS, "Solid Reactive Multinexus",    true  },
    { VIALRGB_EFFECT_SPLASH,                    "Splash",                       true  },
    { VIALRGB_EFFECT_MULTISPLASH,               "Multisplash",                  true  },
    { VIALRGB_EFFECT_SOLID_SPLASH,              "Solid Splash",                 true  },
    { VIALRGB_EFFECT_SOLID_MULTISPLASH,         "Solid Multisplash",            true  },
    { VIALRGB_EFFECT_PIXEL_RAIN,                "Pixel Rain",                   true  },
    { VIALRGB_EFFECT_PIXEL_FRACTAL,             "Pixel Fractal",                true  },
};

RGBController_QMKVialRGB::RGBController_QMKVialRGB(QMKVialRGBController* controller_ptr)
{
    controller                  = controller_ptr;
    name                        = controller->GetName();
    description                 = "QMK VialRGB Device";
    vendor                      = controller->GetVendor();
    location                    = controller->GetLocation();
    serial                      = controller->GetSerial();
    version                     = controller->GetVersion();
    type                        = DEVICE_TYPE_KEYBOARD;

    /*-----------------------------------------------------*\
    | Read mode list                                        |
    \*-----------------------------------------------------*/
    for(std::size_t effect_idx = 0; effect_idx < controller->GetEffectCount(); effect_idx++)
    {
        mode new_mode;
        new_mode.name           = vialrgb_modes[controller->GetEffect(effect_idx)].name;
        new_mode.value          = vialrgb_modes[controller->GetEffect(effect_idx)].value;

        if(new_mode.value == VIALRGB_EFFECT_DIRECT)
        {
            new_mode.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
            new_mode.color_mode = MODE_COLORS_PER_LED;
        }

        if(new_mode.value >= VIALRGB_EFFECT_SOLID_COLOR)
        {
            new_mode.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
            new_mode.color_mode = MODE_COLORS_MODE_SPECIFIC;
            new_mode.colors_min = 1;
            new_mode.colors_max = 1;
            new_mode.colors.resize(1);
        }

        if(vialrgb_modes[controller->GetEffect(effect_idx)].has_speed)
        {
            new_mode.flags     |= MODE_FLAG_HAS_SPEED;
            new_mode.speed_min  = 0;
            new_mode.speed_max  = 255;
            new_mode.speed      = 128;
        }

        modes.push_back(new_mode);
    }

    /*-----------------------------------------------------*\
    | Read current mode                                     |
    \*-----------------------------------------------------*/
    unsigned short  cur_mode;
    unsigned char   cur_speed;
    unsigned char   cur_hue;
    unsigned char   cur_sat;
    unsigned char   cur_val;

    controller->GetMode(&cur_mode, &cur_speed, &cur_hue, &cur_sat, &cur_val);

    active_mode = cur_mode;

    if(modes[active_mode].flags & MODE_FLAG_HAS_SPEED)
    {
        modes[active_mode].speed = cur_speed;
    }

    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        hsv_t       hsv_color;
        hsv_color.hue                   = ((float)cur_hue * (360.0f / 256.0f));
        hsv_color.saturation            = cur_sat;
        hsv_color.value                 = cur_val;

        RGBColor    rgb_color           = hsv2rgb(&hsv_color);
        modes[active_mode].colors[0]    = rgb_color;
    }

    SetupZones();
}

RGBController_QMKVialRGB::~RGBController_QMKVialRGB()
{
    delete controller;
}

void RGBController_QMKVialRGB::SetupZones()
{
    /*-----------------------------------------------------*\
    | Build matrix map                                      |
    \*-----------------------------------------------------*/
    unsigned char max_col = 0;
    unsigned char max_row = 0;

    for(unsigned short led_index = 0; led_index < controller->GetLEDCount(); led_index++)
    {
        vialrgb_led_info info = controller->GetLEDInfo(led_index);

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
        vialrgb_led_info info = controller->GetLEDInfo(led_index);

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

void RGBController_QMKVialRGB::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_QMKVialRGB::DeviceUpdateLEDs()
{
    controller->SendLEDs(colors.size(), colors.data());
}

void RGBController_QMKVialRGB::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_QMKVialRGB::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_QMKVialRGB::DeviceUpdateMode()
{
    unsigned char hue = 0;
    unsigned char sat = 0;
    unsigned char val = 0;

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
