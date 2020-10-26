/*-----------------------------------------*\
|  RGBController_HyperXMousemat.cpp         |
|                                           |
|  Generic RGB Interface for HyperX         |
|  mousemat                                 |
|                                           |
|  Adam Honse (CalcProgrammer1) 10/25/2020  |
\*-----------------------------------------*/

#include "RGBController_HyperXMousemat.h"

//Include thread libraries for Windows or Linux
#ifdef WIN32
#include <process.h>
#else
#include "pthread.h"
#include "unistd.h"
#endif

//Thread functions have different types in Windows and Linux
#ifdef WIN32
#define THREAD static void
#define THREADRETURN
#else
#define THREAD static void*
#define THREADRETURN return(NULL);
#endif

using namespace std::chrono_literals;

THREAD keepalive_thread(void *param)
{
    RGBController_HyperXMousemat* controller = static_cast<RGBController_HyperXMousemat*>(param);
    controller->KeepaliveThread();
    THREADRETURN
}

RGBController_HyperXMousemat::RGBController_HyperXMousemat(HyperXMousematController* hyperx_ptr)
{
    hyperx = hyperx_ptr;

    name        = "HyperX Mousemat Device";
    type        = DEVICE_TYPE_MOUSEMAT;
    description = "HyperX Mousemat Device";
    location    = hyperx->GetDeviceLocation();

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
#ifdef WIN32
    _beginthread(keepalive_thread, 0, this);
#else
    pthread_t thread;
    pthread_create(&thread, NULL, &keepalive_thread, this);
#endif
};

RGBController_HyperXMousemat::~RGBController_HyperXMousemat()
{

}

void RGBController_HyperXMousemat::SetupZones()
{
    zone led_strip;
    led_strip.name       = "LED Strip";
    led_strip.type       = ZONE_TYPE_LINEAR;
    led_strip.leds_min   = 32;
    led_strip.leds_max   = 32;
    led_strip.leds_count = 32;
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
    while(1)
    {
        if(active_mode == 0)
        {
            if((std::chrono::steady_clock::now() - last_update_time) > std::chrono::milliseconds(50))
            {
                UpdateLEDs();
            }
        }
        std::this_thread::sleep_for(10ms);
    }
}
