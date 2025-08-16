/*---------------------------------------------------------*\
| RGBController_SteelSeriesRival3.cpp                       |
|                                                           |
|   RGBController for SteelSeries Rival 3                   |
|                                                           |
|   B Horn (bahorn)                             29 Aug 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_SteelSeriesRival3.h"

/**------------------------------------------------------------------*\
    @name Steel Series Rival 3
    @category Mouse
    @type USB
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectSteelSeriesRival3,DetectSteelSeriesAerox3,DetectSteelSeriesAerox5,DetectSteelSeriesAerox9
    @comment
\*-------------------------------------------------------------------*/

RGBController_SteelSeriesRival3::RGBController_SteelSeriesRival3(SteelSeriesMouseController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetNameString();
    vendor                  = "SteelSeries";
    type                    = DEVICE_TYPE_MOUSE;
    description             = "SteelSeries Mouse Device";
    location                = controller->GetDeviceLocation();
    serial                  = controller->GetSerialString();
    version                 = controller->GetFirmwareVersion();

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = STEELSERIES_MOUSE_EFFECT_DIRECT;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    Direct.brightness_min   = 0x00;
    Direct.brightness_max   = STEELSERIES_MOUSE_BRIGHTNESS_MAX;
    Direct.brightness       = STEELSERIES_MOUSE_BRIGHTNESS_MAX;

    mode Breathing;
    Breathing.name          = "Breathing";
    Breathing.value         = STEELSERIES_MOUSE_EFFECT_BREATHING_MIN;
    Breathing.flags         = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE;
    Breathing.color_mode    = MODE_COLORS_PER_LED;
    Breathing.speed_min     = 0;
    Breathing.speed_max     = 2;
    Breathing.speed         = 1;

    mode SpectrumCycle;
    SpectrumCycle.name      = "Spectrum Cycle";
    SpectrumCycle.value     = STEELSERIES_MOUSE_EFFECT_SPECTRUM_CYCLE;
    SpectrumCycle.flags     = MODE_FLAG_MANUAL_SAVE;

    mode RainbowBreathing;
    RainbowBreathing.name   = "Rainbow Breathing";
    RainbowBreathing.value  = STEELSERIES_MOUSE_EFFECT_RAINBOW_BREATHING;
    RainbowBreathing.flags  = MODE_FLAG_MANUAL_SAVE;

    /*------------------------------------------------------------------------*\
    | This is a pretty cool mode where it flashes random colors.               |
    |                                                                          |
    | However, the flashes are in the frequency range where it probably needs  |
    | a proper warning for it to be compiled in by default.                    |
    |                                                                          |
    | It is disabled in the vendor software, and is only known about as it is  |
    | documented in rivalcfg.                                                  |
    |                                                                          |
    | If this does get re-enabled, worth noting it has an issue where this     |
    | mode is black if you come directly from one of the pulsating modes.      |
    \*------------------------------------------------------------------------*/
    /*
    mode Disco;
    Disco.name              = "Disco";
    Disco.value             = STEELSERIES_MOUSE_EFFECT_DISCO;
    Disco.flags             = MODE_FLAG_MANUAL_SAVE;
    modes.push_back(Disco);
    */

    steelseries_mouse mouse = controller->GetMouse();

    for(const uint8_t i: mouse.modes)
    {
        switch(i)
        {
            case STEELSERIES_MOUSE_EFFECT_SPECTRUM_CYCLE:
                modes.push_back(SpectrumCycle);
                break;
            case STEELSERIES_MOUSE_EFFECT_BREATHING_MIN:
                modes.push_back(Breathing);
                break;
            case STEELSERIES_MOUSE_EFFECT_DIRECT:
                modes.push_back(Direct);
                break;
            case STEELSERIES_MOUSE_EFFECT_RAINBOW_BREATHING:
                modes.push_back(RainbowBreathing);
                break;
        }
    }

    SetupZones();
}

void RGBController_SteelSeriesRival3::DeviceSaveMode()
{
    controller->Save();
}

RGBController_SteelSeriesRival3::~RGBController_SteelSeriesRival3()
{
    delete controller;
}

void RGBController_SteelSeriesRival3::SetupZones()
{
    steelseries_mouse mouse = controller->GetMouse();

    for(const led_info info: mouse.leds)
    {
        zone zone;
        zone.name          = info.name;
        zone.type          = ZONE_TYPE_SINGLE;
        zone.leds_min      = 1;
        zone.leds_max      = 1;
        zone.leds_count    = 1;
        zone.matrix_map    = NULL;
        zones.push_back(zone);

        led mouse_led;
        mouse_led.name          = info.name;
        mouse_led.value         = info.value;
        leds.push_back(mouse_led);
    }
    SetupColors();
}

void RGBController_SteelSeriesRival3::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_SteelSeriesRival3::DeviceUpdateLEDs()
{
    for(unsigned int i = 0; i < zones.size(); i++)
    {
        UpdateZoneLEDs(i);
    }
    DeviceUpdateMode();
}

void RGBController_SteelSeriesRival3::UpdateZoneLEDs(int zone)
{
    UpdateSingleLED(zone);
}

void RGBController_SteelSeriesRival3::UpdateSingleLED(int led)
{
    unsigned char red = RGBGetRValue(colors[led]);
    unsigned char grn = RGBGetGValue(colors[led]);
    unsigned char blu = RGBGetBValue(colors[led]);
    controller->SetColor(leds[led].value, red, grn, blu, modes[active_mode].brightness);
}

void RGBController_SteelSeriesRival3::DeviceUpdateMode()
{
    controller->SetLightEffectAll(modes[active_mode].value - modes[active_mode].speed);
}
