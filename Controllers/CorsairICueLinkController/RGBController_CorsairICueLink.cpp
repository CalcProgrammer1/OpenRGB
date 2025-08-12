/*---------------------------------------------------------*\
| RGBController_CorsairICueLink.cpp                         |
|                                                           |
|   Driver for Corsair iCue Link Devices                    |
|                                                           |
|   Aiden Vigue (acvigue)                       02 Mar 2025 |
|   Adam Honse <calcprogrammer1@gmail.com>      01 Aug 2025 |
|   Nikola Jurkovic (jurkovic.nikola)           11 Aug 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "CorsairICueLinkProtocol.h"
#include "RGBController_CorsairICueLink.h"

using namespace std::chrono_literals;

/**------------------------------------------------------------------*\
    @name Corsair iCUE Link Device Controller
    @category Cooler
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectCorsairICueLinkController
    @comment
\*-------------------------------------------------------------------*/

RGBController_CorsairICueLink::RGBController_CorsairICueLink(CorsairICueLinkController* controller)
{
    this->controller        = controller;

    name                    = controller->GetNameString();
    vendor                  = "Corsair";
    description             = "iCUE Link Device";
    version                 = controller->GetFirmwareString();
    location                = controller->GetLocationString();
    serial                  = controller->GetSerialString();
    type                    = DEVICE_TYPE_COOLER;

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = 0;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();

    keepalive_thread_run    = 1;
    keepalive_thread        = new std::thread(&RGBController_CorsairICueLink::KeepaliveThread, this);
}

RGBController_CorsairICueLink::~RGBController_CorsairICueLink()
{
    keepalive_thread_run = 0;
    keepalive_thread->join();
    delete keepalive_thread;

    delete controller;
}

void RGBController_CorsairICueLink::SetupZones()
{
    for(std::size_t zone_idx = 0; zone_idx < controller->GetEndpoints().size(); zone_idx++)
    {
        if(controller->GetEndpoints()[zone_idx]->type == 0x06)
        {
            /*-----------------------------------------------------*\
            | We skip LCD processing here                           |
            \*-----------------------------------------------------*/
            continue;
        }

        zone new_zone;
        new_zone.name       = controller->GetEndpoints()[zone_idx]->display_name;
        new_zone.type       = ZONE_TYPE_LINEAR;
        new_zone.leds_min   = controller->GetEndpoints()[zone_idx]->led_channels;
        new_zone.leds_max   = new_zone.leds_min;
        new_zone.leds_count = new_zone.leds_min;
        zones.push_back(new_zone);

        if(controller->GetEndpoints()[zone_idx]->type == 0x07 || controller->GetEndpoints()[zone_idx]->type == 0x11)
        {
            /*---------------------------------------------------------*\
            | iCUE LINK AIO 'H' Series || iCUE LINK AIO 'TITAN' Series  |
            \*---------------------------------------------------------*/
            for(std::size_t lcd_idx = 0; lcd_idx < controller->GetEndpoints().size(); lcd_idx++)
            {
                if(controller->GetEndpoints()[lcd_idx]->type == 0x06)
                {
                    zone lcd_zone;
                    lcd_zone.name       = controller->GetEndpoints()[lcd_idx]->display_name;
                    lcd_zone.type       = ZONE_TYPE_LINEAR;
                    lcd_zone.leds_min   = controller->GetEndpoints()[lcd_idx]->led_channels;
                    lcd_zone.leds_max   = lcd_zone.leds_min;
                    lcd_zone.leds_count = lcd_zone.leds_min;
                    zones.push_back(lcd_zone);
                }
            }
        }

        for(unsigned int led_idx = 0; led_idx < new_zone.leds_count; led_idx++)
        {
            led new_led;
            new_led.name = "LED " + std::to_string(led_idx + 1);

            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_CorsairICueLink::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*-----------------------------------------------------*\
    | Device does not support resizing zones                |
    \*-----------------------------------------------------*/
}

void RGBController_CorsairICueLink::DeviceUpdateLEDs()
{
    controller->UpdateLights(&colors[0], colors.size());
}

void RGBController_CorsairICueLink::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CorsairICueLink::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CorsairICueLink::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}

void RGBController_CorsairICueLink::KeepaliveThread()
{
    while(keepalive_thread_run.load())
    {
        if((std::chrono::steady_clock::now() - last_commit_time) > std::chrono::seconds(5))
        {
            DeviceUpdateLEDs();
        }
        std::this_thread::sleep_for(1s);
    }
}