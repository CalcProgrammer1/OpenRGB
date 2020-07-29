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
    Direct.value = 0;
    Direct.flags = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

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
    zones.push_back(led_strip);

    zone logo;
    logo.name       = "Logo";
    logo.type       = ZONE_TYPE_SINGLE;
    logo.leds_min   = 1;
    logo.leds_max   = 1;
    logo.leds_count = 1;
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
    hyperx->SendData(&colors[0]);
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

}

void RGBController_HyperXPulsefireSurge::KeepaliveThread()
{

}
