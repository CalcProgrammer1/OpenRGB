/*---------------------------------------------------------*\
| RGBController_ZalmanZSync.cpp                             |
|                                                           |
|   RGBController for Zalman Z Sync                         |
|                                                           |
|   Adam Honse (CalcProgrammer1)                30 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_ZalmanZSync.h"

/**------------------------------------------------------------------*\
    @name Zalmna Z Sync
    @category LEDStrip
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectZalmanZSyncControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_ZalmanZSync::RGBController_ZalmanZSync(ZalmanZSyncController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetNameString();
    vendor                  = "Zalman";
    description             = "Zalman Z Sync Device";
    type                    = DEVICE_TYPE_LEDSTRIP;
    version                 = controller->GetFirmwareString();
    location                = controller->GetLocationString();
    serial                  = controller->GetSerialString();

    SetupZones();
    SetupModes();
}

RGBController_ZalmanZSync::~RGBController_ZalmanZSync()
{
    Shutdown();

    delete controller;
}

void RGBController_ZalmanZSync::SetupModes()
{
    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = ZALMAN_Z_SYNC_MODE_DIRECT;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_REQUIRES_ENTIRE_DEVICE;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode RainbowWave;
    RainbowWave.name        = "Rainbow Wave";
    RainbowWave.value       = ZALMAN_Z_SYNC_MODE_RAINBOW_WAVE;
    RainbowWave.flags       = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    RainbowWave.speed_min   = ZALMAN_Z_SYNC_SPEED_SLOW;
    RainbowWave.speed_max   = ZALMAN_Z_SYNC_SPEED_FAST;
    RainbowWave.speed       = ZALMAN_Z_SYNC_SPEED_MEDIUM;
    RainbowWave.direction   = MODE_DIRECTION_RIGHT;
    RainbowWave.color_mode  = MODE_COLORS_NONE;
    modes.push_back(RainbowWave);
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].modes.push_back(RainbowWave);
    }

    mode ColorShift;
    ColorShift.name         = "Color Shift";
    ColorShift.value        = ZALMAN_Z_SYNC_MODE_COLOR_SHIFT;
    ColorShift.flags        = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    ColorShift.colors_min   = 2;
    ColorShift.colors_max   = 2;
    ColorShift.speed_min    = ZALMAN_Z_SYNC_SPEED_SLOW;
    ColorShift.speed_max    = ZALMAN_Z_SYNC_SPEED_FAST;
    ColorShift.speed        = ZALMAN_Z_SYNC_SPEED_MEDIUM;
    ColorShift.color_mode   = MODE_COLORS_MODE_SPECIFIC;
    ColorShift.colors.resize(2);
    modes.push_back(ColorShift);
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].modes.push_back(ColorShift);
    }

    mode ColorPulse;
    ColorPulse.name         = "Color Pulse";
    ColorPulse.value        = ZALMAN_Z_SYNC_MODE_COLOR_PULSE;
    ColorPulse.flags        = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    ColorPulse.colors_min   = 2;
    ColorPulse.colors_max   = 2;
    ColorPulse.speed_min    = ZALMAN_Z_SYNC_SPEED_SLOW;
    ColorPulse.speed_max    = ZALMAN_Z_SYNC_SPEED_FAST;
    ColorPulse.speed        = ZALMAN_Z_SYNC_SPEED_MEDIUM;
    ColorPulse.color_mode   = MODE_COLORS_MODE_SPECIFIC;
    ColorPulse.colors.resize(2);
    modes.push_back(ColorPulse);
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].modes.push_back(ColorPulse);
    }

    mode ColorWave;
    ColorWave.name          = "Color Wave";
    ColorWave.value         = ZALMAN_Z_SYNC_MODE_COLOR_WAVE;
    ColorWave.flags         = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    ColorWave.colors_min    = 2;
    ColorWave.colors_max    = 2;
    ColorWave.speed_min     = ZALMAN_Z_SYNC_SPEED_SLOW;
    ColorWave.speed_max     = ZALMAN_Z_SYNC_SPEED_FAST;
    ColorWave.speed         = ZALMAN_Z_SYNC_SPEED_MEDIUM;
    ColorWave.direction     = MODE_DIRECTION_RIGHT;
    ColorWave.color_mode    = MODE_COLORS_MODE_SPECIFIC;
    ColorWave.colors.resize(2);
    modes.push_back(ColorWave);
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].modes.push_back(ColorWave);
    }

    mode Static;
    Static.name             = "Static";
    Static.value            = ZALMAN_Z_SYNC_MODE_STATIC;
    Static.flags            = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.colors_min       = 1;
    Static.colors_max       = 1;
    Static.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].modes.push_back(Static);
    }

    mode Temperature;
    Temperature.name        = "Temperature";
    Temperature.value       = ZALMAN_Z_SYNC_MODE_TEMPERATURE;
    Temperature.flags       = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Temperature.colors_min  = 3;
    Temperature.colors_max  = 3;
    Temperature.color_mode  = MODE_COLORS_MODE_SPECIFIC;
    Temperature.colors.resize(3);
    modes.push_back(Temperature);
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].modes.push_back(Temperature);
    }

    mode Visor;
    Visor.name              = "Visor";
    Visor.value             = ZALMAN_Z_SYNC_MODE_VISOR;
    Visor.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Visor.colors_min        = 2;
    Visor.colors_max        = 2;
    Visor.speed_min         = ZALMAN_Z_SYNC_SPEED_SLOW;
    Visor.speed_max         = ZALMAN_Z_SYNC_SPEED_FAST;
    Visor.speed             = ZALMAN_Z_SYNC_SPEED_MEDIUM;
    Visor.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Visor.colors.resize(2);
    modes.push_back(Visor);
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].modes.push_back(Visor);
    }

    mode Marquee;
    Marquee.name            = "Marquee";
    Marquee.value           = ZALMAN_Z_SYNC_MODE_MARQUEE;
    Marquee.flags           = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Marquee.colors_min      = 1;
    Marquee.colors_max      = 1;
    Marquee.speed_min       = ZALMAN_Z_SYNC_SPEED_SLOW;
    Marquee.speed_max       = ZALMAN_Z_SYNC_SPEED_FAST;
    Marquee.speed           = ZALMAN_Z_SYNC_SPEED_MEDIUM;
    Marquee.direction       = MODE_DIRECTION_RIGHT;
    Marquee.color_mode      = MODE_COLORS_MODE_SPECIFIC;
    Marquee.colors.resize(1);
    modes.push_back(Marquee);
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].modes.push_back(Marquee);
    }

    mode Blink;
    Blink.name              = "Blink";
    Blink.value             = ZALMAN_Z_SYNC_MODE_BLINK;
    Blink.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Blink.colors_min        = 2;
    Blink.colors_max        = 2;
    Blink.speed_min         = ZALMAN_Z_SYNC_SPEED_SLOW;
    Blink.speed_max         = ZALMAN_Z_SYNC_SPEED_FAST;
    Blink.speed             = ZALMAN_Z_SYNC_SPEED_MEDIUM;
    Blink.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Blink.colors.resize(2);
    modes.push_back(Blink);
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].modes.push_back(Blink);
    }

    mode Sequential;
    Sequential.name         = "Sequential";
    Sequential.value        = ZALMAN_Z_SYNC_MODE_SEQUENTIAL;
    Sequential.flags        = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Sequential.colors_min   = 1;
    Sequential.colors_max   = 1;
    Sequential.speed_min    = ZALMAN_Z_SYNC_SPEED_SLOW;
    Sequential.speed_max    = ZALMAN_Z_SYNC_SPEED_FAST;
    Sequential.speed        = ZALMAN_Z_SYNC_SPEED_MEDIUM;
    Sequential.direction    = MODE_DIRECTION_RIGHT;
    Sequential.color_mode   = MODE_COLORS_MODE_SPECIFIC;
    Sequential.colors.resize(1);
    modes.push_back(Sequential);
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].modes.push_back(Sequential);
    }

    mode Rainbow;
    Rainbow.name            = "Rainbow";
    Rainbow.value           = ZALMAN_Z_SYNC_MODE_RAINBOW;
    Rainbow.flags           = MODE_FLAG_HAS_SPEED;
    Rainbow.speed_min       = ZALMAN_Z_SYNC_SPEED_SLOW;
    Rainbow.speed_max       = ZALMAN_Z_SYNC_SPEED_FAST;
    Rainbow.speed           = ZALMAN_Z_SYNC_SPEED_MEDIUM;
    Rainbow.color_mode      = MODE_COLORS_NONE;
    modes.push_back(Rainbow);
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].modes.push_back(Rainbow);
    }
}

void RGBController_ZalmanZSync::SetupZones()
{
    /*-----------------------------------------------------*\
    | Only set LED count on the first run                   |
    \*-----------------------------------------------------*/
    bool first_run = false;

    if(zones.size() == 0)
    {
        first_run = true;
    }

    /*-----------------------------------------------------*\
    | Clear any existing color/LED configuration            |
    \*-----------------------------------------------------*/
    leds.clear();
    colors.clear();
    zones.resize(ZALMAN_Z_SYNC_NUM_CHANNELS);

    /*-----------------------------------------------------*\
    | Set zones and leds                                    |
    \*-----------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].leds_min                    = 0;
        zones[zone_idx].leds_max                    = 40;

        if(first_run)
        {
            zones[zone_idx].flags                   = ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_NAME
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_TYPE
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_MATRIX_MAP
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_SEGMENTS;
        }

        if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_NAME))
        {
            zones[zone_idx].name                    = "Addressable RGB Header ";
            zones[zone_idx].name.append(std::to_string(zone_idx + 1));
        }

        if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_SIZE))
        {
            zones[zone_idx].leds_count              = 0;
        }

        if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_TYPE))
        {
            zones[zone_idx].type                    = ZONE_TYPE_LINEAR;
        }

        if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_MATRIX_MAP))
        {
            zones[zone_idx].matrix_map.width        = 0;
            zones[zone_idx].matrix_map.height       = 0;
            zones[zone_idx].matrix_map.map.resize(0);
        }

        for(unsigned int led_ch_idx = 0; led_ch_idx < zones[zone_idx].leds_count; led_ch_idx++)
        {
            led new_led;
            new_led.name                            = zones[zone_idx].name + ", LED " + std::to_string(led_ch_idx + 1);

            leds.push_back(new_led);
            leds_channel.push_back(zone_idx);
        }
    }

    SetupColors();
}

void RGBController_ZalmanZSync::DeviceConfigureZone(int zone_idx)
{
    if((size_t)zone_idx < zones.size())
    {
        SetupZones();
    }
}

void RGBController_ZalmanZSync::DeviceUpdateLEDs()
{
    for(unsigned char zone_idx = 0; zone_idx < (unsigned char)zones.size(); zone_idx++)
    {
        DeviceUpdateZoneLEDs(zone_idx);
    }
}

void RGBController_ZalmanZSync::DeviceUpdateZoneLEDs(int zone)
{
    if(modes[active_mode].value == ZALMAN_Z_SYNC_MODE_DIRECT)
    {
        controller->SetChannelLEDs(zone, zones[zone].colors, zones[zone].leds_count);
    }
}

void RGBController_ZalmanZSync::DeviceUpdateSingleLED(int led)
{
    unsigned int channel = leds_channel[led];

    DeviceUpdateZoneLEDs(channel);
}

void RGBController_ZalmanZSync::DeviceUpdateMode()
{
    if(modes[active_mode].value == ZALMAN_Z_SYNC_MODE_DIRECT)
    {
        DeviceUpdateLEDs();
    }
    else
    {
        for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
        {
            DeviceUpdateZoneMode(zone_idx);
        }
    }
}

void RGBController_ZalmanZSync::DeviceUpdateZoneMode(int zone)
{
    if(modes[active_mode].value == ZALMAN_Z_SYNC_MODE_DIRECT)
    {
        return;
    }
    else
    {
        unsigned int    direction   = 0;
        mode*           mode_ptr    = NULL;
        bool            random      = false;

        if((zones[zone].active_mode >= 0) && (zones[zone].active_mode < (int)zones[zone].modes.size()))
        {
            mode_ptr = &zones[zone].modes[zones[zone].active_mode];
        }
        else if(active_mode < (int)modes.size())
        {
            mode_ptr = &modes[active_mode];
        }

        if(mode_ptr != NULL)
        {
            random = (mode_ptr->color_mode == MODE_COLORS_RANDOM);

            if(mode_ptr->direction == MODE_DIRECTION_RIGHT)
            {
                direction = 1;
            }

            unsigned char mode_colors[9];
            memset(mode_colors, 0, sizeof(mode_colors));

            if(mode_ptr->color_mode == MODE_COLORS_MODE_SPECIFIC)
            {
                for(std::size_t i = 0; i < mode_ptr->colors.size(); i++)
                {
                    mode_colors[(3 * i) + 0] = RGBGetRValue(mode_ptr->colors[i]);
                    mode_colors[(3 * i) + 1] = RGBGetGValue(mode_ptr->colors[i]);
                    mode_colors[(3 * i) + 2] = RGBGetBValue(mode_ptr->colors[i]);
                }
            }

            controller->SetChannelEffect(zone,
                                         zones[zone].leds_count,
                                         mode_ptr->value,
                                         mode_ptr->speed,
                                         direction,
                                         random,
                                         mode_colors[0],
                                         mode_colors[1],
                                         mode_colors[2],
                                         mode_colors[3],
                                         mode_colors[4],
                                         mode_colors[5],
                                         mode_colors[6],
                                         mode_colors[7],
                                         mode_colors[8]);
        }
    }
}
