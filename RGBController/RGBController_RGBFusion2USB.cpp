/*-----------------------------------------*\
|  RGBController_RGBFusion2USB.cpp          |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Gigabyte RGB Fusion 2.0 USB Driver       |
|                                           |
|  jackun 1/8/2020                          |
\*-----------------------------------------*/

#include "RGBController_RGBFusion2USB.h"
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

RGBController_RGBFusion2USB::RGBController_RGBFusion2USB(RGBFusion2USBController* controller_ptr)
{
    controller = controller_ptr;

    name        = "RGB Fusion 2 Controller";
    type        = DEVICE_TYPE_MOTHERBOARD;
    description = controller->GetDeviceName();
    version     = controller->GetFWVersion();
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerial();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0xFFFF;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

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

    SetupZones();
}

void RGBController_RGBFusion2USB::SetupZones()
{
    /*-------------------------------------------------*\
    | Only set LED count on the first run               |
    \*-------------------------------------------------*/
    bool first_run = false;

    if(zones.size() == 0)
    {
        first_run = true;
    }

    /*---------------------------------------------------------*\
    | Look up channel map based on device name                  |
    \*---------------------------------------------------------*/
    KnownChannels::const_iterator it = known_channels.find(controller->GetDeviceName());
    if (it == known_channels.end())
    {
        it = known_channels.find("Generic");
    }

    /*---------------------------------------------------------*\
    | Get number of motherboard LEDs and set addressable LED    |
    | count                                                     |
    \*---------------------------------------------------------*/
    size_t mb_led_cnt = it->second[0].size();

    /*-------------------------------------------------*\
    | Clear any existing color/LED configuration        |
    \*-------------------------------------------------*/
    leds.clear();
    colors.clear();
    zones.resize(led_zones.size());

    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < led_zones.size(); zone_idx++)
    {
        zones[zone_idx].name           = led_zones[zone_idx];
        zones[zone_idx].type           = ZONE_TYPE_LINEAR;

        /*---------------------------------------------------------*\
        | Zone index 0 is motherboard LEDs and has fixed size       |
        \*---------------------------------------------------------*/
        if(zone_idx == ZONE_MB)
        {
            zones[zone_idx].leds_min   = mb_led_cnt;
            zones[zone_idx].leds_max   = mb_led_cnt;
            zones[zone_idx].leds_count = mb_led_cnt;
        }
        else
        {
            zones[zone_idx].leds_min   = 0;
            zones[zone_idx].leds_max   = 1024;

            if(first_run)
            {
                zones[zone_idx].leds_count = 0;
            }

            controller->SetLedCount(zones[zone_idx].leds_count);
            controller->DisableBuiltinEffect(0, 0x3);
        }
    }

    /*---------------------------------------------------------*\
    | Set up LEDs                                               |
    \*---------------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        for(std::size_t led_idx = 0; led_idx < zones[zone_idx].leds_count; led_idx++)
        {
            led new_led;

            if( it != known_channels.end() && zone_idx < it->second.size() && led_idx < it->second[zone_idx].size())
            {
                new_led.name = it->second[zone_idx][led_idx].name;
            }
            else
            {
                new_led.name = zones[zone_idx].name + " LED ";
                new_led.name.append(std::to_string(led_idx));
            }

            new_led.value = zone_idx;

            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_RGBFusion2USB::ResizeZone(int zone, int new_size)
{
    if(((unsigned int)new_size >= zones[zone].leds_min) && ((unsigned int)new_size <= zones[zone].leds_max))
    {
        zones[zone].leds_count = new_size;

        SetupZones();
    }
}

void RGBController_RGBFusion2USB::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_RGBFusion2USB::UpdateLEDs()
{
    for(size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        UpdateZoneLEDs(zone_idx);
    }
}

void RGBController_RGBFusion2USB::UpdateZoneLEDs(int zone)
{
    /*---------------------------------------------------------*\
    | Get mode parameters                                       |
    \*---------------------------------------------------------*/
    bool    random      = (modes[active_mode].color_mode == MODE_COLORS_RANDOM);
    int     mode_value  = (modes[active_mode].value);

    KnownChannels::const_iterator it = known_channels.find(controller->GetDeviceName());

    if (it == known_channels.end())
    {
        it = known_channels.find("Generic");
    }

    if (it == known_channels.end() || zone >= (int)it->second.size())
    {
        return;
    }

    /*---------------------------------------------------------*\
    | Set motherboard LEDs                                      |
    \*---------------------------------------------------------*/
    if(zone == ZONE_MB)
    {
        /*---------------------------------------------------------*\
        | Motherboard LEDs always use effect mode, so use static for|
        | direct mode                                               |
        \*---------------------------------------------------------*/
        if(mode_value == 0xFFFF)
        {
            mode_value = EFFECT_STATIC;
        }
        
        for(size_t led_idx = 0; led_idx < zones[zone].leds_count; led_idx++)
        {
            unsigned char red = RGBGetRValue(zones[zone].colors[led_idx]);
            unsigned char grn = RGBGetGValue(zones[zone].colors[led_idx]);
            unsigned char blu = RGBGetBValue(zones[zone].colors[led_idx]);

            controller->SetLEDEffect(it->second[zone][led_idx].header, modes[active_mode].value, modes[active_mode].speed, random, red, grn, blu);
        }
        
        controller->ApplyEffect();
    }
    /*---------------------------------------------------------*\
    | Set strip LEDs                                            |
    \*---------------------------------------------------------*/
    else
    {
        /*---------------------------------------------------------*\
        | Direct mode                                               |
        \*---------------------------------------------------------*/
        if(mode_value == 0xFFFF)
        {
            if(zones[zone].leds_count > 0)
            {
                unsigned char hdr = 0;
                if(zone == ZONE_STRIP1)
                {
                    hdr = HDR_D_LED1_RGB;
                }
                else if(zone == ZONE_STRIP2)
                {
                    hdr = HDR_D_LED2_RGB;
                }

                controller->DisableBuiltinEffect(1, zone == 1 ? 0x01 : 0x02);
                controller->SetStripColors(hdr, zones[zone].colors, zones[zone].leds_count);
            }
        }
        /*---------------------------------------------------------*\
        | Effect mode                                               |
        \*---------------------------------------------------------*/
        else
        {
            if(it->second[zone].size())
            {
                unsigned char red = RGBGetRValue(zones[zone].colors[0]);
                unsigned char grn = RGBGetGValue(zones[zone].colors[0]);
                unsigned char blu = RGBGetBValue(zones[zone].colors[0]);
                
                int hdr = it->second[zone].front().header;

                // apply built-in effects to LED strips
                controller->DisableBuiltinEffect(0, zone == 1 ? 0x01 : 0x02);
                controller->SetLEDEffect(hdr, modes[active_mode].value, modes[active_mode].speed, random, red, grn, blu);
                controller->ApplyEffect();
            }
        }
    }
}

void RGBController_RGBFusion2USB::UpdateSingleLED(int led)
{
    /*---------------------------------------------------------*\
    | Get mode parameters                                       |
    \*---------------------------------------------------------*/
    bool            random      = (modes[active_mode].color_mode == MODE_COLORS_RANDOM);
    int             mode_value  = (modes[active_mode].value);
    unsigned int    zone_idx    = leds[led].value;

    /*---------------------------------------------------------*\
    | Set motherboard LEDs                                      |
    \*---------------------------------------------------------*/
    if(zone_idx == ZONE_MB)
    {
        int header = led;
        auto it = known_channels.find(controller->GetDeviceName());
        if (it == known_channels.end() || it->second.size() == 0)
            header = it->second[ZONE_MB][led].header;

        /*---------------------------------------------------------*\
        | Motherboard LEDs always use effect mode, so use static for|
        | direct mode                                               |
        \*---------------------------------------------------------*/
        if(mode_value == 0xFFFF)
        {
            mode_value = EFFECT_STATIC;
        }
        
        unsigned char red = RGBGetRValue(colors[led]);
        unsigned char grn = RGBGetGValue(colors[led]);
        unsigned char blu = RGBGetBValue(colors[led]);

        controller->SetLEDEffect(header, modes[active_mode].value, modes[active_mode].speed, random, red, grn, blu);
        controller->ApplyEffect();
    }
    /*---------------------------------------------------------*\
    | Set strip LEDs                                            |
    \*---------------------------------------------------------*/
    else
    {
        UpdateZoneLEDs(zone_idx);
    }
}

void RGBController_RGBFusion2USB::UpdateMode()
{
    // XXX Comment out to allow each zone or motherboard LED have different modes
    UpdateLEDs();
}
