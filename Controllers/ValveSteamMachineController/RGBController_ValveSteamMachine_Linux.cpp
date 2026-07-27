/*---------------------------------------------------------*\
| RGBController_ValveSteamMachine_Linux.cpp                 |
|                                                           |
|   RGBController for Valve Steam Machine LEDs              |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      23 Jul 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_ValveSteamMachine_Linux.h"
#include <cctype>

/**------------------------------------------------------------------*\
    @name Valve Steam Machine LEDs
    @category LEDStrip
    @type
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectValveSteamMachineControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_ValveSteamMachine::RGBController_ValveSteamMachine(ValveSteamMachineController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetName();
    vendor                      = "Valve";
    type                        = DEVICE_TYPE_LEDSTRIP;
    description                 = "Valve Steam Machine Device";
    location                    = controller->GetLEDPath(0);

    /*-----------------------------------------------------*\
    | If the device supports hardware effects, add them as  |
    | modes.  Effects are stored as strings; the mode value |
    | is the index into the available effects list.         |
    \*-----------------------------------------------------*/
    effects                     = controller->GetAvailableEffects();

    for(unsigned int effect_idx = 0; effect_idx < effects.size(); effect_idx++)
    {
        unsigned int effect_color_mode;
        unsigned int effect_flags;
        std::string effect_name = effects[effect_idx];

        /*-------------------------------------------------*\
        | Set active_mode if this effect is active          |
        \*-------------------------------------------------*/
        if(effect_name == controller->GetEffect())
        {
            active_mode         = effect_idx;
        }

        /*-------------------------------------------------*\
        | Determine flags and color mode for effect         |
        \*-------------------------------------------------*/
        if(effect_name == "off")
        {
            effect_flags        = 0;
            effect_color_mode   = MODE_COLORS_NONE;
        }
        else if(effect_name == "rainbow" || effect_name == "demo")
        {
            effect_flags        = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
            effect_color_mode   = MODE_COLORS_RANDOM;
        }
        else
        {
            effect_flags        = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
            effect_color_mode   = MODE_COLORS_PER_LED;
        }

        if(effect_name == "breath" || effect_name == "patrol" || effect_name == "rainbow" || effect_name == "demo")
        {
            effect_flags       |= MODE_FLAG_HAS_SPEED;
        }

        /*---------------------------------------------*\
        | The OpenRGB common name for manual mode is    |
        | Direct                                        |
        \*---------------------------------------------*/
        if(effect_name == "manual")
        {
            effect_name         = "Direct";
        }
        else
        {
            if(!effect_name.empty())
            {
                effect_name[0]  = std::toupper((unsigned char)effect_name[0]);
            }
        }

        mode new_mode;
        new_mode.name           = effect_name;
        new_mode.value          = effect_idx;
        new_mode.flags          = effect_flags;
        new_mode.color_mode     = effect_color_mode;
        new_mode.speed_min      = 20;
        new_mode.speed_max      = 0;
        new_mode.speed          = controller->GetDelay();
        new_mode.brightness_min = 0;
        new_mode.brightness_max = 0xFF;
        new_mode.brightness     = controller->GetBrightness();
        modes.push_back(new_mode);
    }

    SetupZones();
}

RGBController_ValveSteamMachine::~RGBController_ValveSteamMachine()
{
    Shutdown();

    delete controller;
}

void RGBController_ValveSteamMachine::SetupZones()
{
    /*-----------------------------------------------------*\
    | Create a single linear zone containing all LEDs       |
    \*-----------------------------------------------------*/
    zone led_zone;
    led_zone.name       = "Steam Machine LEDs";
    led_zone.type       = ZONE_TYPE_LINEAR;
    led_zone.leds_min   = (unsigned int)controller->GetLEDCount();
    led_zone.leds_max   = (unsigned int)controller->GetLEDCount();
    led_zone.leds_count = (unsigned int)controller->GetLEDCount();

    zones.push_back(led_zone);

    /*-----------------------------------------------------*\
    | Create LED entries for each LED in the controller     |
    \*-----------------------------------------------------*/
    for(unsigned int led_idx = 0; led_idx < controller->GetLEDCount(); led_idx++)
    {
        led new_led;
        new_led.name    = "Steam Machine LED " + std::to_string(led_idx);
        new_led.value   = led_idx;

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_ValveSteamMachine::DeviceUpdateLEDs()
{
    for(unsigned int led_idx = 0; led_idx < controller->GetLEDCount(); led_idx++)
    {
        DeviceUpdateSingleLED(led_idx);
    }
}

void RGBController_ValveSteamMachine::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_ValveSteamMachine::DeviceUpdateSingleLED(int led)
{
    unsigned char red = RGBGetRValue(colors[led]);
    unsigned char grn = RGBGetGValue(colors[led]);
    unsigned char blu = RGBGetBValue(colors[led]);

    controller->SetLEDColor(led, red, grn, blu);
}

void RGBController_ValveSteamMachine::DeviceUpdateMode()
{
    if((unsigned int)active_mode < effects.size())
    {
        controller->SetEffect(effects[active_mode]);
        controller->SetBrightness(modes[active_mode].brightness);
        controller->SetDelay(modes[active_mode].speed);
    }
}
