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

/*---------------------------------------------------------*\
| A device's zone shape is picked from this fixed set of    |
| known layouts rather than special-cased by SKU string, so |
| adding a device that fits an existing shape is a one-line |
| table entry rather than a new code path.                  |
|                                                           |
|   LINEAR      single strip, no matrix                     |
|   SERPENTINE  matrix wired bottom-to-top, alternating     |
|               scan direction each row (e.g. H6022)        |
|   SPLIT_BARS  two disconnected bars either side of a gap, |
|               e.g. two light bars flanking a monitor      |
\*---------------------------------------------------------*/
enum GoveeZoneLayout
{
    GOVEE_ZONE_LINEAR,
    GOVEE_ZONE_SERPENTINE,
    GOVEE_ZONE_SPLIT_BARS,
};

struct GoveeDeviceInfo
{
    unsigned int    led_count;
    GoveeZoneLayout layout;
    unsigned int    matrix_row_len;    // SERPENTINE only: LEDs per row
    unsigned int    gap_columns;       // SPLIT_BARS only: empty columns between the two bars
    bool            supports_razer;
};

const unsigned int GOVEE_FALLBACK_LED_COUNT = 1;
#define GOVEE_MATRIX_LED_NONE                       0xFFFFFFFF

static std::map<std::string, GoveeDeviceInfo> govee_device_info
{
    { "H6022",   { 132, GOVEE_ZONE_SERPENTINE, 12, 0,  true } },    // Govee Smart Table Lamp 2
    { "H6046",   { 20,  GOVEE_ZONE_SPLIT_BARS, 0,  10, true } },    // Govee RGBIC Gaming Light Bars - two 10-LED bars, right then left
    { "H612F",   { 12,  GOVEE_ZONE_LINEAR,     0,  0,  true } },    // Govee Strip Light S (3m)
    { "H619A",   { 20,  GOVEE_ZONE_LINEAR,     0,  0,  true } },    // Govee RGBIC Led Strip Lights
    { "H70B1",   { 20,  GOVEE_ZONE_LINEAR,     0,  0,  true } },    // Govee LED Curtain Lights
    { "H607C",   { 174, GOVEE_ZONE_LINEAR,     0,  0,  true } },    // Govee Floor Lamp 2
};

enum
{
    GOVEE_MODE_STATIC         = 0,
    GOVEE_MODE_DIRECT         = 1,
};

RGBController_Govee::RGBController_Govee(GoveeController* controller_ptr)
{
    controller              = controller_ptr;
    last_static_color       = 0;
    last_static_brightness  = 0;
    razer_supported         = false;
    static_initialized      = false;

    name        = "Govee " + controller->GetSku();
    vendor      = "Govee";
    type        = DEVICE_TYPE_LIGHT;
    description = "Govee Device";
    location    = controller->GetLocation();
    version     = controller->GetVersion();

    mode Static;
    Static.name           = "Static";
    Static.value          = GOVEE_MODE_STATIC;
    Static.flags          = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.brightness_min = 0;
    Static.brightness_max = 100;
    Static.brightness     = 100;
    Static.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min     = 1;
    Static.colors_max     = 1;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Direct;
    Direct.name           = "Direct";
    Direct.value          = GOVEE_MODE_DIRECT;
    Direct.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.brightness_min = 0;
    Direct.brightness_max = 100;
    Direct.brightness     = 100;
    Direct.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();

    keepalive_thread_run    = 1;
    last_update_time        = std::chrono::steady_clock::now();
    keepalive_thread        = new std::thread(&RGBController_Govee::KeepaliveThread, this);
}

RGBController_Govee::~RGBController_Govee()
{
    Shutdown();

    keepalive_thread_run = 0;
    keepalive_thread->join();
    delete keepalive_thread;

    delete controller;
}

void RGBController_Govee::SetupZones()
{
    GoveeDeviceInfo hw = { GOVEE_FALLBACK_LED_COUNT, GOVEE_ZONE_LINEAR, 0, 0, false };

    std::map<std::string, GoveeDeviceInfo>::iterator it = govee_device_info.find(controller->GetSku());
    if(it != govee_device_info.end())
    {
        hw = it->second;
        razer_supported = hw.supports_razer;
    }

    zone strip;
    strip.leds_count            = hw.led_count;
    strip.leds_min              = hw.led_count;
    strip.leds_max              = hw.led_count;

    switch(hw.layout)
    {
        case GOVEE_ZONE_LINEAR:
        {
            strip.name              = "Govee Strip";
            strip.type              = ZONE_TYPE_LINEAR;
            break;
        }

        case GOVEE_ZONE_SPLIT_BARS:
        {
            strip.name              = "Light Bars";
            strip.type              = ZONE_TYPE_MATRIX;

            unsigned int bar_leds   = hw.led_count / 2;
            unsigned int width      = hw.gap_columns + 2;
            unsigned int height     = bar_leds;

            strip.matrix_map.Set(height, width, NULL);

            /*-------------------------------------------------*\
            | The two bars stand either side of a monitor, so   |
            | the left bar takes the first column and the right |
            | bar the last one.  The columns between them are   |
            | empty and represent the gap across the desk,      |
            | which keeps horizontal effects moving at the same |
            | rate as they appear to the eye.                   |
            \*-------------------------------------------------*/
            for(unsigned int y = 0; y < height; y++)
            {
                for(unsigned int x = 0; x < width; x++)
                {
                    strip.matrix_map.map[(y * width) + x] = GOVEE_MATRIX_LED_NONE;
                }

                strip.matrix_map.map[(y * width)]                 = bar_leds + y;
                strip.matrix_map.map[(y * width) + (width - 1)]   = y;
            }
            break;
        }

        case GOVEE_ZONE_SERPENTINE:
        default:
        {
            strip.name              = "Govee Matrix";
            strip.type              = ZONE_TYPE_MATRIX;

            /*-------------------------------------------------*\
            | matrix_row_len is only meaningful for this layout |
            | - guard a table entry that forgets to set it so a |
            | bad row degrades to a single column instead of    |
            | dividing by zero.                                 |
            \*-------------------------------------------------*/
            unsigned int width      = (hw.matrix_row_len > 0) ? hw.matrix_row_len : 1;
            unsigned int height     = hw.led_count / width;

            strip.matrix_map.height = height;
            strip.matrix_map.width  = width;
            strip.matrix_map.map.resize(hw.led_count);

            /*-------------------------------------------------*\
            | LEDs indexed bottom to top, alternating clockwise |
            | and counterclockwise for each row (e.g. H6022).   |
            \*-------------------------------------------------*/
            for(unsigned int y = 0; y < height; y++)
            {
                /*---------------------------------------------*\
                | LEDs numbered bottom to top, opposite of      |
                | matrix clockwise and counterclockwise for     |
                | each row.                                     |
                \*---------------------------------------------*/
                unsigned int led_y = (height - 1) - y;

                for(unsigned int x = 0; x < width; x++)
                {
                    /*-----------------------------------------*\
                    | LED is right-to-left for even rows,       |
                    | including first one                       |
                    \*-----------------------------------------*/
                    unsigned int led_x = led_y & 1 ? x : (width - 1) - x;
                    strip.matrix_map.map[y * width + x] = led_y * width + led_x;
                }
            }
            break;
        }
    }
    zones.push_back(strip);

    for(std::size_t led_idx = 0; led_idx < strip.leds_count; led_idx++)
    {
        led strip_led;

        if(hw.layout == GOVEE_ZONE_SPLIT_BARS)
        {
            unsigned int bar_leds = hw.led_count / 2;

            if(led_idx < bar_leds)
            {
                strip_led.name  = "Right Bar LED " + std::to_string(led_idx + 1);
            }
            else
            {
                strip_led.name  = "Left Bar LED " + std::to_string(led_idx - (bar_leds - 1));
            }
        }
        else
        {
            strip_led.name      = "Govee LED " + std::to_string(led_idx);
        }

        leds.push_back(strip_led);
    }

    SetupColors();
}

void RGBController_Govee::DeviceConfigureZone(int zone_idx)
{
    if(zones[zone_idx].type == ZONE_TYPE_MATRIX)
    {
        return;
    }

    if((size_t)zone_idx < zones.size())
    {
        leds.clear();
        leds.resize(zones[zone_idx].leds_count);
        for(unsigned int i = 0; i < zones[zone_idx].leds_count; i++)
        {
            leds[i].name = "Govee LED " + std::to_string(i);
        }

        SetupColors();      /* re-sync color buffers with LED count */
        DeviceUpdateLEDs(); /* push an updated frame */
    }
}

void RGBController_Govee::DeviceUpdateLEDs()
{
    if(colors.empty())
    {
        return;
    }

    const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    last_update_time = now;

    const unsigned int brightness = modes[active_mode].brightness;

    if(razer_supported)
    {
        controller->SendRazerData(&colors[0], (unsigned int)colors.size());
        controller->SetBrightness(brightness);
    }
    else
    {
        if(!static_initialized)
        {
            controller->SetPower(true);
        }

        controller->SetColor
            (
            RGBGetRValue(colors[0]),
            RGBGetGValue(colors[0]),
            RGBGetBValue(colors[0])
            );

        controller->SetBrightness(brightness);

        last_static_color       = colors[0];
        last_static_brightness  = brightness;
        static_initialized      = true;
    }
}

void RGBController_Govee::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_Govee::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_Govee::DeviceUpdateMode()
{
    if(modes[active_mode].value == GOVEE_MODE_STATIC)
    {
        UpdateStatic(false);
    }
    else if(modes[active_mode].value == GOVEE_MODE_DIRECT)
    {
        if(razer_supported)
        {
            controller->SendRazerEnable();
        }

        last_update_time = std::chrono::steady_clock::now();
        DeviceUpdateLEDs();
    }
}

void RGBController_Govee::UpdateStatic(bool force)
{
    last_update_time = std::chrono::steady_clock::now();

    const RGBColor color          = modes[active_mode].colors[0];
    const unsigned int brightness = modes[active_mode].brightness;

    if(force || !static_initialized || (color != last_static_color))
    {
        controller->SetPower(true);
        controller->SetColor(RGBGetRValue(color), RGBGetGValue(color), RGBGetBValue(color));
        last_static_color       = color;
    }

    if(force || !static_initialized || (brightness != last_static_brightness))
    {
        controller->SetBrightness(brightness);
        last_static_brightness = brightness;
    }

    static_initialized = true;
}

void RGBController_Govee::KeepaliveThread()
{
    while(keepalive_thread_run.load())
    {
        const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

        if((now - last_update_time) > std::chrono::seconds(30))
        {
            if(modes[active_mode].value == GOVEE_MODE_STATIC)
            {
                UpdateStatic(true);
            }
            else if(modes[active_mode].value == GOVEE_MODE_DIRECT)
            {
                /*-----------------------------------------------------*\
                | Direct/per-LED mode has no other periodic refresh -   |
                | SendRazerEnable() only fires once, at mode entry.     |
                | Re-assert it here so a long gap between real updates  |
                | (e.g. idle time between plugin-driven alerts) can't   |
                | let the device's external-control session time out    |
                | and fall back to its own firmware default.            |
                \*-----------------------------------------------------*/
                if(razer_supported)
                {
                    controller->SendRazerEnable();
                }

                DeviceUpdateLEDs();
            }
        }

        std::this_thread::sleep_for(10s);
    }
}
