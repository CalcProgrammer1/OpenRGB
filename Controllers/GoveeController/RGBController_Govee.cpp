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
#include <exception>
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

struct GoveeDeviceEntry
{
    const char*     sku;
    GoveeDeviceInfo info;
};

#define GOVEE_FALLBACK_LED_COUNT                     1
#define GOVEE_MATRIX_LED_NONE                       0xFFFFFFFF

static const GoveeDeviceEntry govee_device_info[] =
{
    { "H6022",   { 132, GOVEE_ZONE_SERPENTINE, 12, 0,  true } },    // Govee Smart Table Lamp 2
    { "H6046",   { 20,  GOVEE_ZONE_SPLIT_BARS, 0,  10, true } },    // Govee RGBIC Gaming Light Bars - two 10-LED bars, right then left
    { "H612F",   { 12,  GOVEE_ZONE_LINEAR,     0,  0,  true } },    // Govee Strip Light S (3m)
    { "H6159",   { 1,   GOVEE_ZONE_LINEAR,     0,  0,  true } },    // Govee RGB Strip Light (single-color zone)
    { "H619A",   { 20,  GOVEE_ZONE_LINEAR,     0,  0,  true } },    // Govee RGBIC Led Strip Lights
    { "H70B1",   { 20,  GOVEE_ZONE_LINEAR,     0,  0,  true } },    // Govee LED Curtain Lights
    { "H607C",   { 174, GOVEE_ZONE_LINEAR,     0,  0,  true } },    // Govee Floor Lamp 2
    { "H6069",   { 20,  GOVEE_ZONE_LINEAR,     0,  0,  true } },    // Govee Mini Panel Lights (20-panel kit)
};

static GoveeDeviceInfo GetGoveeDeviceInfo(const std::string& sku)
{
    for(unsigned int idx = 0; idx < (sizeof(govee_device_info) / sizeof(govee_device_info[0])); idx++)
    {
        if(sku == govee_device_info[idx].sku)
        {
            return(govee_device_info[idx].info);
        }
    }

    return(GoveeDeviceInfo{ GOVEE_FALLBACK_LED_COUNT, GOVEE_ZONE_LINEAR, 1, 10, false });
}

enum
{
    GOVEE_MODE_STATIC         = 0,
    GOVEE_MODE_DIRECT         = 1,
};

RGBController_Govee::RGBController_Govee(GoveeController* controller_ptr)
{
    controller                      = controller_ptr;
    last_static_color               = 0;
    last_static_brightness          = 0;
    last_direct_brightness          = 0;
    razer_supported                 = false;
    razer_status_available          = controller->SupportsRazerStatus();
    gradient_enabled                = false;
    control_initialized             = false;
    razer_delay_ms                  = 50;
    configured_led_count            = 1;
    configured_matrix_width         = 1;
    configured_gap_columns          = 10;
    configured_layout               = GOVEE_ZONE_LINEAR;
    razer_active                    = controller->RazerInitiallyArmed();
    power_known                     = false;
    power_on                        = false;
    static_initialized              = false;
    direct_brightness_initialized   = false;

    name        = "Govee " + controller->GetSku();
    vendor      = "Govee";
    type        = DEVICE_TYPE_LIGHT;
    description = "Govee Device";
    location    = controller->GetLocation();
    version     = controller->GetVersion();
    serial      = controller->GetDeviceId();

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

    InitDeviceSpecificConfiguration();
    ApplyDeviceSpecificConfiguration();

    bool initial_on;
    unsigned int initial_brightness;
    RGBColor initial_color;
    if(controller->GetInitialState(initial_on, initial_brightness, initial_color))
    {
        modes[0].colors[0] = initial_color;
        modes[0].brightness = initial_on ? initial_brightness : 0;
        modes[1].brightness = modes[0].brightness;
        last_static_color = initial_color;
        last_static_brightness = initial_brightness;
        static_initialized = initial_on;
        power_known = true;
        power_on = initial_on;
    }

    keepalive_thread_run    = 1;
    last_update_time        = std::chrono::steady_clock::now();
    keepalive_thread        = new std::thread(&RGBController_Govee::KeepaliveThread, this);
}

RGBController_Govee::~RGBController_Govee()
{
    keepalive_thread_run = false;
    keepalive_cv.notify_one();
    keepalive_thread->join();
    delete keepalive_thread;
    Shutdown();

    if(razer_active && control_initialized)
    {
        controller->SendRazerDisable();
    }

    delete controller;
}

void RGBController_Govee::InitDeviceSpecificConfiguration()
{
    const GoveeDeviceInfo hw = GetGoveeDeviceInfo(controller->GetSku());

    nlohmann::json settings;
    nlohmann::json& schema = settings["schema"];
    nlohmann::json& config = settings["configuration"];

    schema["protocol"] =
    {
        { "title", "Direct Protocol" },
        { "type", "string" },
        { "description", "Auto enables Razer when the model is known or a valid B2 status response was detected. A B2 reply does not prove independent segment support." },
        { "enum", { "Auto", "LAN", "Razer" } },
        { "default", "Auto" },
        { "order", 0 }
    };
    config["protocol"] = "Auto";

    schema["segment_count"] =
    {
        { "title", "Razer Segment Count" },
        { "type", "integer" },
        { "description", "Logical RGB entries in a Razer frame (1-255); not automatically reported by firmware. LAN mode always uses one whole-device color." },
        { "minimum", 1 },
        { "maximum", 255 },
        { "default", hw.led_count },
        { "order", 1 }
    };
    config["segment_count"] = hw.led_count;

    const char* layout_name = hw.layout == GOVEE_ZONE_SERPENTINE ? "Serpentine"
                            : hw.layout == GOVEE_ZONE_SPLIT_BARS ? "Split Bars" : "Linear";
    schema["layout"] =
    {
        { "title", "Zone Layout" },
        { "type", "string" },
        { "description", "OpenRGB visualization only. Split Bars requires an even segment count." },
        { "enum", { "Linear", "Serpentine", "Split Bars" } },
        { "default", layout_name },
        { "order", 2 }
    };
    config["layout"] = layout_name;

    schema["matrix_width"] =
    {
        { "title", "Matrix Row Width" },
        { "type", "integer" },
        { "description", "Number of entries per serpentine row." },
        { "minimum", 1 },
        { "maximum", 255 },
        { "default", std::max(1u, hw.matrix_row_len) },
        { "order", 3 }
    };
    config["matrix_width"] = std::max(1u, hw.matrix_row_len);

    schema["gap_columns"] =
    {
        { "title", "Split-Bar Gap Columns" },
        { "type", "integer" },
        { "description", "Empty columns drawn between the two bars." },
        { "minimum", 0 },
        { "maximum", 100 },
        { "default", hw.gap_columns },
        { "order", 4 }
    };
    config["gap_columns"] = hw.gap_columns;

    schema["gradient"] =
    {
        { "title", "Razer Gradient" },
        { "type", "bool" },
        { "description", "Interpolate across segments (B0 gradient=1) rather than render discrete colors." },
        { "default", false },
        { "order", 5 }
    };
    config["gradient"] = false;

    const int default_delay = controller->GetSku() == "H6159" ? 700 : 50;
    schema["razer_delay_ms"] =
    {
        { "title", "Razer Startup Delay (ms)" },
        { "type", "integer" },
        { "description", "Delay between B1 enable and first B0 frame; applies on next entry to Direct." },
        { "minimum", 0 },
        { "maximum", 3000 },
        { "default", default_delay },
        { "order", 6 }
    };
    config["razer_delay_ms"] = default_delay;

    flags |= CONTROLLER_FLAG_MANUALLY_CONFIGURABLE_DEVICE_SPECIFIC;
    configuration = settings.dump();
}

static unsigned int GetGoveeUnsignedSetting(const nlohmann::json& settings,
                                            const char* key,
                                            unsigned int fallback,
                                            unsigned int low,
                                            unsigned int high)
{
    if(!settings.contains(key) || !settings[key].is_number_integer())
    {
        return(fallback);
    }

    try
    {
        const int value = settings[key].get<int>();
        return((unsigned int)std::max((int)low, std::min(value, (int)high)));
    }
    catch(const std::exception&)
    {
        return(fallback);
    }
}

void RGBController_Govee::ApplyDeviceSpecificConfiguration()
{
    const GoveeDeviceInfo hw = GetGoveeDeviceInfo(controller->GetSku());

    nlohmann::json settings;
    try
    {
        settings = nlohmann::json::parse(configuration).at("configuration");
    }
    catch(const std::exception&)
    {
        return;
    }

    configured_led_count    = GetGoveeUnsignedSetting(settings, "segment_count", hw.led_count, 1, 255);
    configured_matrix_width = GetGoveeUnsignedSetting(settings, "matrix_width", std::max(1u, hw.matrix_row_len), 1, 255);
    configured_gap_columns  = GetGoveeUnsignedSetting(settings, "gap_columns", hw.gap_columns, 0, 100);
    razer_delay_ms          = GetGoveeUnsignedSetting(settings, "razer_delay_ms", controller->GetSku() == "H6159" ? 700 : 50, 0, 3000);
    gradient_enabled        = settings.contains("gradient") && settings["gradient"].is_boolean()
                           ? settings["gradient"].get<bool>() : false;

    const std::string layout = settings.contains("layout") && settings["layout"].is_string()
                             ? settings["layout"].get<std::string>() : "Linear";
    configured_layout = layout == "Serpentine" ? GOVEE_ZONE_SERPENTINE
                      : layout == "Split Bars" ? GOVEE_ZONE_SPLIT_BARS
                      : GOVEE_ZONE_LINEAR;

    const std::string protocol = settings.contains("protocol") && settings["protocol"].is_string()
                               ? settings["protocol"].get<std::string>() : "Auto";
    const bool supports_auto = hw.supports_razer || razer_status_available;
    const bool allow_razer = protocol == "Razer" || ((protocol != "LAN") && supports_auto);

    if(razer_active && !allow_razer && control_initialized)
    {
        controller->SendRazerDisable();
        razer_active = false;
        power_known = false;
    }
    razer_supported = allow_razer;
    static_initialized = false;
    direct_brightness_initialized = false;
    SetupZones();

    if(control_initialized)
    {
        DeviceUpdateMode();
    }
}

void RGBController_Govee::DeviceUpdateDeviceSpecificConfiguration()
{
    ApplyDeviceSpecificConfiguration();
}

void RGBController_Govee::SetupZones()
{
    GoveeDeviceInfo hw = { configured_led_count, (GoveeZoneLayout)configured_layout,
                           configured_matrix_width, configured_gap_columns, razer_supported };

    if(!razer_supported)
    {
        hw.led_count = 1;
        hw.layout = GOVEE_ZONE_LINEAR;
    }
    else if((hw.layout == GOVEE_ZONE_SPLIT_BARS) && ((hw.led_count % 2) != 0))
    {
        hw.layout = GOVEE_ZONE_LINEAR;
    }

    zones.clear();
    leds.clear();

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
            unsigned int height     = (hw.led_count + width - 1) / width;

            strip.matrix_map.height = height;
            strip.matrix_map.width  = width;
            strip.matrix_map.map.resize(height * width, GOVEE_MATRIX_LED_NONE);

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
                    const unsigned int led_idx = led_y * width + led_x;
                    if(led_idx < hw.led_count)
                    {
                        strip.matrix_map.map[y * width + x] = led_idx;
                    }
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
    if((zone_idx < 0) || ((size_t)zone_idx >= zones.size()))
    {
        return;
    }

    if(zones[zone_idx].type == ZONE_TYPE_MATRIX)
    {
        return;
    }

    leds.clear();
    leds.resize(zones[zone_idx].leds_count);
    for(unsigned int i = 0; i < zones[zone_idx].leds_count; i++)
    {
        leds[i].name = "Govee LED " + std::to_string(i);
    }

    SetupColors();
    DeviceUpdateLEDs();
}

void RGBController_Govee::UpdatePower(unsigned int brightness)
{
    control_initialized = true;
    const bool enabled = (brightness > 0);

    if(!power_known || (power_on != enabled))
    {
        if(!enabled && razer_active)
        {
            controller->SendRazerDisable();
            razer_active = false;
        }

        controller->SetPower(enabled);
        power_known = true;
        power_on = enabled;

        if(!enabled)
        {
            static_initialized = false;
            direct_brightness_initialized = false;
        }
    }
}

void RGBController_Govee::UpdateLEDs()
{
    last_update_time = std::chrono::steady_clock::now();

    if(colors.empty())
    {
        return;
    }

    const unsigned int brightness = modes[active_mode].brightness;
    UpdatePower(brightness);

    if(brightness == 0)
    {
        return;
    }

    if(razer_supported)
    {
        if(!razer_active)
        {
            controller->SendRazerEnable();
            std::this_thread::sleep_for(std::chrono::milliseconds(razer_delay_ms));
            razer_active = true;
        }

        controller->SendRazerData(&colors[0], (unsigned int)colors.size(), gradient_enabled);

        if(!direct_brightness_initialized || (brightness != last_direct_brightness))
        {
            controller->SetBrightness(brightness);
            last_direct_brightness = brightness;
            direct_brightness_initialized = true;
        }
    }
    else
    {
        if(razer_active)
        {
            controller->SendRazerDisable();
            razer_active = false;
            power_known = false;
            UpdatePower(brightness);
        }

        if(!static_initialized || (colors[0] != last_static_color))
        {
            controller->SetColor(RGBGetRValue(colors[0]), RGBGetGValue(colors[0]), RGBGetBValue(colors[0]));
            last_static_color = colors[0];
        }

        if(!static_initialized || (brightness != last_static_brightness))
        {
            controller->SetBrightness(brightness);
            last_static_brightness = brightness;
        }

        static_initialized = true;
    }
}

void RGBController_Govee::DeviceUpdateLEDs()
{
    if(modes[active_mode].value == GOVEE_MODE_STATIC)
    {
        UpdateStatic(false);
    }
    else
    {
        UpdateLEDs();
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
        UpdateStatic(true);
    }
    else if(modes[active_mode].value == GOVEE_MODE_DIRECT)
    {
        direct_brightness_initialized = false;
        UpdateLEDs();
    }
}

void RGBController_Govee::UpdateStatic(bool force)
{
    last_update_time = std::chrono::steady_clock::now();

    const RGBColor color          = modes[active_mode].colors[0];
    const unsigned int brightness = modes[active_mode].brightness;

    if(razer_active)
    {
        controller->SendRazerDisable();
        razer_active = false;
        power_known = false;
        force = true;
    }

    UpdatePower(brightness);

    if(brightness == 0)
    {
        return;
    }

    if(force || !static_initialized || (color != last_static_color))
    {
        controller->SetColor(RGBGetRValue(color), RGBGetGValue(color), RGBGetBValue(color));
        last_static_color = color;
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
        AccessMutex.lock_shared();
        DeviceMutex.lock();

        const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

        if(control_initialized && (now - last_update_time) > std::chrono::seconds(30))
        {
            if(modes[active_mode].value == GOVEE_MODE_STATIC)
            {
                UpdateStatic(true);
            }
            else if(modes[active_mode].value == GOVEE_MODE_DIRECT)
            {
                UpdateLEDs();
            }
        }

        DeviceMutex.unlock();
        AccessMutex.unlock_shared();

        std::unique_lock<std::mutex> lock(keepalive_mutex);
        keepalive_cv.wait_for(lock, 10s, [this]
        {
            return(!keepalive_thread_run.load());
        });
    }
}
