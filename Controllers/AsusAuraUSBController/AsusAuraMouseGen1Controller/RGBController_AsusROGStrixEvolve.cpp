/*---------------------------------------------------------*\
| RGBController_AsusROGStrixEvolve.cpp                      |
|                                                           |
|   RGBController for ASUS ROG Evolve                       |
|                                                           |
|   Mola19                                      30 Nov 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_AsusROGStrixEvolve.h"

/**------------------------------------------------------------------*\
    @name Asus Aura Strix Evolve
    @category Mouse
    @type USB
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectAsusAuraUSBStrixEvolve
    @comment
\*-------------------------------------------------------------------*/

RGBController_AsusROGStrixEvolve::RGBController_AsusROGStrixEvolve(AsusAuraMouseGen1Controller* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetName();
    vendor                      = "ASUS";
    type                        = DEVICE_TYPE_MOUSE;
    description                 = "ASUS Aura Mouse Device";
    version                     = controller->GetVersion();
    location                    = controller->GetDeviceLocation();
    serial                      = controller->GetSerialString();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = ASUS_ROG_STRIX_EVOLVE_MODE_DIRECT;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.brightness_min       = ASUS_ROG_STRIX_EVOLVE_BRIGHTNESS_MIN;
    Direct.brightness_max       = ASUS_ROG_STRIX_EVOLVE_BRIGHTNESS_MAX;
    Direct.brightness           = ASUS_ROG_STRIX_EVOLVE_BRIGHTNESS_DEFAULT;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = ASUS_ROG_STRIX_EVOLVE_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Breathing.brightness_min    = ASUS_ROG_STRIX_EVOLVE_BRIGHTNESS_MIN;
    Breathing.brightness_max    = ASUS_ROG_STRIX_EVOLVE_BRIGHTNESS_MAX;
    Breathing.brightness        = ASUS_ROG_STRIX_EVOLVE_BRIGHTNESS_DEFAULT;
    Breathing.color_mode        = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode ColorCycle;
    ColorCycle.name             = "Spectrum Cycle";
    ColorCycle.value            = ASUS_ROG_STRIX_EVOLVE_MODE_SPECTRUM_CYCLE;
    ColorCycle.flags            = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    ColorCycle.brightness_min   = ASUS_ROG_STRIX_EVOLVE_BRIGHTNESS_MIN;
    ColorCycle.brightness_max   = ASUS_ROG_STRIX_EVOLVE_BRIGHTNESS_MAX;
    ColorCycle.brightness       = ASUS_ROG_STRIX_EVOLVE_BRIGHTNESS_DEFAULT;
    ColorCycle.color_mode       = MODE_COLORS_NONE;
    modes.push_back(ColorCycle);

    mode Reactive;
    Reactive.name               = "Reactive";
    Reactive.value              = ASUS_ROG_STRIX_EVOLVE_MODE_REACTIVE;
    Reactive.flags              = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Reactive.brightness_min     = ASUS_ROG_STRIX_EVOLVE_BRIGHTNESS_MIN;
    Reactive.brightness_max     = ASUS_ROG_STRIX_EVOLVE_BRIGHTNESS_MAX;
    Reactive.brightness         = ASUS_ROG_STRIX_EVOLVE_BRIGHTNESS_DEFAULT;
    Reactive.color_mode         = MODE_COLORS_PER_LED;
    modes.push_back(Reactive);

    SetupZones();
}

RGBController_AsusROGStrixEvolve::~RGBController_AsusROGStrixEvolve()
{
    delete controller;
}

void RGBController_AsusROGStrixEvolve::SetupZones()
{
    zone mouse_zone;

    mouse_zone.name         = "Underglow";
    mouse_zone.type         = ZONE_TYPE_SINGLE;
    mouse_zone.leds_min     = 1;
    mouse_zone.leds_max     = 1;
    mouse_zone.leds_count   = 1;
    mouse_zone.matrix_map   = NULL;

    zones.push_back(mouse_zone);

    led mouse_led;

    mouse_led.name          = "Underglow";
    mouse_led.value         = 1;

    leds.push_back(mouse_led);

    SetupColors();
}

void RGBController_AsusROGStrixEvolve::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_AsusROGStrixEvolve::DeviceUpdateLEDs()
{
    UpdateSingleLED(0);
}

void RGBController_AsusROGStrixEvolve::UpdateZoneLEDs(int zone)
{
    UpdateSingleLED(zone);
}

void RGBController_AsusROGStrixEvolve::UpdateSingleLED(int /*led*/)
{
    controller->SendUpdate(0x1C, RGBGetRValue(colors[0]));
    controller->SendUpdate(0x1D, RGBGetGValue(colors[0]));
    controller->SendUpdate(0x1E, RGBGetBValue(colors[0]));
}

void RGBController_AsusROGStrixEvolve::DeviceUpdateMode()
{
    controller->SendUpdate(0x19, modes[active_mode].value);
    controller->SendUpdate(0x1A, modes[active_mode].brightness);
}

void RGBController_AsusROGStrixEvolve::DeviceSaveMode()
{
    unsigned int profile = controller->GetActiveProfile();


    controller->UpdateProfile(0x19, profile, modes[active_mode].value);
    controller->UpdateProfile(0x1A, profile, modes[active_mode].brightness);

    if(modes[active_mode].value != ASUS_ROG_STRIX_EVOLVE_MODE_SPECTRUM_CYCLE)
    {
        controller->UpdateProfile(0x1C, profile, RGBGetRValue(colors[0]));
        controller->UpdateProfile(0x1D, profile, RGBGetGValue(colors[0]));
        controller->UpdateProfile(0x1E, profile, RGBGetBValue(colors[0]));
    }

    controller->ResetToSavedLighting();
}
