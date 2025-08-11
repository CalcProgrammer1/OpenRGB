/*---------------------------------------------------------*\
| RGBController_HyperXPulsefireDart.cpp                     |
|                                                           |
|   RGBController for HyperX Pulsefire Dart                 |
|                                                           |
|   Santeri Pikarinen (santeri3700)             26 Dec 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_HyperXPulsefireDart.h"

/**------------------------------------------------------------------*\
    @name HyperX Pulsefire Dart
    @category Mouse
    @type USB
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectHyperXPulsefireDartControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_HyperXPulsefireDart::RGBController_HyperXPulsefireDart(HyperXPulsefireDartController* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = controller->GetNameString();
    vendor                          = "HyperX";
    type                            = DEVICE_TYPE_MOUSE;
    description                     = "HyperX Pulsefire Dart Device";
    location                        = controller->GetDeviceLocation();
    serial                          = controller->GetSerialString();

    mode Direct;
    Direct.name                     = "Direct";
    Direct.value                    = HYPERX_PULSEFIRE_DART_MODE_STATIC;
    Direct.speed                    = HYPERX_PULSEFIRE_DART_SPEED_NONE;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Direct.color_mode               = MODE_COLORS_PER_LED;
    Direct.brightness_min           = HYPERX_PULSEFIRE_DART_BRIGHTNESS_MIN;
    Direct.brightness_max           = HYPERX_PULSEFIRE_DART_BRIGHTNESS_MAX;
    Direct.brightness               = HYPERX_PULSEFIRE_DART_BRIGHTNESS_MAX;
    modes.push_back(Direct);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = HYPERX_PULSEFIRE_DART_MODE_BREATHING;
    Breathing.speed                 = HYPERX_PULSEFIRE_DART_SPEED_MED;
    Breathing.speed_min             = HYPERX_PULSEFIRE_DART_SPEED_MIN;
    Breathing.speed_max             = HYPERX_PULSEFIRE_DART_SPEED_MAX;
    Breathing.flags                 = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Breathing.color_mode            = MODE_COLORS_PER_LED;
    Breathing.brightness_min        = HYPERX_PULSEFIRE_DART_BRIGHTNESS_MIN;
    Breathing.brightness_max        = HYPERX_PULSEFIRE_DART_BRIGHTNESS_MAX;
    Breathing.brightness            = HYPERX_PULSEFIRE_DART_BRIGHTNESS_MAX;
    modes.push_back(Breathing);

    mode SpectrumCycle;
    SpectrumCycle.name              = "Spectrum Cycle";
    SpectrumCycle.value             = HYPERX_PULSEFIRE_DART_MODE_CYCLE;
    SpectrumCycle.speed             = HYPERX_PULSEFIRE_DART_SPEED_MED;
    SpectrumCycle.speed_min         = HYPERX_PULSEFIRE_DART_SPEED_MIN;
    SpectrumCycle.speed_max         = HYPERX_PULSEFIRE_DART_SPEED_MAX;
    SpectrumCycle.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    SpectrumCycle.color_mode        = MODE_COLORS_NONE;
    SpectrumCycle.brightness_min    = HYPERX_PULSEFIRE_DART_BRIGHTNESS_MIN;
    SpectrumCycle.brightness_max    = HYPERX_PULSEFIRE_DART_BRIGHTNESS_MAX;
    SpectrumCycle.brightness        = HYPERX_PULSEFIRE_DART_BRIGHTNESS_MAX;
    modes.push_back(SpectrumCycle);

    mode Reactive;
    Reactive.name                   = "Reactive";
    Reactive.value                  = HYPERX_PULSEFIRE_DART_MODE_REACTIVE;
    Reactive.speed                  = HYPERX_PULSEFIRE_DART_SPEED_MED;
    Reactive.speed_min              = HYPERX_PULSEFIRE_DART_SPEED_MIN;
    Reactive.speed_max              = HYPERX_PULSEFIRE_DART_SPEED_MAX;
    Reactive.flags                  = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Reactive.color_mode             = MODE_COLORS_PER_LED;
    Reactive.brightness_min         = HYPERX_PULSEFIRE_DART_BRIGHTNESS_MIN;
    Reactive.brightness_max         = HYPERX_PULSEFIRE_DART_BRIGHTNESS_MAX;
    Reactive.brightness             = HYPERX_PULSEFIRE_DART_BRIGHTNESS_MAX;
    modes.push_back(Reactive);

    SetupZones();
}

RGBController_HyperXPulsefireDart::~RGBController_HyperXPulsefireDart()
{

}

void RGBController_HyperXPulsefireDart::SetupZones()
{
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
    if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        controller->SendDirect(colors[led], leds[led].value, modes[active_mode].value, modes[active_mode].brightness, modes[active_mode].speed);
    }
    else
    {
        controller->SendDirect(colors[led], HYPERX_PULSEFIRE_DART_LED_ALL, modes[active_mode].value, modes[active_mode].brightness, modes[active_mode].speed);
    }
}

void RGBController_HyperXPulsefireDart::DeviceUpdateMode()
{
    if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        controller->SendDirect(colors[0], HYPERX_PULSEFIRE_DART_LED_SCROLL, modes[active_mode].value, modes[active_mode].brightness, modes[active_mode].speed);
        controller->SendDirect(colors[1], HYPERX_PULSEFIRE_DART_LED_LOGO,   modes[active_mode].value, modes[active_mode].brightness, modes[active_mode].speed);
    }
    else
    {
        controller->SendDirect(colors[0], HYPERX_PULSEFIRE_DART_LED_ALL, modes[active_mode].value, modes[active_mode].brightness, modes[active_mode].speed);
    }
}

void RGBController_HyperXPulsefireDart::DeviceSaveMode()
{
    controller->Save();
}
