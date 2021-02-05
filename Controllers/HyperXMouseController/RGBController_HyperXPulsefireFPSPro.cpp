/*-----------------------------------------*\
|  RGBController_HyperXPulsefireFPSPro.cpp  |
|                                           |
|  Generic RGB Interface for HyperX         |
|  Pulsefire Surge                          |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/2/2020    |
\*-----------------------------------------*/

#include "RGBController_HyperXPulsefireFPSPro.h"

using namespace std::chrono_literals;

RGBController_HyperXPulsefireFPSPro::RGBController_HyperXPulsefireFPSPro(HyperXPulsefireFPSProController* hyperx_ptr)
{
    hyperx = hyperx_ptr;

    name        = "HyperX Pulsefire FPS Pro Device";
    vendor      = "HyperX";
    type        = DEVICE_TYPE_MOUSE;
    description = "HyperX Pulsefire FPS Pro Device";
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
    keepalive_thread = new std::thread(&RGBController_HyperXPulsefireFPSPro::KeepaliveThread, this);
};

RGBController_HyperXPulsefireFPSPro::~RGBController_HyperXPulsefireFPSPro()
{
    keepalive_thread_run = 0;
    keepalive_thread->join();
    delete keepalive_thread;

    delete hyperx;
}

void RGBController_HyperXPulsefireFPSPro::SetupZones()
{
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

void RGBController_HyperXPulsefireFPSPro::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_HyperXPulsefireFPSPro::DeviceUpdateLEDs()
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

void RGBController_HyperXPulsefireFPSPro::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXPulsefireFPSPro::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXPulsefireFPSPro::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_HyperXPulsefireFPSPro::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXPulsefireFPSPro::KeepaliveThread()
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
