/*-----------------------------------------*\
|  RGBController_AuraCore.cpp               |
|                                           |
|  Generic RGB Interface for ROG Aura Core  |
|                                           |
|  Adam Honse (CalcProgrammer1) 4/17/2020   |
\*-----------------------------------------*/

#include "RGBController_AuraCore.h"

RGBController_AuraCore::RGBController_AuraCore(AuraCoreController* aura_ptr)
{
    aura = aura_ptr;

    name        = "ASUS Aura Core";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "ASUS Aura Core Device";

    mode Static;
    Static.name       = "Static";
    Static.value      = AURA_CORE_MODE_STATIC;
    Static.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = AURA_CORE_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode ColorCycle;
    ColorCycle.name       = "Color Cycle";
    ColorCycle.value      = AURA_CORE_MODE_SPECTRUM_CYCLE;
    ColorCycle.flags      = 0;
    ColorCycle.color_mode = MODE_COLORS_NONE;
    modes.push_back(ColorCycle);

    SetupZones();
}

void RGBController_AuraCore::SetupZones()
{
    zone Keyboard;
    Keyboard.name       = "Keyboard";
    Keyboard.type       = ZONE_TYPE_SINGLE;
    Keyboard.leds_min   = 4;
    Keyboard.leds_max   = 4;
    Keyboard.leds_count = 4;
    Keyboard.matrix_map = NULL;
    zones.push_back(Keyboard);

    for(int led_idx = 0; led_idx < Keyboard.leds_count; led_idx++)
    {
        led KeyLED;
        KeyLED.name = "Keyboard LED ";
        KeyLED.name.append(std::to_string(led_idx + 1));
        leds.push_back(KeyLED);
    }

    SetupColors();
}

void RGBController_AuraCore::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AuraCore::DeviceUpdateLEDs()
{
    UpdateZoneLEDs(0);
}

void RGBController_AuraCore::UpdateZoneLEDs(int /*zone*/)
{
    for(int led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        UpdateSingleLED(led_idx);
    }
}

void RGBController_AuraCore::UpdateSingleLED(int led)
{
    aura->SendUpdate
            (
            led + 1,
            modes[active_mode].value,
            AURA_CORE_SPEED_NORMAL,
            RGBGetRValue(colors[led]),
            RGBGetGValue(colors[led]),
            RGBGetBValue(colors[led])
            );
    aura->SendSet();
    aura->SendApply();
}

void RGBController_AuraCore::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_AuraCore::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
