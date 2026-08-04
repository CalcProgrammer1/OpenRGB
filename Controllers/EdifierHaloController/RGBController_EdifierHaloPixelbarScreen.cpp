/*---------------------------------------------------------*\
| RGBController_EdifierHaloPixelbarScreen.cpp               |
|                                                           |
|   RGBController for Edifier Halo PixelBar Screen Color    |
|                                                           |
|   Bartholomew Ho <qiangqiang101@hotmail.com>  20 Jul 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_EdifierHaloPixelbarScreen.h"

/**------------------------------------------------------------------*\
    @name Edifier Halo Pixelbar - Screen
    @category Speaker
    @type USB
    @save :robot:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectEdifierHaloControllers
    @comment
\*-------------------------------------------------------------------*/

struct CategoryDefinition
{
    std::string name;
    unsigned char category_id;
};

RGBController_EdifierHaloPixelbarScreen::RGBController_EdifierHaloPixelbarScreen(EdifierHaloController* controller_ptr)
{
    controller                = controller_ptr;
    first_leds_update_ignored = false;
    first_mode_update_ignored = false;

    name                = "Edifier Halo Pixelbar - Pixel Display";
    vendor              = "Edifier";
    type                = DEVICE_TYPE_SPEAKER;
    description         = "Edifier Halo PixelBar Pixel Display Color";
    serial              = controller->GetSerial();
    location            = controller->GetLocation();

    // Clock 1 to Clock 11 (11 built-in clock styles)
    for(unsigned char i = 0; i < 11; ++i)
    {
        mode m;
        m.name           = "Clock " + std::to_string(i + 1);
        m.value          = (0 << 8) | i;
        m.flags          = MODE_FLAG_HAS_PER_LED_COLOR;
        m.color_mode     = MODE_COLORS_PER_LED;
        modes.push_back(m);
    }

    // Default built-in layouts for other categories
    std::vector<CategoryDefinition> categories =
    {
        { "Gaming",     1 },
        { "Work",       2 },
        { "Study",      3 },
        { "Cats",       4 },
        { "Dogs",       5 },
        { "Buzzwords",  6 },
        { "Cyberpunk",  7 },
        { "Spectrum",   8 }
    };

    for(const CategoryDefinition& cat : categories)
    {
        mode m;
        m.name           = cat.name;
        m.value          = (cat.category_id << 8) | 0;
        m.flags          = MODE_FLAG_HAS_PER_LED_COLOR;
        m.color_mode     = MODE_COLORS_PER_LED;
        modes.push_back(m);
    }

    // Default to Clock 1 mode
    active_mode = 0;

    SetupZones();
}

RGBController_EdifierHaloPixelbarScreen::~RGBController_EdifierHaloPixelbarScreen()
{
    delete controller;
}

void RGBController_EdifierHaloPixelbarScreen::SetupZones()
{
    zone screen_zone;
    screen_zone.name       = "Screen Color";
    screen_zone.type       = ZONE_TYPE_SINGLE;
    screen_zone.leds_min   = 1;
    screen_zone.leds_max   = 1;
    screen_zone.leds_count = 1;
    zones.push_back(screen_zone);

    led screen_led;
    screen_led.name        = "Screen Text";
    leds.push_back(screen_led);

    SetupColors();
}

void RGBController_EdifierHaloPixelbarScreen::DeviceUpdateLEDs()
{
    if(!first_leds_update_ignored)
    {
        first_leds_update_ignored = true;
        return;
    }

    unsigned char r = 255;
    unsigned char g = 255;
    unsigned char b = 255;

    if(!colors.empty())
    {
        RGBColor color = colors[0];
        r = RGBGetRValue(color);
        g = RGBGetGValue(color);
        b = RGBGetBValue(color);
    }

    // For all other modes, color updates are sent using SetPixelScreenColor
    controller->SetPixelScreenColor(r, g, b);
}

void RGBController_EdifierHaloPixelbarScreen::DeviceUpdateZoneLEDs(int)
{
    DeviceUpdateLEDs();
}

void RGBController_EdifierHaloPixelbarScreen::DeviceUpdateSingleLED(int)
{
    DeviceUpdateLEDs();
}

void RGBController_EdifierHaloPixelbarScreen::DeviceUpdateMode()
{
    if(!first_mode_update_ignored)
    {
        first_mode_update_ignored = true;
        return;
    }

    int active_idx = GetActiveMode();
    unsigned char r = 255;
    unsigned char g = 255;
    unsigned char b = 255;

    if(!colors.empty())
    {
        RGBColor color = colors[0];
        r = RGBGetRValue(color);
        g = RGBGetGValue(color);
        b = RGBGetBValue(color);
    }

    unsigned int mode_val = modes[active_idx].value;
    unsigned char category = (mode_val >> 8) & 0xFF;
    unsigned char index    = mode_val & 0xFF;

    if(category == 0) // Clock category (11 built-in styles)
    {
        controller->SetScreenScene(0, index, r, g, b);
    }
    else // Other categories (Gaming, Working, etc.) use the default built-in layouts
    {
        controller->SetScreenUIModel(category, r, g, b);
    }
}
