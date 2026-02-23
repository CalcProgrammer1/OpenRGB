/*---------------------------------------------------------*\
| RGBController_ThermaltakeRiingQuad.cpp                    |
|                                                           |
|   RGBController for Thermaltake Riing Quad                |
|                                                           |
|   Chris M (Dr_No)                             15 Feb 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_ThermaltakeRiingQuad.h"

/**------------------------------------------------------------------*\
    @name Thermaltake Riing Quad
    @category Cooler
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectThermaltakeRiingQuadControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_ThermaltakeRiingQuad::RGBController_ThermaltakeRiingQuad(ThermaltakeRiingQuadController* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller->GetDeviceName();
    vendor      = "Thermaltake";
    type        = DEVICE_TYPE_COOLER;
    description = "Thermaltake Riing Quad Device";
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerial();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = THERMALTAKE_QUAD_MODE_DIRECT;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.speed_min  = 0;
    Direct.speed_max  = 0;
    Direct.speed      = 0;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_ThermaltakeRiingQuad::~RGBController_ThermaltakeRiingQuad()
{
    Shutdown();

    delete controller;
}

void RGBController_ThermaltakeRiingQuad::SetupZones()
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
    zones.resize(THERMALTAKE_QUAD_NUM_CHANNELS);

    /*-----------------------------------------------------*\
    | Set zones and leds                                    |
    \*-----------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        /*-------------------------------------------------*\
        | The maximum number of colors that would fit in the|
        | Riing Quad protocol is 54                         |
        \*-------------------------------------------------*/
        zones[zone_idx].leds_min                    = 0;
        zones[zone_idx].leds_max                    = 60;

        if(first_run)
        {
            zones[zone_idx].flags                   = ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_NAME
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_TYPE
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_MATRIX_MAP;
        }

        if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_NAME))
        {
            zones[zone_idx].name                    = "Thermaltake Fan Header ";
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

void RGBController_ThermaltakeRiingQuad::DeviceConfigureZone(int zone_idx)
{
    if((size_t)zone_idx < zones.size())
    {
        SetupZones();
    }
}

void RGBController_ThermaltakeRiingQuad::DeviceUpdateLEDs()
{
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        controller->SetChannelLEDs((unsigned char)zone_idx, zones[zone_idx].colors, zones[zone_idx].leds_count);
    }
}

void RGBController_ThermaltakeRiingQuad::DeviceUpdateZoneLEDs(int zone)
{
    controller->SetChannelLEDs(zone, zones[zone].colors, zones[zone].leds_count);
}

void RGBController_ThermaltakeRiingQuad::DeviceUpdateSingleLED(int led)
{
    unsigned int channel = leds_channel[led];

    controller->SetChannelLEDs(channel, zones[channel].colors, zones[channel].leds_count);
}

void RGBController_ThermaltakeRiingQuad::DeviceUpdateMode()
{
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        controller->SetMode(modes[active_mode].value, modes[active_mode].speed);
        controller->SetChannelLEDs((unsigned char)zone_idx, zones[zone_idx].colors, zones[zone_idx].leds_count);
    }
}
