/*-------------------------------------------*\
|  RGBController_HyperXPulsefireDart.cpp      |
|                                             |
|  Generic RGB Interface for HyperX           |
|  Pulsefire Dart                             |
|                                             |
|  Santeri Pikarinen (santeri3700) 12/26/2020 |
\*-------------------------------------------*/

#include "RGBController_HyperXPulsefireDart.h"

RGBController_HyperXPulsefireDart::RGBController_HyperXPulsefireDart(HyperXPulsefireDartController* hyperx_ptr)
{
    hyperx = hyperx_ptr;

    name        = "HyperX Pulsefire Dart Device";
    vendor      = "HyperX";
    type        = DEVICE_TYPE_MOUSE;
    description = "HyperX Pulsefire Dart Device";
    location    = hyperx->GetDeviceLocation();
    serial      = hyperx->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = HYPERX_PULSEFIRE_DART_MODE_STATIC;
    Direct.speed      = HYPERX_PULSEFIRE_DART_SPEED_NONE;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED | MODE_FLAG_HAS_BRIGHTNESS;
    modes.push_back(Direct);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = HYPERX_PULSEFIRE_DART_MODE_BREATHING;
    Breathing.speed      = HYPERX_PULSEFIRE_DART_SPEED_MED;
    Breathing.speed_min  = HYPERX_PULSEFIRE_DART_SPEED_MIN;
    Breathing.speed_max  = HYPERX_PULSEFIRE_DART_SPEED_MAX;
    Breathing.flags      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Breathing.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode SpectrumCycle;
    SpectrumCycle.name       = "Spectrum Cycle";
    SpectrumCycle.value      = HYPERX_PULSEFIRE_DART_MODE_CYCLE;
    SpectrumCycle.speed      = HYPERX_PULSEFIRE_DART_SPEED_MED;
    SpectrumCycle.speed_min  = HYPERX_PULSEFIRE_DART_SPEED_MIN;
    SpectrumCycle.speed_max  = HYPERX_PULSEFIRE_DART_SPEED_MAX;
    SpectrumCycle.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    SpectrumCycle.color_mode = MODE_COLORS_NONE;
    modes.push_back(SpectrumCycle);

    mode TriggerFade;
    TriggerFade.name       = "Trigger Fade";
    TriggerFade.value      = HYPERX_PULSEFIRE_DART_MODE_TRIGGER;
    TriggerFade.speed      = HYPERX_PULSEFIRE_DART_SPEED_MED;
    TriggerFade.speed_min  = HYPERX_PULSEFIRE_DART_SPEED_MIN;
    TriggerFade.speed_max  = HYPERX_PULSEFIRE_DART_SPEED_MAX;
    TriggerFade.flags      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    TriggerFade.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(TriggerFade);

    SetupZones();
}

RGBController_HyperXPulsefireDart::~RGBController_HyperXPulsefireDart()
{

}

void RGBController_HyperXPulsefireDart::SetupZones()
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
    logo_led.value          = HYPERX_PULSEFIRE_DART_LED_LOGO;
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
    scroll_led.value        = HYPERX_PULSEFIRE_DART_LED_SCROLL;
    leds.push_back(scroll_led);

    SetupColors();
}

void RGBController_HyperXPulsefireDart::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_HyperXPulsefireDart::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_HyperXPulsefireDart::UpdateZoneLEDs(int zone)
{
    UpdateSingleLED(zone);
}

void RGBController_HyperXPulsefireDart::UpdateSingleLED(int led)
{
    int brightness = 0x64;

    if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        hyperx->SendDirect(colors[led], leds[led].value, modes[active_mode].value, brightness, modes[active_mode].speed);
    }
    else
    {
        hyperx->SendDirect(colors[led], HYPERX_PULSEFIRE_DART_LED_ALL, modes[active_mode].value, brightness, modes[active_mode].speed);
    }
}

void RGBController_HyperXPulsefireDart::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_HyperXPulsefireDart::DeviceUpdateMode()
{
    int brightness = 0x64;

    if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        hyperx->SendDirect(colors[0], HYPERX_PULSEFIRE_DART_LED_SCROLL, modes[active_mode].value, brightness, modes[active_mode].speed);
        hyperx->SendDirect(colors[1], HYPERX_PULSEFIRE_DART_LED_LOGO,   modes[active_mode].value, brightness, modes[active_mode].speed);
    }
    else
    {
        hyperx->SendDirect(colors[0], HYPERX_PULSEFIRE_DART_LED_ALL, modes[active_mode].value, brightness, modes[active_mode].speed);
    }
}
