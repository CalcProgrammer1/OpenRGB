/*-----------------------------------------*\
|  RGBController_HyperXMicrophone.cpp       |
|                                           |
|  Generic RGB Interface for HyperX         |
|  microphone                               |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/16/2023   |
\*-----------------------------------------*/

#include "RGBController_HyperXMicrophone.h"

using namespace std::chrono_literals;

/**------------------------------------------------------------------*\
    @name HyperX Microphone
    @category Microphone
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectHyperXMicrophoneControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_HyperXMicrophone::RGBController_HyperXMicrophone(HyperXMicrophoneController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "HyperX Microphone Device";
    vendor      = "HyperX";
    type        = DEVICE_TYPE_MICROPHONE;
    description = "HyperX Microphone Device";
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0xFFFF;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
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
    keepalive_thread = new std::thread(&RGBController_HyperXMicrophone::KeepaliveThread, this);
};

RGBController_HyperXMicrophone::~RGBController_HyperXMicrophone()
{
    keepalive_thread_run = 0;
    keepalive_thread->join();
    delete keepalive_thread;

    delete controller;
}

void RGBController_HyperXMicrophone::SetupZones()
{
    zone microphone;
    microphone.name       = "Microphone";
    microphone.type       = ZONE_TYPE_SINGLE;
    microphone.leds_min   = 2;
    microphone.leds_max   = 2;
    microphone.leds_count = 2;
    microphone.matrix_map = NULL;
    zones.push_back(microphone);

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

void RGBController_HyperXMicrophone::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_HyperXMicrophone::DeviceUpdateLEDs()
{
    last_update_time = std::chrono::steady_clock::now();

    if(active_mode == 0)
    {
        controller->SendDirect(&colors[0]);
    }
    else
    {
    }
}

void RGBController_HyperXMicrophone::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXMicrophone::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXMicrophone::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXMicrophone::KeepaliveThread()
{
    while(keepalive_thread_run.load())
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
