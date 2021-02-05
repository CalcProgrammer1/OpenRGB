/*-----------------------------------------*\
|  RGBController_AsusAuraMouse.cpp          |
|                                           |
|  Generic RGB Interface for Asus Aura      |
|  USB controller driver                    |
|                                           |
|  Adam Honse (CalcProgrammer1) 10/25/2020  |
\*-----------------------------------------*/

#include "RGBController_AsusAuraMouse.h"

RGBController_AuraMouse::RGBController_AuraMouse(AuraMouseController* aura_ptr)
{
    aura = aura_ptr;

    name        = "ASUS Aura Mouse";
    vendor      = "ASUS";
    type        = DEVICE_TYPE_MOUSE;
    description = "ASUS Aura Mouse Device";
    location    = aura->GetDeviceLocation();
    serial      = aura->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = AURA_MOUSE_MODE_STATIC;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = AURA_MOUSE_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode SpectrumCycle;
    SpectrumCycle.name       = "Spectrum Cycle";
    SpectrumCycle.value      = AURA_MOUSE_MODE_COLOR_CYCLE;
    SpectrumCycle.flags      = 0;
    SpectrumCycle.color_mode = MODE_COLORS_NONE;
    modes.push_back(SpectrumCycle);

    mode Reactive;
    Reactive.name       = "Reactive";
    Reactive.value      = AURA_MOUSE_MODE_REACTIVE;
    Reactive.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Reactive.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Reactive);

    SetupZones();
}

RGBController_AuraMouse::~RGBController_AuraMouse()
{
    delete aura;
}

void RGBController_AuraMouse::SetupZones()
{
    zone logo_zone;

    logo_zone.name          = "Logo";
    logo_zone.type          = ZONE_TYPE_SINGLE;
    logo_zone.leds_min      = 1;
    logo_zone.leds_max      = 1;
    logo_zone.leds_count    = 1;
    logo_zone.matrix_map    = NULL;

    zones.push_back(logo_zone);

    led logo_led;

    logo_led.name           = "Logo";

    leds.push_back(logo_led);

    zone scroll_zone;

    scroll_zone.name        = "Scroll Wheel";
    scroll_zone.type        = ZONE_TYPE_SINGLE;
    scroll_zone.leds_min    = 1;
    scroll_zone.leds_max    = 1;
    scroll_zone.leds_count  = 1;
    scroll_zone.matrix_map  = NULL;

    zones.push_back(scroll_zone);

    led scroll_led;

    scroll_led.name         = "Scroll Wheel";

    leds.push_back(scroll_led);

    zone underglow_zone;

    underglow_zone.name       = "Underglow";
    underglow_zone.type       = ZONE_TYPE_SINGLE;
    underglow_zone.leds_min   = 1;
    underglow_zone.leds_max   = 1;
    underglow_zone.leds_count = 1;
    underglow_zone.matrix_map = NULL;

    zones.push_back(underglow_zone);

    led underglow_led;

    underglow_led.name        = "Underglow";

    leds.push_back(underglow_led);

    SetupColors();
}

void RGBController_AuraMouse::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_AuraMouse::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_AuraMouse::UpdateZoneLEDs(int zone)
{
    UpdateSingleLED(zone);
}

void RGBController_AuraMouse::UpdateSingleLED(int led)
{
    unsigned char red = 0;
    unsigned char grn = 0;
    unsigned char blu = 0;

    if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        if(led == 0)
        {
            red = RGBGetRValue(colors[led]);
            grn = RGBGetGValue(colors[led]);
            blu = RGBGetBValue(colors[led]);

            aura->SendUpdate(AURA_MOUSE_ZONE_LOGO, modes[active_mode].value, red, grn, blu);
        }
        else if(led == 1)
        {
            red = RGBGetRValue(colors[led]);
            grn = RGBGetGValue(colors[led]);
            blu = RGBGetBValue(colors[led]);

            aura->SendUpdate(AURA_MOUSE_ZONE_SCROLL, modes[active_mode].value, red, grn, blu);
        }
        else
        {
            red = RGBGetRValue(colors[led]);
            grn = RGBGetGValue(colors[led]);
            blu = RGBGetBValue(colors[led]);

            aura->SendUpdate(AURA_MOUSE_ZONE_UNDERGLOW, modes[active_mode].value, red, grn, blu);
        }
    }
    else
    {
        aura->SendUpdate(AURA_MOUSE_ZONE_ALL, modes[active_mode].value, red, grn, blu);
    }
}

void RGBController_AuraMouse::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_AuraMouse::DeviceUpdateMode()
{
    unsigned char red = 0;
    unsigned char grn = 0;
    unsigned char blu = 0;

    if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        red = RGBGetRValue(colors[0]);
        grn = RGBGetGValue(colors[0]);
        blu = RGBGetBValue(colors[0]);

        aura->SendUpdate(AURA_MOUSE_ZONE_LOGO, modes[active_mode].value, red, grn, blu);

        red = RGBGetRValue(colors[1]);
        grn = RGBGetGValue(colors[1]);
        blu = RGBGetBValue(colors[1]);

        aura->SendUpdate(AURA_MOUSE_ZONE_SCROLL, modes[active_mode].value, red, grn, blu);

        red = RGBGetRValue(colors[2]);
        grn = RGBGetGValue(colors[2]);
        blu = RGBGetBValue(colors[2]);

        aura->SendUpdate(AURA_MOUSE_ZONE_UNDERGLOW, modes[active_mode].value, red, grn, blu);
    }
    else
    {
        aura->SendUpdate(AURA_MOUSE_ZONE_ALL, modes[active_mode].value, red, grn, blu);
    }
}
