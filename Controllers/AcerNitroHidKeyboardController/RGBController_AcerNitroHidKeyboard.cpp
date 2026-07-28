/*---------------------------------------------------------*\
| RGBController_AcerNitroHidKeyboard.cpp                    |
|                                                           |
|   RGBController for Acer Nitro HID Keyboard               |
|                                                           |
|   Marek Pokropiński                           Jul 27 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_AcerNitroHidKeyboard.h"

// /**------------------------------------------------------------------*\
//     @name Acer Nitro HID Keyboard
//     @category Laptop
//     @type HID
//     @save :x:
//     @direct :white_check_mark:
//     @effects :white_check_mark:
//     @detectors DetectAcerHIDControllers
//     @comment
// \*-------------------------------------------------------------------*/

RGBController_AcerNitroHidKeyboard::RGBController_AcerNitroHidKeyboard(std::shared_ptr<AcerNitroHidKeyboardController> controller)
{
    this->controller            = controller;
    device_id                   = 0x21;

    name                        = "Acer Nitro HID Keyboard";
    vendor                      = "Acer";
    type                        = DEVICE_TYPE_LAPTOP;
    description                 = "Acer Nitro HID Keyboard Device";
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
    Breathing.speed_max         = 9;
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
    Neon.speed_max              = 9;
    Neon.speed                  = 4;  

    modes.push_back(Neon);

    mode Wave;
    Wave.name                   = "Wave";
    Wave.value                  = ACER_HID_MODE_WAVE;
    Wave.flags                  = MODE_FLAG_REQUIRES_ENTIRE_DEVICE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Wave.color_mode             = MODE_COLORS_NONE;
    Wave.brightness_min         = 0;
    Wave.brightness_max         = 100;
    Wave.brightness             = 100;
    Wave.speed_min              = 1;
    Wave.speed_max              = 9;
    Wave.speed                  = 4;
    Wave.direction              = 0;

    modes.push_back(Wave);
    
    mode Shifting;
    Shifting.name               = "Shifting";
    Shifting.value              = ACER_HID_MODE_SHIFTING;
    Shifting.flags              = MODE_FLAG_REQUIRES_ENTIRE_DEVICE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Shifting.color_mode         = MODE_COLORS_NONE;
    Shifting.brightness_min     = 0;
    Shifting.brightness_max     = 100;
    Shifting.brightness         = 100;
    Shifting.speed_min          = 1;
    Shifting.speed_max          = 9;
    Shifting.speed              = 4;

    modes.push_back(Shifting);

    mode Zoom;
    Zoom.name                   = "Zoom";
    Zoom.value                  = ACER_HID_MODE_ZOOM;
    Zoom.flags                  = MODE_FLAG_REQUIRES_ENTIRE_DEVICE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Zoom.color_mode             = MODE_COLORS_NONE;
    Zoom.brightness_min         = 0;
    Zoom.brightness_max         = 100;
    Zoom.brightness             = 100;
    Zoom.speed_min              = 1;
    Zoom.speed_max              = 9;
    Zoom.speed                  = 4;

    modes.push_back(Zoom);

    mode Meteor;
    Meteor.name                 = "Meteor";
    Meteor.value                = ACER_HID_MODE_METEOR;
    Meteor.flags                = MODE_FLAG_REQUIRES_ENTIRE_DEVICE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Meteor.color_mode           = MODE_COLORS_NONE;
    Meteor.brightness_min       = 0;
    Meteor.brightness_max       = 100;
    Meteor.brightness           = 100;
    Meteor.speed_min            = 1;
    Meteor.speed_max            = 9;
    Meteor.speed                = 4;

    modes.push_back(Meteor);

    mode Twinkling;
    Twinkling.name              = "Twinkling";
    Twinkling.value             = ACER_HID_MODE_TWINKLING;
    Twinkling.flags             = MODE_FLAG_REQUIRES_ENTIRE_DEVICE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Twinkling.color_mode        = MODE_COLORS_NONE;
    Twinkling.brightness_min    = 0;
    Twinkling.brightness_max    = 100;
    Twinkling.brightness        = 100;
    Twinkling.colors_min        = 1;
    Twinkling.colors_max        = 1;
    Twinkling.speed_min         = 1;
    Twinkling.speed_max         = 9;
    Twinkling.speed             = 4;

    modes.push_back(Twinkling);

    SetupZones();
}

RGBController_AcerNitroHidKeyboard::~RGBController_AcerNitroHidKeyboard()
{
    Shutdown();
}

void RGBController_AcerNitroHidKeyboard::SetupZones()
{
    zone* new_zone          = new zone;

    new_zone->name          = "Keyboard Backlight Zone";
    new_zone->type          = ZONE_TYPE_LINEAR;
    new_zone->leds_min      = 4;
    new_zone->leds_max      = 4;
    new_zone->leds_count    = 4;

    zones.push_back(*new_zone);

    for(std::size_t zone_idx = 0; zone_idx < ACER_HID_NUM_ZONES; zone_idx++)
    {
        led* new_led        = new led();
        new_led->name       = "Keyboard Backlight Zone ";
        new_led->name.append(std::to_string(zone_idx + 1));
        new_led->value      = (unsigned int)leds.size();

        leds.push_back(*new_led);
    }

    SetupColors();
}

void RGBController_AcerNitroHidKeyboard::DeviceUpdateLEDs()
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

void RGBController_AcerNitroHidKeyboard::DeviceUpdateZoneLEDs(int)
{
    DeviceUpdateLEDs();
}

void RGBController_AcerNitroHidKeyboard::DeviceUpdateSingleLED(int)
{
    DeviceUpdateLEDs();
}

void RGBController_AcerNitroHidKeyboard::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
