#include "RGBController_CorsairK55RGBPRO.h"
/*-----------------------------------------*\
|  RGBController_CorsairK55RGBPRO.cpp       |
|                                           |
|  Driver for Corsair K55 RGB PRO0 Keyboard |
\*-----------------------------------------*/

#include "LogManager.h"

using namespace std::chrono_literals;

/**------------------------------------------------------------------*\
    @name Corsair K55 RGB Pro
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectCorsairK55RGBPROControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_CorsairK55RGBPRO::RGBController_CorsairK55RGBPRO(CorsairK55RGBPROController* controller_ptr)
{
    controller      = controller_ptr;

    name            = controller->GetName();
    vendor          = "Corsair";
    description     = "Corsair K55 RGB PRO Keyboard Device";
    type            = DEVICE_TYPE_KEYBOARD;
    version         = controller->GetFirmwareString();
    location        = controller->GetDeviceLocation();
    serial          = controller->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
    /*-----------------------------------------------------*\
    | The Corsair K55 RGB PRO requires a packet within      |
    | 1 minutes of sending the lighting change in order     |
    | to not revert back into rainbow mode.  Start a thread |
    | to continuously send a keepalive packet every 50 sec  |
    \*-----------------------------------------------------*/
    keepalive_thread_run = true;
    keepalive_thread = new std::thread(&RGBController_CorsairK55RGBPRO::KeepaliveThread, this);
}

RGBController_CorsairK55RGBPRO::~RGBController_CorsairK55RGBPRO()
{
    /*-----------------------------------------------------*\
    | Close keepalive thread                                |
    \*-----------------------------------------------------*/
    keepalive_thread_run = false;
    keepalive_thread->join();
    delete keepalive_thread;

    /*---------------------------------------------------------*\
    | Delete the matrix map                                     |
    \*---------------------------------------------------------*/
    for(unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if(zones[zone_index].type == ZONE_TYPE_MATRIX)
        {
            delete zones[zone_index].matrix_map;
        }
    }

    delete controller;
}

void RGBController_CorsairK55RGBPRO::SetupZones()
{
    zone new_zone;

    new_zone.name                   = "Keyboard";
    new_zone.type                   = ZONE_TYPE_LINEAR;
    new_zone.leds_min               = 5;
    new_zone.leds_max               = 5;
    new_zone.leds_count             = 5;
    new_zone.matrix_map             = NULL;

    for(unsigned int led_idx = 0; led_idx < new_zone.leds_count; led_idx++)
    {
        led new_led;
        new_led.name = "Zone " + std::to_string( led_idx );
        leds.push_back(new_led);
    }

    zones.push_back(new_zone);

    SetupColors();
}

void RGBController_CorsairK55RGBPRO::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_CorsairK55RGBPRO::DeviceUpdateLEDs()
{
    last_update_time = std::chrono::steady_clock::now();

    controller->SetLEDs(colors);
}

void RGBController_CorsairK55RGBPRO::UpdateZoneLEDs(int /*zone*/)
{
    controller->SetLEDs(colors);
}

void RGBController_CorsairK55RGBPRO::UpdateSingleLED(int /*led*/)
{
    controller->SetLEDs(colors);
}

void RGBController_CorsairK55RGBPRO::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_CorsairK55RGBPRO::DeviceUpdateMode()
{

}

void RGBController_CorsairK55RGBPRO::KeepaliveThread()
{
    while(keepalive_thread_run.load())
    {
        if(active_mode == 0)
        {
            if((std::chrono::steady_clock::now() - last_update_time) > std::chrono::milliseconds(50000))
            {
                DeviceUpdateLEDs();
            }
        }
        std::this_thread::sleep_for(3000ms);
    }
}
