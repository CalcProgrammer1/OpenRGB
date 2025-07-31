/*---------------------------------------------------------*\
| RGBController_PhilipsHueEntertainment.cpp                 |
|                                                           |
|   RGBController for Philips Hue Entertainment Mode        |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      07 Nov 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_PhilipsHueEntertainment.h"
#include "ResourceManager.h"

using namespace std::chrono_literals;

/**------------------------------------------------------------------*\
    @name Philips Hue Entertainment
    @category Light
    @type Network
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectPhilipsHueControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_PhilipsHueEntertainment::RGBController_PhilipsHueEntertainment(PhilipsHueEntertainmentController* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller->GetManufacturer() + " " + controller->GetName();
    type        = DEVICE_TYPE_LIGHT;
    version     = controller->GetVersion();
    description = "Philips Hue Entertainment Mode Device";
    serial      = controller->GetUniqueID();
    location    = controller->GetLocation();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Disconnected;
    Disconnected.name       = "Disconnected";
    Disconnected.value      = 1;
    Disconnected.flags      = 0;
    Disconnected.color_mode = MODE_COLORS_NONE;
    modes.push_back(Disconnected);

    SetupZones();

    /*-----------------------------------------------------------------------------------------------------*\
    | The Philips Hue Entertainment Mode only supports one stream at a time. So we must start Disconnected. |
    | https://developers.meethue.com/develop/hue-entertainment/philips-hue-entertainment-api/               |
    \*-----------------------------------------------------------------------------------------------------*/

    active_mode = 1;

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
    led_zone.leds_min   = controller->GetNumLEDs();
    led_zone.leds_max   = controller->GetNumLEDs();
    led_zone.leds_count = controller->GetNumLEDs();
    led_zone.matrix_map = NULL;
    zones.push_back(led_zone);

    for(unsigned int led_idx = 0; led_idx < controller->GetNumLEDs(); led_idx++)
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

    if(active_mode == 0)
    {
        controller->SetColor(&colors[0]);
    }
}

void RGBController_PhilipsHueEntertainment::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_PhilipsHueEntertainment::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_PhilipsHueEntertainment::DeviceUpdateMode()
{
    if(active_mode == 0)
    {
        std::vector<RGBController*> rgb_controllers = ResourceManager::get()->GetRGBControllers();

        for(unsigned int controller_idx = 0; controller_idx < rgb_controllers.size(); controller_idx++)
        {
            if(rgb_controllers[controller_idx] != this && rgb_controllers[controller_idx]->GetDescription() == "Philips Hue Entertainment Mode Device" && rgb_controllers[controller_idx]->active_mode == 0)
            {
                rgb_controllers[controller_idx]->SetMode(1);
            }
        }

        controller->Connect();
    }
    else
    {
        controller->Disconnect();
    }
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
