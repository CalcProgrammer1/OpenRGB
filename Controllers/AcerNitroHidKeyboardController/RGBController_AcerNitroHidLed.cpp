/*---------------------------------------------------------*\
| RGBController_AcerNitroHidLed.cpp                         |
|                                                           |
|   RGBController for Acer Nitro HID LED                    |
|                                                           |
|   Marek Pokropiński                           Jul 27 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_AcerNitroHidLed.h"

/**------------------------------------------------------------------*\
    @name Acer Nitro HID Led
    @category Laptop
    @type HID
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectAcerHIDControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_AcerNitroHidLed::RGBController_AcerNitroHidLed(std::shared_ptr<AcerNitroHidKeyboardController> controller)
{
    this->controller            = controller;
    device_id                   = 0x65;

    name                        = "Acer Nitro HID LED Device";
    vendor                      = "Acer";
    type                        = DEVICE_TYPE_LIGHT;
    description                 = "Acer Nitro HID LED Device";
    location                    = controller->GetLocation();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = ACER_HID_MODE_DIRECT;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS ;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    Direct.brightness_min       = 0;
    Direct.brightness_max       = 100;
    Direct.brightness           = 100;

    modes.push_back(Direct);

    mode Static;
    Static.name                 = "Static";
    Static.value                = ACER_HID_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR  | MODE_FLAG_HAS_BRIGHTNESS;
    Static.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Static.brightness_min       = 0;
    Static.brightness_max       = 100;
    Static.brightness           = 100;
    Static.colors_min           = 1;
    Static.colors_max           = 1;
    Static.colors.resize(1);
    Static.speed                = 0;

    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = ACER_HID_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR  | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.brightness_min    = 0;
    Breathing.brightness_max    = 100;
    Breathing.brightness        = 100;
    Breathing.colors_min        = 1;
    Breathing.colors_max        = 1;
    Breathing.speed_min         = 1;
    Breathing.speed_max         = 5;
    Breathing.speed             = 4;
    Breathing.colors.resize(1);

    modes.push_back(Breathing);

    mode Neon;
    Neon.name                   = "Neon";
    Neon.value                  = ACER_HID_MODE_NEON;
    Neon.flags                  = MODE_FLAG_REQUIRES_ENTIRE_DEVICE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Neon.color_mode             = MODE_COLORS_NONE;
    Neon.brightness_min         = 0;
    Neon.brightness_max         = 100;
    Neon.brightness             = 100;
    Neon.speed_min              = 1;
    Neon.speed_max              = 5;
    Neon.speed                  = 4;

    modes.push_back(Neon);

    SetupZones();
}

RGBController_AcerNitroHidLed::~RGBController_AcerNitroHidLed()
{
    Shutdown();
}

void RGBController_AcerNitroHidLed::SetupZones()
{
    zone* new_zone          = new zone;

    new_zone->name          = "LED";
    new_zone->type          = ZONE_TYPE_LINEAR;
    new_zone->leds_min      = 1;
    new_zone->leds_max      = 1;
    new_zone->leds_count    = 1;
    zones.push_back(*new_zone);

    led* new_led            = new led();
    new_led->name           = "LED";
    new_led->value          = (unsigned int)leds.size();
    leds.push_back(*new_led);

    SetupColors();
}

void RGBController_AcerNitroHidLed::DeviceUpdateLEDs()
{
    mode& current_mode = modes[active_mode];
    if(current_mode.value == ACER_HID_MODE_DIRECT)
    {
        controller->SetLEDs(colors, current_mode.brightness, device_id);
    }
    else
    {
        RGBColor color          = current_mode.colors.size()==0? (RGBColor)0 :  current_mode.colors[0];
        unsigned char direction = 1;
        if((modes[active_mode].flags & MODE_FLAG_HAS_DIRECTION_LR) && modes[active_mode].direction)
        {
            direction           = 2;
        }
        controller->SetAllLEDs(color, current_mode.brightness, current_mode.speed, direction, current_mode.value, device_id);
    }
}

void RGBController_AcerNitroHidLed::DeviceUpdateZoneLEDs(int)
{
    DeviceUpdateLEDs();
}

void RGBController_AcerNitroHidLed::DeviceUpdateSingleLED(int)
{
    DeviceUpdateLEDs();
}

void RGBController_AcerNitroHidLed::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
