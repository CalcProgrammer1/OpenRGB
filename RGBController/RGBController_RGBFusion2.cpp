/*-----------------------------------------*\
|  RGBController_RGBFusion2.cpp             |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Gigabyte RGB Fusion 2.0 Driver           |
|                                           |
|  jackun 1/8/2020                          |
\*-----------------------------------------*/

#include "RGBController_RGBFusion2.h"
#include <sstream>
#include <array>

enum
{
    ZONE_MB,
    ZONE_STRIP1,
    ZONE_STRIP2,
};

static const std::array<const char *, 3> led_zones
{
    "Motherboard",
    "LED Strip 1", // WS2812(B) strips
    "LED Strip 2",
};

static const KnownChannels known_channels
{
    {
        "Generic",
        {
            {
                { "Led 1", 0x20 },
                { "Led 2", 0x21 },
                { "Led 3", 0x22 },
                { "Led 4", 0x23 },
                { "Led 5", 0x24 },
                { "Led 6", 0x25 },
                { "Led 7", 0x26 },
                { "Led 8", 0x27 },
            },
            // Zone 1
            {
                { "LED Strip 1, LED 0", HDR_D_LED1 },
            },
            // Zone 2
            {
                { "LED Strip 2, LED 0", HDR_D_LED2 },
            }
        }
    },
    {
        "IT8297BX-GBX570",
        {
            // Zone 0
            {
                { "Back I/O",   HDR_BACK_IO },
                { "CPU",        HDR_CPU },
                { "PCIe",       HDR_PCIE },
                { "LED C1/C2",  HDR_LED_C1C2 }, // 12VGRB headers seem to be connected
            },
            // Zone 1
            {
                { "LED Strip 1, LED 0", HDR_D_LED1 },
            },
            // Zone 2
            {
                { "LED Strip 2, LED 0", HDR_D_LED2 },
            }
        }
    },
};

RGBController_RGBFusion2::RGBController_RGBFusion2(RGBFusion2Controller* controller_ptr)
{
    controller = controller_ptr;

    name = "RGB Fusion 2 Controller";
    type = DEVICE_TYPE_MOTHERBOARD;
    description = controller->GetDeviceName();
    version     = controller->GetFWVersion();
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerial();

    auto it = known_channels.find(controller->GetDeviceName());
    if (it == known_channels.end())
    {
        it = known_channels.find("Generic");
    }

    size_t mb_led_cnt = it->second[0].size();

    mode Static;
    Static.name       = "Static";
    Static.value      = EFFECT_STATIC;
    Static.flags      = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = EFFECT_PULSE;
    Breathing.flags      = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Breathing.speed_min  = 0;
    Breathing.speed_max  = 4;
    Breathing.color_mode = MODE_COLORS_PER_LED;
    Breathing.speed      = 2;
    modes.push_back(Breathing);

    mode Blinking;
    Blinking.name       = "Blinking";
    Blinking.value      = EFFECT_BLINKING;
    Blinking.flags      = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Blinking.speed_min  = 0;
    Blinking.speed_max  = 4;
    Blinking.color_mode = MODE_COLORS_PER_LED;
    Blinking.speed      = 2;
    modes.push_back(Blinking);

    mode ColorCycle;
    ColorCycle.name       = "Color Cycle";
    ColorCycle.value      = EFFECT_COLORCYCLE;
    ColorCycle.flags      = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    ColorCycle.speed_min  = 0;
    ColorCycle.speed_max  = 4;
    ColorCycle.color_mode = MODE_COLORS_NONE;
    ColorCycle.speed      = 2;
    modes.push_back(ColorCycle);

    mode Flashing;
    Flashing.name       = "Flashing";
    Flashing.value      = 10;
    Flashing.flags      = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Flashing.speed_min  = 0;
    Flashing.speed_max  = 4;
    Flashing.color_mode = MODE_COLORS_PER_LED;
    Flashing.speed      = 2;
    modes.push_back(Flashing);

    per_strip_led_cnt = 64; // TODO needs GUI option
    colors.resize(mb_led_cnt + per_strip_led_cnt * 2, 0x000021FF);
    controller->SetLedCount(per_strip_led_cnt);
    controller->DisableBuiltinEffect(0, 0x3);

    unsigned int led_idx = 0;
    std::stringstream ss;

    for(size_t i = 0; i < led_zones.size(); i++)
    {
        std::vector<int> new_zone_map;
        zone new_zone;
        new_zone.name = led_zones[i];
        new_zone.type = ZONE_TYPE_LINEAR; // TODO

        unsigned int zone_led_cnt = mb_led_cnt;
        if( i > 0 )
        {
            zone_led_cnt = per_strip_led_cnt;
        }

        for(unsigned int zone_led_idx = 0; zone_led_idx < zone_led_cnt; zone_led_idx++)
        {
            led new_led;

            if( it != known_channels.end() && i < it->second.size() && zone_led_idx < it->second[i].size())
            {
                new_led.name = it->second[i][zone_led_idx].name;
            }
            else
            {
                ss.clear(); ss.str("");
                ss << led_zones[i];
                if (zone_led_cnt > 1)
                {
                    ss << ", LED " << zone_led_idx;
                }
                new_led.name = ss.str();
            }

            leds.push_back(new_led);
            new_zone_map.push_back(led_idx);
            led_idx++;
        }

        new_zone.map.push_back(new_zone_map);
        zones.push_back(new_zone);
    }
}

void RGBController_RGBFusion2::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_RGBFusion2::UpdateLEDs()
{
    for(size_t i = 0; i < led_zones.size(); i++)
    {
        UpdateZoneLEDs(i);
    }
}

void RGBController_RGBFusion2::UpdateZoneLEDs(int zone)
{
    bool random = (modes[active_mode].color_mode == MODE_COLORS_RANDOM);

    auto it = known_channels.find(controller->GetDeviceName());

    if (it == known_channels.end())
    {
        it = known_channels.find("Generic");
    }

    if (it == known_channels.end() || zone >= (int)it->second.size())
    {
        return;
    }

    if(zone == ZONE_MB)
    {
        std::vector<int>& leds = zones[zone].map[0];

        for(size_t i = 0; i < leds.size(); i++)
        {
            unsigned char red = RGBGetRValue(colors[leds[i]]);
            unsigned char grn = RGBGetGValue(colors[leds[i]]);
            unsigned char blu = RGBGetBValue(colors[leds[i]]);

            controller->SetLEDEffect(it->second[zone][i].header, modes[active_mode].value, modes[active_mode].speed, random, red, grn, blu);
        }
        
        controller->ApplyEffect();
    }
    else // assuming other two zones are LED strips
    {
        int led = zones[zone].map[0][0];
        unsigned char red = RGBGetRValue(colors[led]);
        unsigned char grn = RGBGetGValue(colors[led]);
        unsigned char blu = RGBGetBValue(colors[led]);

        int hdr = it->second[zone].size() ? it->second[zone].front().header : led;

        // apply built-in effects to LED strips
        controller->DisableBuiltinEffect(0, zone == 1 ? 0x01 : 0x02);
        controller->SetLEDEffect(hdr, modes[active_mode].value, modes[active_mode].speed, random, red, grn, blu);
        controller->ApplyEffect();
    }
}

void RGBController_RGBFusion2::UpdateSingleLED(int led)
{
    bool random = (modes[active_mode].color_mode == MODE_COLORS_RANDOM);

    if (led >= zones[ZONE_STRIP1].map[0].front() && led <= zones[ZONE_STRIP1].map[0].back())
    {
        controller->SetStripColors(HDR_D_LED1_RGB,
                zones[ZONE_STRIP1].map[0].front(),
                zones[ZONE_STRIP1].map[0].back(),
                colors, led);
    }
    else if (led >= zones[ZONE_STRIP2].map[0].front() && led <= zones[ZONE_STRIP2].map[0].back())
    {
        controller->SetStripColors(HDR_D_LED2_RGB,
               zones[ZONE_STRIP2].map[0].front(),
               zones[ZONE_STRIP2].map[0].back(),
               colors, led);
    }
    else
    {
        int header = led;
        auto it = known_channels.find(controller->GetDeviceName());
        if (it == known_channels.end() || it->second.size() == 0)
            header = it->second[ZONE_MB][led].header;

        unsigned char red = RGBGetRValue(colors[led]);
        unsigned char grn = RGBGetGValue(colors[led]);
        unsigned char blu = RGBGetBValue(colors[led]);

        controller->SetLEDEffect(header, modes[active_mode].value, modes[active_mode].speed, random, red, grn, blu);
        controller->ApplyEffect();
    }
}

void RGBController_RGBFusion2::UpdateMode()
{
    // XXX Comment out to allow each zone or motherboard LED have different modes
    UpdateLEDs();
}
