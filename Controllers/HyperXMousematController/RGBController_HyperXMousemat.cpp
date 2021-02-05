/*-----------------------------------------*\
|  RGBController_HyperXMousemat.cpp         |
|                                           |
|  Generic RGB Interface for HyperX         |
|  mousemat                                 |
|                                           |
|  Adam Honse (CalcProgrammer1) 10/25/2020  |
\*-----------------------------------------*/

#include "RGBController_HyperXMousemat.h"

using namespace std::chrono_literals;

RGBController_HyperXMousemat::RGBController_HyperXMousemat(HyperXMousematController* hyperx_ptr)
{
    hyperx = hyperx_ptr;

    name        = "HyperX Mousemat Device";
    vendor      = "HyperX";
    type        = DEVICE_TYPE_MOUSEMAT;
    description = "HyperX Mousemat Device";
    location    = hyperx->GetDeviceLocation();
    serial      = hyperx->GetSerialString();

    mode Direct;
    Direct.name = "Direct";
    Direct.value = 0xFFFF;
    Direct.flags = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();

    /*-----------------------------------------------------*\
    | The Corsair Lighting Node Pro requires a packet within|
    | 20 seconds of sending the lighting change in order    |
    | to not revert back into rainbow mode.  Start a thread |
    | to continuously send a keepalive packet every 5s      |
    \*-----------------------------------------------------*/
    keepalive_thread_run = 1;
    keepalive_thread = new std::thread(&RGBController_HyperXMousemat::KeepaliveThread, this);
};

RGBController_HyperXMousemat::~RGBController_HyperXMousemat()
{
    keepalive_thread_run = 0;
    keepalive_thread->join();
    delete keepalive_thread;

    delete hyperx;
}

void RGBController_HyperXMousemat::SetupZones()
{
    zone underglow;
    underglow.name       = "Underglow";
    underglow.type       = ZONE_TYPE_LINEAR;
    underglow.leds_min   = 15;
    underglow.leds_max   = 15;
    underglow.leds_count = 15;
    underglow.matrix_map = NULL;
    zones.push_back(underglow);

    zone led_strip;
    led_strip.name       = "LED Strip";
    led_strip.type       = ZONE_TYPE_LINEAR;
    led_strip.leds_min   = 5;
    led_strip.leds_max   = 5;
    led_strip.leds_count = 5;
    led_strip.matrix_map = NULL;
    zones.push_back(led_strip);

    for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        for(unsigned int led_idx = 0; led_idx < zones[zone_idx].leds_count; led_idx++)
        {
            led new_led;
            
            new_led.name = zones[zone_idx].name;

            if(zones[zone_idx].leds_count > 1)
            {
                new_led.name.append(" LED ");
                new_led.name.append(std::to_string(led_idx + 1));
            }  

            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_HyperXMousemat::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_HyperXMousemat::DeviceUpdateLEDs()
{
    last_update_time = std::chrono::steady_clock::now();

    if(active_mode == 0)
    {
        hyperx->SendDirect(&colors[0]);
    }
    else
    {
    }

}

void RGBController_HyperXMousemat::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXMousemat::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXMousemat::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_HyperXMousemat::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXMousemat::KeepaliveThread()
{
    while(keepalive_thread_run.load())
    {
        if(active_mode == 0)
        {
            if((std::chrono::steady_clock::now() - last_update_time) > std::chrono::milliseconds(50))
            {
                DeviceUpdateLEDs();
            }
        }
        std::this_thread::sleep_for(10ms);
    }
}
