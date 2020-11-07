/*-----------------------------------------*\
|  RGBController_PhilipsHueEntertainment.cpp|
|                                           |
|  Generic RGB Interface for Philips Hue    |
|  Entertainment Mode                       |
|                                           |
|  Adam Honse (CalcProgrammer1) 11/7/2020   |
\*-----------------------------------------*/

#include "RGBController_PhilipsHueEntertainment.h"

using namespace std::chrono_literals;

RGBController_PhilipsHueEntertainment::RGBController_PhilipsHueEntertainment(PhilipsHueEntertainmentController* light_ptr)
{
    light = light_ptr;

    name        = light->GetManufacturer() + " " + light->GetName();
    type        = DEVICE_TYPE_LIGHT;
    version     = light->GetVersion();
    description = "Philips Hue Entertainment Mode Device";
    serial      = light->GetUniqueID();
    location    = light->GetLocation();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();

    /*-----------------------------------------------------*\
    | The Philips Hue Entertainment Mode requires a packet  |
    | within 10 seconds of sending the lighting change in   |
    | order to not exit entertainment mode.  Start a thread |
    | to continuously send a packet every 5s                |
    \*-----------------------------------------------------*/
    KeepaliveThreadRunning = true;
    KeepaliveThread = new std::thread(&RGBController_PhilipsHueEntertainment::KeepaliveThreadFunction, this);
}

void RGBController_PhilipsHueEntertainment::SetupZones()
{
    zone led_zone;
    led_zone.name       = "RGB Light";
    led_zone.type       = ZONE_TYPE_SINGLE;
    led_zone.leds_min   = light->GetNumLEDs();
    led_zone.leds_max   = light->GetNumLEDs();
    led_zone.leds_count = light->GetNumLEDs();
    led_zone.matrix_map = NULL;
    zones.push_back(led_zone);

    for(unsigned int led_idx = 0; led_idx < light->GetNumLEDs(); led_idx++)
    {
        led new_led;
        new_led.name = "RGB Light";

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_PhilipsHueEntertainment::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_PhilipsHueEntertainment::DeviceUpdateLEDs()
{
    last_update_time = std::chrono::steady_clock::now();

    light->SetColor(&colors[0]);
}

void RGBController_PhilipsHueEntertainment::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_PhilipsHueEntertainment::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_PhilipsHueEntertainment::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_PhilipsHueEntertainment::DeviceUpdateMode()
{

}

void RGBController_PhilipsHueEntertainment::KeepaliveThreadFunction()
{
    while(KeepaliveThreadRunning)
    {
        if(active_mode == 0)
        {
            if((std::chrono::steady_clock::now() - last_update_time) > std::chrono::seconds(5))
            {
                UpdateLEDs();
            }
        }
        std::this_thread::sleep_for(1s);
    }
}
