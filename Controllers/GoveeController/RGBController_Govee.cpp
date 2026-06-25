/*---------------------------------------------------------*\
| RGBController_Govee.cpp                                   |
|                                                           |
|   RGBController for Govee wireless lighting devices       |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      27 Dec 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <algorithm>
#include <map>
#include <string>
#include "RGBController_Govee.h"

using namespace std::chrono_literals;

struct GoveeHardwareInfo
{
    unsigned int led_count;
    unsigned int matrix_row_len;    // 0 = linear
};

const unsigned int GOVEE_FALLBACK_LED_COUNT = 20;

static std::map<std::string, GoveeHardwareInfo> govee_hardware_info
{
    { "H6022",   { 132, 12 } },    // Govee Smart Table Lamp 2
    { "H612F",   { 12,  0 } },     // Govee Strip Light S (3m)
    { "H619A",   { 20,  0 } },     // Govee RGBIC Led Strip Lights
    { "H70B1",   { 20,  0 } },     // Govee LED Curtain Lights
    { "H607C",   { 174, 0 } },     // Govee Floor Lamp 2
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
    GoveeHardwareInfo hw = { GOVEE_FALLBACK_LED_COUNT, 0 };
    bool resizable = true;

    std::map<std::string, GoveeHardwareInfo>::iterator it = govee_hardware_info.find(controller->GetSku());
    if(it != govee_hardware_info.end())
    {
        hw = it->second;
        resizable = false;
    }

    zone strip;
    strip.leds_count = hw.led_count;
    strip.leds_min   = resizable ? 0   : hw.led_count;
    strip.leds_max   = resizable ? 255 : hw.led_count;

    if(hw.matrix_row_len == 0)
    {
        strip.name       = "Govee Strip";
        strip.type       = ZONE_TYPE_LINEAR;
        strip.matrix_map = NULL;
    }
    else
    {
        strip.name = "Govee Matrix";
        strip.type = ZONE_TYPE_MATRIX;

        unsigned int width = hw.matrix_row_len;
        unsigned int height = hw.led_count / width;

        strip.matrix_map = new matrix_map_type;
        strip.matrix_map->height = height;
        strip.matrix_map->width  = width;
        strip.matrix_map->map    = new unsigned int[hw.led_count];

        /*-----------------------------------------------------*\
        | On H6022, LEDs indexed bottom to top, alternating     |
        | clockwise and counterclockwise for each row.          |
        \*-----------------------------------------------------*/
        for(unsigned int y = 0; y < height; y++)
        {
            /*-------------------------------------------------*\
            | LEDs numbered bottom to top, opposite of matrix   |
            | clockwise and counterclockwise for each row.      |
            \*-------------------------------------------------*/
            unsigned int led_y = (height - 1) - y;

            for(unsigned int x = 0; x < width; x++)
            {
                /*---------------------------------------------*\
                | LED is right-to-left for even rows, including |
                | first one                                     |
                \*---------------------------------------------*/
                unsigned int led_x = led_y & 1 ? x : (width - 1) - x;
                strip.matrix_map->map[y * width + x] = led_y * width + led_x;
            }
        }
    }

    zones.push_back(strip);

    for(std::size_t led_idx = 0; led_idx < strip.leds_count; led_idx++)
    {
        led strip_led;
        strip_led.name      = "Govee LED " + std::to_string(led_idx);
        leds.push_back(strip_led);
    }

    SetupColors();
}

void RGBController_Govee::ResizeZone(int zone, int new_size)
{
    if(zones[zone].type == ZONE_TYPE_MATRIX)
    {
        return;
    }

    if(zone < 0 || zone >= (int)zones.size() || new_size <= 0)
    {
        return;
    }

    new_size = std::max(1, std::min(255, new_size));
    zones[zone].leds_count = new_size;
    zones[zone].leds_min = 1;
    zones[zone].leds_max = 255;

    leds.clear();
    leds.resize(new_size);
    for(int i = 0; i < new_size; ++i)
    {
        leds[i].name = "Govee LED " + std::to_string(i);
    }

    SetupColors();      /* re-sync color buffers with LED count */
    DeviceUpdateLEDs(); /* push an updated frame */
}

void RGBController_Govee::DeviceUpdateLEDs()
{
    last_update_time = std::chrono::steady_clock::now();

    if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        if(!colors.empty())
        {
            controller->SendRazerData(&colors[0], (unsigned int)colors.size());
        }
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
