/*---------------------------------------------------------*\
| RGBController_Govee.cpp                                   |
|                                                           |
|   RGBController for Govee wireless lighting devices       |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      27 Dec 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <map>
#include "RGBController_Govee.h"

using namespace std::chrono_literals;

static std::map<std::string, unsigned int> govee_led_counts
{
    { "H619A", 20  },
    { "H70B1", 20  },
};

RGBController_Govee::RGBController_Govee(GoveeController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "Govee " + controller->GetSku();
    vendor      = "Govee";
    type        = DEVICE_TYPE_LIGHT;
    description = "Govee Device";
    location    = controller->GetLocation();
    version     = controller->GetVersion();

    mode Static;
    Static.name         = "Static";
    Static.value        = 1;
    Static.flags        = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.color_mode   = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min   = 1;
    Static.colors_max   = 1;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = 0;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();

    keepalive_thread_run    = 1;
    keepalive_thread        = new std::thread(&RGBController_Govee::KeepaliveThread, this);
}

RGBController_Govee::~RGBController_Govee()
{
    keepalive_thread_run = 0;
    keepalive_thread->join();
    delete keepalive_thread;

    delete controller;
}

void RGBController_Govee::SetupZones()
{
    unsigned int led_count = govee_led_counts[controller->GetSku()];

    zone strip;
    strip.name          = "Govee Strip";
    strip.type          = ZONE_TYPE_LINEAR;
    strip.leds_count    = led_count;
    strip.leds_min      = led_count;
    strip.leds_max      = led_count;
    strip.matrix_map    = NULL;
    zones.push_back(strip);

    for(std::size_t led_idx = 0; led_idx < strip.leds_count; led_idx++)
    {
        led strip_led;
        strip_led.name      = "Govee LED";
        leds.push_back(strip_led);
    }

    SetupColors();
}

void RGBController_Govee::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_Govee::DeviceUpdateLEDs()
{
    last_update_time = std::chrono::steady_clock::now();

    if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        controller->SendRazerData(&colors[0], (unsigned int)colors.size());
    }
}

void RGBController_Govee::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_Govee::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_Govee::DeviceUpdateMode()
{
    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        unsigned char red = RGBGetRValue(modes[active_mode].colors[0]);
        unsigned char grn = RGBGetGValue(modes[active_mode].colors[0]);
        unsigned char blu = RGBGetBValue(modes[active_mode].colors[0]);
        controller->SetColor(red, grn, blu);
    }
    else
    {
        controller->SendRazerEnable();
        DeviceUpdateLEDs();
    }
}

void RGBController_Govee::KeepaliveThread()
{
    while(keepalive_thread_run.load())
    {
        if((std::chrono::steady_clock::now() - last_update_time) > std::chrono::seconds(30))
        {
            DeviceUpdateLEDs();
        }
        std::this_thread::sleep_for(10s);
    }
}
