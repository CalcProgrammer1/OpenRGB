/*-----------------------------------------*\
|  RGBController_HyperXPulsefireSurge.cpp   |
|                                           |
|  Generic RGB Interface for HyperX         |
|  Pulsefire Surge                          |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/2/2020    |
\*-----------------------------------------*/

#include "RGBController_HyperXPulsefireSurge.h"

RGBController_HyperXPulsefireSurge::RGBController_HyperXPulsefireSurge(HyperXPulsefireSurgeController* hyperx_ptr)
{
    hyperx = hyperx_ptr;

    name        = "HyperX Pulsefire Surge Device";
    type        = DEVICE_TYPE_MOUSE;
    description = "HyperX Pulsefire Surge Device";

    mode Direct;
    Direct.name = "Direct";
    Direct.value = HYPERX_PULSEFIRE_SURGE_MODE_SOLID;
    Direct.flags = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Cycle;
    Cycle.name = "Cycle";
    Cycle.value = HYPERX_PULSEFIRE_SURGE_MODE_CYCLE;
    Cycle.flags = 0;
    Cycle.color_mode = MODE_COLORS_NONE;
    modes.push_back(Cycle);

    mode Breathing;
    Breathing.name = "Breathing";
    Breathing.value = HYPERX_PULSEFIRE_SURGE_MODE_BREATHING;
    Breathing.flags = 0;
    Breathing.color_mode = MODE_COLORS_NONE;
    modes.push_back(Breathing);

    mode Wave;
    Wave.name = "Wave";
    Wave.value = HYPERX_PULSEFIRE_SURGE_MODE_WAVE;
    Wave.flags = 0;
    Wave.color_mode = MODE_COLORS_NONE;
    modes.push_back(Wave);

    SetupZones();
};

RGBController_HyperXPulsefireSurge::~RGBController_HyperXPulsefireSurge()
{

}

void RGBController_HyperXPulsefireSurge::SetupZones()
{
    zone led_strip;
    led_strip.name       = "LED Strip";
    led_strip.type       = ZONE_TYPE_LINEAR;
    led_strip.leds_min   = 31;
    led_strip.leds_max   = 31;
    led_strip.leds_count = 31;
    led_strip.matrix_map = NULL;
    zones.push_back(led_strip);

    zone logo;
    logo.name       = "Logo";
    logo.type       = ZONE_TYPE_SINGLE;
    logo.leds_min   = 1;
    logo.leds_max   = 1;
    logo.leds_count = 1;
    logo.matrix_map = NULL;
    zones.push_back(logo);

    for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        for(unsigned int led_idx = 0; led_idx < zones[zone_idx].leds_count; led_idx++)
        {
            led new_led;
            new_led.name = zones[zone_idx].name + " LED";
            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_HyperXPulsefireSurge::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_HyperXPulsefireSurge::DeviceUpdateLEDs()
{
    hyperx->SendData(active_mode, &colors[0]);
}

void RGBController_HyperXPulsefireSurge::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXPulsefireSurge::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXPulsefireSurge::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_HyperXPulsefireSurge::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXPulsefireSurge::KeepaliveThread()
{

}
