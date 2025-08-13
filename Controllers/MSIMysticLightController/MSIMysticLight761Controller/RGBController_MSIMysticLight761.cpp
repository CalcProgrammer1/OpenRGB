/*---------------------------------------------------------*\
| MSIMysticLight185Controller.cpp                           |
|                                                           |
|   RGBController for MSI Mystic Light 761-byte motherboard |
|                                                           |
|                                                           |
|   rom4ster                                    11 Jun 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_MSIMysticLight761.h"
#include "LogManager.h"

struct ZoneDescription
{
    std::string name;
    MSI_ZONE    zone_type;
};

#define NUMOF_ZONES     (sizeof(led_zones) / sizeof(ZoneDescription))

const ZoneDescription led_zones[] =
{
    ZoneDescription{ "JAF",         MSI_ZONE_JAF     },
    ZoneDescription{ "JARGB 1",     MSI_ZONE_JARGB_1 },
    ZoneDescription{ "JARGB 2",     MSI_ZONE_JARGB_2 },
    ZoneDescription{ "JARGB 3",     MSI_ZONE_JARGB_3 },
};

static std::vector<const ZoneDescription*> zone_description;

static int IndexOfZoneForType(MSI_ZONE zone_type)
{
    for(size_t i = 0; i < zone_description.size(); ++i)
    {
        if(zone_description[i]->zone_type == zone_type)
        {
            return (int)i;
        }
    }

    return -1;
}

RGBController_MSIMysticLight761::RGBController_MSIMysticLight761
    (
        MSIMysticLight761Controller* controller_ptr
    )
{
    controller  = controller_ptr;

    name        = controller->GetDeviceName();
    vendor      = "MSI";
    type        = DEVICE_TYPE_MOTHERBOARD;
    description = "MSI Mystic Light Device (761-byte)";
    version     = controller->GetFWVersion();
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerial();

    const std::vector<MSI_ZONE>* supported_zones = controller->GetSupportedZones();

    for(std::size_t i = 0; i < supported_zones->size(); ++i)
    {
        for(std::size_t j = 0; j < NUMOF_ZONES; ++j)
        {
            if(led_zones[j].zone_type == (*supported_zones)[i])
            {
                zone_description.push_back(&led_zones[j]);
                break;
            }
        }
    }

    last_resizable_zone = MSI_ZONE_NONE;
    SetupModes();
    SetupZones();
    active_mode         = GetDeviceMode();
}

RGBController_MSIMysticLight761::~RGBController_MSIMysticLight761()
{
    zone_description.clear();
    delete controller;
}

int RGBController_MSIMysticLight761::GetDeviceMode()
{
    MSI_MODE mode = controller->GetMode();

    for(unsigned int i = 0; i < modes.size(); ++i)
    {
        if(mode == modes[i].value)
        {
            return i;
        }
    }

    return 0;
}

void RGBController_MSIMysticLight761::SetupZones()
{
    /*-------------------------------------------------*\
    | Clear any existing color/LED configuration        |
    \*-------------------------------------------------*/
    leds.clear();
    colors.clear();

    bool first_run = false;

    if(zones.size() == 0)
    {
        first_run = true;
    }

    if(first_run)
    {
        /*---------------------------------------------------------*\
        | Set up zones                                              |
        \*---------------------------------------------------------*/
        for(std::size_t zone_idx = 0; zone_idx < zone_description.size(); ++zone_idx)
        {
            const ZoneDescription* zd = zone_description[zone_idx];

            zone new_zone;

            new_zone.name           = zd->name;
            new_zone.flags          = 0;

            int maxLeds = (int)controller->GetMaxDirectLeds(zd->zone_type);

            /*-------------------------------------------------*\
            | This is a fixed size zone                         |
            \*-------------------------------------------------*/
            if(((zd->zone_type != MSI_ZONE_J_RAINBOW_1)
                 && (zd->zone_type != MSI_ZONE_J_RAINBOW_2)
                 && (zd->zone_type != MSI_ZONE_J_RAINBOW_3)
                 && (zd->zone_type != MSI_ZONE_JAF)
                 && (zd->zone_type != MSI_ZONE_JARGB_1)
                 && (zd->zone_type != MSI_ZONE_JARGB_2)
                 && (zd->zone_type != MSI_ZONE_JARGB_3)
                 && (zd->zone_type != MSI_ZONE_J_CORSAIR)))
            {
                new_zone.leds_min   = maxLeds;
                new_zone.leds_max   = maxLeds;
                new_zone.leds_count = maxLeds;
            }
            /*--------------------------------------------------\
            | This is a resizable zone on a board that does not |
            | support per-LED direct mode                       |
            \*-------------------------------------------------*/
            else if(controller->GetSupportedDirectMode() == MSIMysticLight761Controller::DIRECT_MODE_ZONE_BASED)
            {
                new_zone.leds_min   = 0;
                new_zone.leds_max   = 30;//maxLeds;
                new_zone.leds_count = 0;
                last_resizable_zone = zd->zone_type;
                new_zone.flags     |= ZONE_FLAG_RESIZE_EFFECTS_ONLY;
            }
            /*--------------------------------------------------\
            | This is a resizable zone on a board that does     |
            | support per-LED direct mode                       |
            \*-------------------------------------------------*/
            else
            {
                new_zone.leds_min   = 0;
                new_zone.leds_max   = maxLeds;
                new_zone.leds_count = 0;
                last_resizable_zone = zd->zone_type;
            }

            /*-------------------------------------------------*\
            | Determine zone type based on max number of LEDs   |
            \*-------------------------------------------------*/
            if((new_zone.leds_max == 1) || (new_zone.flags & ZONE_FLAG_RESIZE_EFFECTS_ONLY))
            {
                new_zone.type       = ZONE_TYPE_SINGLE;
            }
            else
            {
                new_zone.type       = ZONE_TYPE_LINEAR;
            }

            new_zone.matrix_map     = NULL;

            zones.push_back(new_zone);
        }
    }

    /*---------------------------------------------------------*\
    | Set up LEDs                                               |
    \*---------------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < zone_description.size(); ++zone_idx)
    {
        controller->SetCycleCount(zone_description[zone_idx]->zone_type, zones[zone_idx].leds_count);

        if((zones[zone_idx].flags & ZONE_FLAG_RESIZE_EFFECTS_ONLY) == 0)
        {
            for(std::size_t led_idx = 0; led_idx < zones[zone_idx].leds_count; ++led_idx)
            {
                led new_led;

                new_led.name = zones[zone_idx].name;

                if(zones[zone_idx].leds_count > 1)
                {
                    new_led.name.append(" LED " + std::to_string(led_idx + 1));
                }

                new_led.value = zone_description[zone_idx]->zone_type;
                leds.push_back(new_led);
            }
        }
        else if(zones[zone_idx].leds_count > 0)
        {
            led new_led;

            new_led.name = zones[zone_idx].name;

            new_led.value = zone_description[zone_idx]->zone_type;
            leds.push_back(new_led);
        }
    }

    SetupColors();
}


void RGBController_MSIMysticLight761::ResizeZone
    (
        int zone,
        int new_size
        )
{
    if((size_t)zone >= zones.size())
    {
        return;
    }

    if(((unsigned int)new_size >= zones[zone].leds_min) && ((unsigned int)new_size <= zones[zone].leds_max))
    {
        zones[zone].leds_count = new_size;
        SetupZones();

        if(zone_description[zone]->zone_type == last_resizable_zone)
        {
            GetDeviceConfig();
            last_resizable_zone = MSI_ZONE_NONE;
        }
    }
}

void RGBController_MSIMysticLight761::DeviceUpdateLEDs()
{
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); ++zone_idx)
    {
        for(int led_idx = zones[zone_idx].leds_count - 1; led_idx >= 0; led_idx--)
        {
            UpdateLed((int)zone_idx, led_idx);
        }
    }
    controller->Update((modes[active_mode].flags & MODE_FLAG_AUTOMATIC_SAVE) != 0);
}

void RGBController_MSIMysticLight761::UpdateZoneLEDs(int zone)
{
    for(int led_idx = zones[zone].leds_count - 1; led_idx >= 0; led_idx--)
    {
        UpdateLed(zone, led_idx);
    }
    controller->Update((modes[active_mode].flags & MODE_FLAG_AUTOMATIC_SAVE) != 0);
}

void RGBController_MSIMysticLight761::UpdateSingleLED
    (
        int led
    )
{
    int zone_index = IndexOfZoneForType((MSI_ZONE)leds[led].value);

    if(zone_index == -1)
    {
        LOG_DEBUG("[%s]: could not find zone for type %d", controller->GetDeviceName().c_str(), leds[led].value);
        return;
    }

    int led_index = led - zones[zone_index].start_idx;
    UpdateLed(zone_index, led_index);
    controller->Update((modes[active_mode].flags & MODE_FLAG_AUTOMATIC_SAVE) != 0);
}

void RGBController_MSIMysticLight761::DeviceUpdateMode()
{
    if(modes[active_mode].value == MSI_MODE_DIRECT_DUMMY)
    {
        controller->SetDirectMode(true);
    }
    else
    {
        controller->SetDirectMode(false);
        DeviceUpdateLEDs();
    }
}

void RGBController_MSIMysticLight761::DeviceSaveMode()
{
    controller->Update(true);
}

void RGBController_MSIMysticLight761::SetupModes()
{
    if(controller->GetSupportedDirectMode() != MSIMysticLight761Controller::DIRECT_MODE_DISABLED)
    {
        SetupMode("Direct",                 MSI_MODE_DIRECT_DUMMY,                  MODE_FLAG_HAS_PER_LED_COLOR);
    }
}

void RGBController_MSIMysticLight761::UpdateLed
    (
        int zone,
        int led
    )
{
    unsigned char red = RGBGetRValue(zones[zone].colors[led]);
    unsigned char grn = RGBGetGValue(zones[zone].colors[led]);
    unsigned char blu = RGBGetBValue(zones[zone].colors[led]);

    if(controller->IsDirectModeActive())
    {
        controller->SetLedColor((MSI_ZONE)(zones[zone].leds[led].value), led, red, grn, blu);
    }
    else
    {
        if(led == 0)
        {
            bool           random     = modes[active_mode].color_mode == MODE_COLORS_RANDOM;
            MSI_MODE       mode       = (MSI_MODE)modes[active_mode].value;
            MSI_SPEED      speed      = (MSI_SPEED)modes[active_mode].speed;
            MSI_BRIGHTNESS brightness = (MSI_BRIGHTNESS)modes[active_mode].brightness;

            controller->SetMode((MSI_ZONE)zones[zone].leds[led].value, mode, speed, brightness, random);
            controller->SetZoneColor((MSI_ZONE)zones[zone].leds[led].value, red, grn, blu, red, grn, blu);
        }
    }
}

void RGBController_MSIMysticLight761::SetupMode
    (
        const char      *name,
        MSI_MODE        mod,
        unsigned int    flags
    )
{
    mode Mode;
    Mode.name   = name;
    Mode.value  = mod;
    Mode.flags  = flags;

    if(flags & MODE_FLAG_HAS_PER_LED_COLOR)
    {
        Mode.color_mode = MODE_COLORS_PER_LED;
    }
    else
    {
        Mode.color_mode = MODE_COLORS_RANDOM;
    }

    if(flags & MODE_FLAG_HAS_SPEED)
    {
        Mode.speed      = MSI_SPEED_MEDIUM;
        Mode.speed_max  = MSI_SPEED_HIGH;
        Mode.speed_min  = MSI_SPEED_LOW;
    }
    else
    {
        /*---------------------------------------------------------*\
        | For modes without speed this needs to be set to avoid     |
        | bad values in the saved profile which in turn corrupts    |
        | the brightness calculation when loading the profile       |
        \*---------------------------------------------------------*/
        Mode.speed      = 0;
        Mode.speed_max  = 0;
        Mode.speed_min  = 0;
    }

    if(flags & MODE_FLAG_HAS_BRIGHTNESS)
    {
        Mode.brightness      = MSI_BRIGHTNESS_LEVEL_100;
        Mode.brightness_max  = MSI_BRIGHTNESS_LEVEL_100;
        Mode.brightness_min  = MSI_BRIGHTNESS_OFF;
    }
    else
    {
        Mode.brightness      = MSI_BRIGHTNESS_LEVEL_100;
        Mode.brightness_max  = MSI_BRIGHTNESS_LEVEL_100;
        Mode.brightness_min  = MSI_BRIGHTNESS_LEVEL_100;
    }

    modes.push_back(Mode);
}

void RGBController_MSIMysticLight761::GetDeviceConfig()
{
    if(controller->GetMode() != MSI_MODE_DIRECT_DUMMY)
    {
        MSI_MODE       mode;
        MSI_SPEED      speed;
        MSI_BRIGHTNESS brightness;
        bool           rainbow;
        unsigned int   color;

        for(size_t i = 0; i < zone_description.size(); ++i)
        {
            controller->GetMode(zone_description[i]->zone_type, mode, speed, brightness, rainbow, color);

            if(zones[i].colors != nullptr)
            {
                for(size_t j = 0; j < zones[i].leds_count; ++j)
                {
                    zones[i].colors[j] = color;
                }
            }
        }

        controller->GetMode(zone_description[0]->zone_type, mode, speed, brightness, rainbow, color);

        for(size_t i = 0; i < modes.size(); ++i)
        {
            if(mode == modes[i].value)
            {
                if(modes[i].flags & MODE_FLAG_HAS_SPEED)
                {
                    modes[i].speed = speed;
                }
                if(modes[i].flags & MODE_FLAG_HAS_BRIGHTNESS)
                {
                    modes[i].brightness = brightness;
                }
                if(rainbow)
                {
                    if(modes[i].flags & (MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR))
                    {
                        if(rainbow)
                        {
                            modes[i].color_mode = MODE_COLORS_RANDOM;
                        }
                        else
                        {
                            modes[i].color_mode = MODE_COLORS_PER_LED;
                        }
                    }
                }
                break;
            }
        }
    }
}
