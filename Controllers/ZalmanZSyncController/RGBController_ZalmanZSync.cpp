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

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = 0xFFFF;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
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

    mode Static;
    Static.name             = "Static";
    Static.value            = ZALMAN_Z_SYNC_MODE_STATIC;
    Static.flags            = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.colors_min       = 1;
    Static.colors_max       = 1;
    Static.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Temperature;
    Temperature.name        = "Temperature";
    Temperature.value       = ZALMAN_Z_SYNC_MODE_TEMPERATURE;
    Temperature.flags       = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Temperature.colors_min  = 3;
    Temperature.colors_max  = 3;
    Temperature.color_mode  = MODE_COLORS_MODE_SPECIFIC;
    Temperature.colors.resize(3);
    modes.push_back(Temperature);

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

    mode Rainbow;
    Rainbow.name            = "Rainbow";
    Rainbow.value           = ZALMAN_Z_SYNC_MODE_RAINBOW;
    Rainbow.flags           = MODE_FLAG_HAS_SPEED;
    Rainbow.speed_min       = ZALMAN_Z_SYNC_SPEED_SLOW;
    Rainbow.speed_max       = ZALMAN_Z_SYNC_SPEED_FAST;
    Rainbow.speed           = ZALMAN_Z_SYNC_SPEED_MEDIUM;
    Rainbow.color_mode      = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    SetupZones();
}

RGBController_ZalmanZSync::~RGBController_ZalmanZSync()
{
    delete controller;
}

void RGBController_ZalmanZSync::SetupZones()
{
    /*-------------------------------------------------*\
    | Only set LED count on the first run               |
    \*-------------------------------------------------*/
    bool first_run = false;

    if(zones.size() == 0)
    {
        first_run = true;
    }

    /*-------------------------------------------------*\
    | Clear any existing color/LED configuration        |
    \*-------------------------------------------------*/
    leds.clear();
    colors.clear();
    zones.resize(ZALMAN_Z_SYNC_NUM_CHANNELS);

    /*-------------------------------------------------*\
    | Set zones and leds                                |
    \*-------------------------------------------------*/
    for (unsigned int channel_idx = 0; channel_idx < ZALMAN_Z_SYNC_NUM_CHANNELS; channel_idx++)
    {
        char ch_idx_string[2];
        snprintf(ch_idx_string, 2, "%d", channel_idx + 1);

        zones[channel_idx].name     = "Channel ";
        zones[channel_idx].name.append(ch_idx_string);
        zones[channel_idx].type     = ZONE_TYPE_LINEAR;

        /*-------------------------------------------------*\
        | I did some experimenting and determined that the  |
        | maximum number of LEDs the Corsair Commander Pro  |
        | can support is 200.                               |
        \*-------------------------------------------------*/
        zones[channel_idx].leds_min   = 0;
        zones[channel_idx].leds_max   = 40;

        if(first_run)
        {
            zones[channel_idx].leds_count = 0;
        }

        zones[channel_idx].matrix_map = NULL;

        for (unsigned int led_ch_idx = 0; led_ch_idx < zones[channel_idx].leds_count; led_ch_idx++)
        {
            char led_idx_string[4];
            snprintf(led_idx_string, 4, "%d", led_ch_idx + 1);

            led new_led;
            new_led.name = "LED ";
            new_led.name.append(led_idx_string);

            leds.push_back(new_led);
            leds_channel.push_back(channel_idx);
        }
    }

    SetupColors();
}

void RGBController_ZalmanZSync::ResizeZone(int zone, int new_size)
{
    if((size_t) zone >= zones.size())
    {
        return;
    }

    if(((unsigned int)new_size >= zones[zone].leds_min) && ((unsigned int)new_size <= zones[zone].leds_max))
    {
        zones[zone].leds_count = new_size;

        SetupZones();
    }
}

void RGBController_ZalmanZSync::DeviceUpdateLEDs()
{
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        if(zones[zone_idx].leds_count > 0)
        {
            controller->SetChannelLEDs((unsigned char)zone_idx, zones[zone_idx].colors, zones[zone_idx].leds_count);
        }
    }
}

void RGBController_ZalmanZSync::UpdateZoneLEDs(int zone)
{
    controller->SetChannelLEDs(zone, zones[zone].colors, zones[zone].leds_count);
}

void RGBController_ZalmanZSync::UpdateSingleLED(int led)
{
    unsigned int channel = leds_channel[led];

    controller->SetChannelLEDs(channel, zones[channel].colors, zones[channel].leds_count);
}

void RGBController_ZalmanZSync::DeviceUpdateMode()
{
    if(modes[active_mode].value == 0xFFFF)
    {
        DeviceUpdateLEDs();
    }
    else
    {
        for(int channel = 0; channel < ZALMAN_Z_SYNC_NUM_CHANNELS; channel++)
        {
            unsigned int direction = 0;
            bool random = (modes[active_mode].color_mode == MODE_COLORS_RANDOM);

            if(modes[active_mode].direction == MODE_DIRECTION_RIGHT)
            {
                direction = 1;
            }

            unsigned char mode_colors[9];

            if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
            {
                for(std::size_t i = 0; i < modes[active_mode].colors.size(); i++)
                {
                    mode_colors[(3 * i) + 0] = RGBGetRValue(modes[active_mode].colors[i]);
                    mode_colors[(3 * i) + 1] = RGBGetGValue(modes[active_mode].colors[i]);
                    mode_colors[(3 * i) + 2] = RGBGetBValue(modes[active_mode].colors[i]);
                }
            }

            controller->SetChannelEffect(channel,
                                         zones[channel].leds_count,
                                         modes[active_mode].value,
                                         modes[active_mode].speed,
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
