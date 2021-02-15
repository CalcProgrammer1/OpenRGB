/*-------------------------------------------------------------------*\
|  RGBController_ThermaltakeRiingQuad.cpp                             |
|                                                                     |
|  Driver for Thermaltake Riing Quad Controller                       |
|                                                                     |
|  Chris M (Dr_No)          15th Feb 2021                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "RGBController_ThermaltakeRiingQuad.h"

RGBController_ThermaltakeRiingQuad::RGBController_ThermaltakeRiingQuad(ThermaltakeRiingQuadController* quad_ptr)
{
    quad = quad_ptr;

    name        = quad->GetDeviceName();
    vendor      = "Thermaltake";
    type        = DEVICE_TYPE_COOLER;
    description = "Thermaltake Riing Quad Device";
    location    = quad->GetDeviceLocation();
    serial      = quad->GetSerial();

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
    delete quad;
}

void RGBController_ThermaltakeRiingQuad::SetupZones()
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
    zones.resize(THERMALTAKE_QUAD_NUM_CHANNELS);

    /*-------------------------------------------------*\
    | Set zones and leds                                |
    \*-------------------------------------------------*/
    for (unsigned int channel_idx = 0; channel_idx < THERMALTAKE_QUAD_NUM_CHANNELS; channel_idx++)
    {
        char ch_idx_string[2];
        sprintf(ch_idx_string, "%d", channel_idx + 1);

        zones[channel_idx].name     = "Riing Channel ";
        zones[channel_idx].name.append(ch_idx_string);
        zones[channel_idx].type     = ZONE_TYPE_LINEAR;

        /*-------------------------------------------------*\
        | The maximum number of colors that would fit in the|
        | Riing Quad protocol is 54                         |
        \*-------------------------------------------------*/
        zones[channel_idx].leds_min   = 0;
        zones[channel_idx].leds_max   = 54;

        if(first_run)
        {
            zones[channel_idx].leds_count = 0;
        }

        zones[channel_idx].matrix_map = NULL;
        
        for (unsigned int led_ch_idx = 0; led_ch_idx < zones[channel_idx].leds_count; led_ch_idx++)
        {
            char led_idx_string[3];
            sprintf(led_idx_string, "%d", led_ch_idx + 1);

            led new_led;
            new_led.name = "Riing Channel ";
            new_led.name.append(ch_idx_string);
            new_led.name.append(", LED ");
            new_led.name.append(led_idx_string);

            leds.push_back(new_led);
            leds_channel.push_back(channel_idx);
        }
    }

    SetupColors();
}

void RGBController_ThermaltakeRiingQuad::ResizeZone(int zone, int new_size)
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

void RGBController_ThermaltakeRiingQuad::DeviceUpdateLEDs()
{
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        quad->SetChannelLEDs(zone_idx, zones[zone_idx].colors, zones[zone_idx].leds_count);
    }
}

void RGBController_ThermaltakeRiingQuad::UpdateZoneLEDs(int zone)
{
    quad->SetChannelLEDs(zone, zones[zone].colors, zones[zone].leds_count);
}

void RGBController_ThermaltakeRiingQuad::UpdateSingleLED(int led)
{
    unsigned int channel = leds_channel[led];

    quad->SetChannelLEDs(channel, zones[channel].colors, zones[channel].leds_count);
}

void RGBController_ThermaltakeRiingQuad::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_ThermaltakeRiingQuad::DeviceUpdateMode()
{
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        quad->SetMode(modes[active_mode].value, modes[active_mode].speed);
        quad->SetChannelLEDs(zone_idx, zones[zone_idx].colors, zones[zone_idx].leds_count);
    }
}
