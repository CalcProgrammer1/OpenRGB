/*---------------------------------------------------------*\
| RGBController_Alienware.cpp                               |
|                                                           |
|   RGBController for Dell Alienware RGB USB controller     |
|                                                           |
|   Gabriel Marcano (gemarcano)                 19 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <thread>
#include <chrono>
#include "RGBController_Alienware.h"

/**------------------------------------------------------------------*\
    @name Alienware
    @category Keyboard
    @type USB
    @save :x:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectAlienwareControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_Alienware::RGBController_Alienware(AlienwareController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetDeviceName();
    vendor                      = "Alienware";
    type                        = DEVICE_TYPE_KEYBOARD;
    description                 = "Alienware USB Device";
    location                    = controller->GetDeviceLocation();
    serial                      = controller->GetSerialString();
    version                     = controller->GetFirmwareVersion();

    mode Color;
    Color.name                 = "Static";
    Color.value                = ALIENWARE_MODE_COLOR;
    Color.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Color.color_mode           = MODE_COLORS_PER_LED;
    Color.colors_min           = 1;
    Color.colors_max           = 1;
    Color.brightness_min       = 100;
    Color.brightness_max       = 0;
    Color.brightness           = 0;
    modes.push_back(Color);

    mode Pulse;
    Pulse.name                 = "Flashing";
    Pulse.value                = ALIENWARE_MODE_PULSE;
    Pulse.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Pulse.color_mode           = MODE_COLORS_PER_LED;
    Pulse.colors_min           = 1;
    Pulse.colors_max           = 1;
    Pulse.speed_min            = ALIENWARE_TEMPO_MIN;
    Pulse.speed_max            = ALIENWARE_TEMPO_MAX;
    Pulse.speed                = ALIENWARE_TEMPO_MIN;
    Pulse.brightness_min       = 100;
    Pulse.brightness_max       = 0;
    Pulse.brightness           = 0;
    modes.push_back(Pulse);

    mode Morph;
    Morph.name                 = "Morph";
    Morph.value                = ALIENWARE_MODE_MORPH;
    Morph.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Morph.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Morph.colors_min           = 2 * controller->GetZoneCount();
    Morph.colors_max           = Morph.colors_min;
    Morph.colors.resize(Morph.colors_max);
    Morph.speed_min            = ALIENWARE_TEMPO_MIN;
    Morph.speed_max            = ALIENWARE_TEMPO_MAX;
    Morph.speed                = ALIENWARE_TEMPO_MIN;
    Morph.brightness_min       = 100;
    Morph.brightness_max       = 0;
    Morph.brightness           = 0;
    modes.push_back(Morph);

    mode Spectrum;
    Spectrum.name              = "Spectrum Cycle";
    Spectrum.value             = ALIENWARE_MODE_SPECTRUM;
    Spectrum.flags             = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Spectrum.color_mode        = MODE_COLORS_NONE;
    Spectrum.speed_min         = ALIENWARE_TEMPO_SPECTRUM;
    Spectrum.speed_max         = ALIENWARE_TEMPO_MAX;
    Spectrum.speed             = ALIENWARE_TEMPO_SPECTRUM;
    Spectrum.brightness_min    = 100;
    Spectrum.brightness_max    = 0;
    Spectrum.brightness        = 0;
    modes.push_back(Spectrum);

    mode Rainbow;
    Rainbow.name               = "Rainbow Wave";
    Rainbow.value              = ALIENWARE_MODE_RAINBOW;
    Rainbow.flags              = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Rainbow.color_mode         = MODE_COLORS_NONE;
    Rainbow.speed_min          = ALIENWARE_TEMPO_SPECTRUM;
    Rainbow.speed_max          = ALIENWARE_TEMPO_MAX;
    Rainbow.speed              = ALIENWARE_TEMPO_SPECTRUM;
    Rainbow.brightness_min     = 100;
    Rainbow.brightness_max     = 0;
    Rainbow.brightness         = 0;
    modes.push_back(Rainbow);

    mode Breathing;
    Breathing.name             = "Breathing";
    Breathing.value            = ALIENWARE_MODE_BREATHING;
    Breathing.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Breathing.color_mode       = MODE_COLORS_PER_LED;
    Breathing.colors_min       = 1;
    Breathing.colors_max       = 1;
    Breathing.speed_min        = ALIENWARE_TEMPO_MIN;
    Breathing.speed_max        = ALIENWARE_TEMPO_MAX;
    Breathing.speed            = ALIENWARE_TEMPO_MIN;
    Breathing.brightness_min   = 100;
    Breathing.brightness_max   = 0;
    Breathing.brightness       = 0;
    modes.push_back(Breathing);

    SetupZones();

    controller->UpdateDim();
}

void RGBController_Alienware::SetupZones()
{
    /*-------------------------------------------------*\
    | Set zones and leds                                |
    \*-------------------------------------------------*/
    std::vector<const char*> zone_names = controller->GetZoneNames();

    for(unsigned int zone_idx = 0; zone_idx < controller->GetZoneCount(); zone_idx++)
    {
        zone new_zone;

        new_zone.name       = zone_names[zone_idx];
        new_zone.type       = ZONE_TYPE_SINGLE;
        new_zone.leds_min   = 1;
        new_zone.leds_max   = 1;
        new_zone.leds_count = 1;
        new_zone.matrix_map = nullptr;

        zones.emplace_back(new_zone);
    }

    for(unsigned int led_idx = 0; led_idx < zones.size(); led_idx++)
    {
        led new_led;

        new_led.name        = zones[led_idx].name + std::string(" LED");

        leds.emplace_back(new_led);
    }

    SetupColors();
}

void RGBController_Alienware::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_Alienware::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_Alienware::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_Alienware::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
}

static bool modes_eq(const mode& mode1, const mode& mode2)
{
    return( ( mode1.name           == mode2.name       )
         && ( mode1.value          == mode2.value      )
         && ( mode1.flags          == mode2.flags      )
         && ( mode1.speed_min      == mode2.speed_min  )
         && ( mode1.speed_max      == mode2.speed_max  )
         && ( mode1.colors_min     == mode2.colors_min )
         && ( mode1.colors_max     == mode2.colors_max )
         && ( mode1.speed          == mode2.speed      )
         && ( mode1.direction      == mode2.direction  )
         && ( mode1.color_mode     == mode2.color_mode )
         && ( mode1.colors         == mode2.colors     )
         && ( mode1.brightness     == mode2.brightness )
         && ( mode1.brightness_min == mode2.brightness_min )
         && ( mode1.brightness_max == mode2.brightness_max ) );
}

void RGBController_Alienware::DeviceUpdateMode()
{
    /*---------------------------------------------------------*\
    | Copy mode to get the current state-- this is racy, as the |
    | UI thread can be actively modifying this variable         |
    \*---------------------------------------------------------*/
    int  current_mode_idx = active_mode;
    mode current_mode     = modes[current_mode_idx];

    bool done             = false;

    while(!done)
    {
        /*-----------------------------------------------------*\
        | Setup state per zone                                  |
        \*-----------------------------------------------------*/
        for(uint8_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
        {
            zone current_zone = zones[zone_idx];

            /*-------------------------------------------------*\
            | Some modes use 2000ms (0x07D0) for their duration,|
            | per traces                                        |
            \*-------------------------------------------------*/
            uint16_t period = 0x07d0;

            controller->SetMode(zone_idx, current_mode.value);

            switch(current_mode_idx)
            {
                case ALIENWARE_MODE_COLOR:
                    controller->SetPeriod(zone_idx, period);
                    controller->SetColor( zone_idx, colors[current_zone.start_idx]);
                    controller->SetTempo( zone_idx, ALIENWARE_TEMPO_MAX);
                    controller->SetDim(   zone_idx, modes[current_mode_idx].brightness);
                    break;

                case ALIENWARE_MODE_PULSE:
                    controller->SetPeriod(zone_idx, period);
                    controller->SetColor( zone_idx, colors[current_zone.start_idx]);
                    controller->SetTempo( zone_idx, current_mode.speed);
                    controller->SetDim(   zone_idx, modes[current_mode_idx].brightness);
                    break;

                case ALIENWARE_MODE_MORPH:
                    controller->SetPeriod(zone_idx, period);
                    controller->SetColor( zone_idx, current_mode.colors[zone_idx * 2], current_mode.colors[(zone_idx * 2) + 1]);
                    controller->SetTempo( zone_idx, current_mode.speed);
                    controller->SetDim(   zone_idx, modes[current_mode_idx].brightness);
                    break;

                case ALIENWARE_MODE_SPECTRUM:
                case ALIENWARE_MODE_RAINBOW:
                    controller->SetPeriod(zone_idx, ALIENWARE_DURATION_SPECTRUM);
                    controller->SetTempo( zone_idx, current_mode.speed);
                    controller->SetDim(   zone_idx, modes[current_mode_idx].brightness);
                    break;

                case ALIENWARE_MODE_BREATHING:
                    controller->SetPeriod(zone_idx, period);
                    controller->SetColor( zone_idx, colors[current_zone.start_idx], 0x0);
                    controller->SetTempo( zone_idx, current_mode.speed);
                    controller->SetDim(   zone_idx, modes[current_mode_idx].brightness);
                    break;
            }
        }

        /*-----------------------------------------------------*\
        | Due to rate-limiting, this can take more than one     |
        | second to execute                                     |
        \*-----------------------------------------------------*/
        controller->UpdateController();

        /*-----------------------------------------------------*\
        | Re-run update if there's anything that's changed from |
        | under us...                                           |
        \*-----------------------------------------------------*/
        int  new_current_mode_idx = active_mode;
        mode new_current_mode     = modes[current_mode_idx];

        done                      = (current_mode_idx == new_current_mode_idx && modes_eq(new_current_mode, current_mode));
        current_mode_idx          = new_current_mode_idx;
        current_mode              = new_current_mode;
    }
}
