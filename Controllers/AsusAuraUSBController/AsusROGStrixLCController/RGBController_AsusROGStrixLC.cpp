/*---------------------------------------------------------*\
| RGBController_AsusROGStrixLC.cpp                          |
|                                                           |
|   RGBController for ASUS Aura liquid cooler               |
|                                                           |
|   Chris M (Dr_No)                             17 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_AsusROGStrixLC.h"

/**------------------------------------------------------------------*\
    @name Asus ROG Strix Liquid Cooler
    @category Cooler
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectAsusAuraUSBROGStrixLC
    @comment
\*-------------------------------------------------------------------*/

RGBController_AsusROGStrixLC::RGBController_AsusROGStrixLC(AsusROGStrixLCController *controller_ptr)
{
    controller                  = controller_ptr;
    uint8_t speed               = ROGSTRIXLC_CONTROLLER_SPEED_NORMAL;

    name                        = controller->GetDeviceName();
    vendor                      = "ASUS";
    type                        = DEVICE_TYPE_COOLER;
    description                 = "ASUS Liquid Cooler including 120mm, 140mm, 240mm, 280mm and 360mm radiators.";
    version                     = KEY_EN_UNUSED;
    serial                      = KEY_EN_UNUSED;
    location                    = controller->GetLocation();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = ROGSTRIXLC_CONTROLLER_MODE_DIRECT;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                 = "Static";
    Static.value                = ROGSTRIXLC_CONTROLLER_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.colors_min           = 1;
    Static.colors_max           = 1;
    Static.colors.resize(Static.colors_max);
    Static.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = ROGSTRIXLC_CONTROLLER_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Breathing.colors_min        = 1;
    Breathing.colors_max        = 1;
    Breathing.colors.resize(Breathing.colors_max);
    Breathing.brightness_min    = ROGSTRIXLC_CONTROLLER_BRIGHTNESS_MIN;
    Breathing.brightness_max    = ROGSTRIXLC_CONTROLLER_BRIGHTNESS_MAX;
    Breathing.brightness        = ROGSTRIXLC_CONTROLLER_BRIGHTNESS_MAX;
    Breathing.speed_min         = ROGSTRIXLC_CONTROLLER_SPEED_SLOWEST;
    Breathing.speed_max         = ROGSTRIXLC_CONTROLLER_SPEED_FASTEST;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.speed             = speed;
    modes.push_back(Breathing);

    mode Flashing;
    Flashing.name               = "Flashing";
    Flashing.value              = ROGSTRIXLC_CONTROLLER_MODE_FLASHING;
    Flashing.flags              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Flashing.colors_min         = 1;
    Flashing.colors_max         = 1;
    Flashing.colors.resize(Flashing.colors_max);
    Flashing.brightness_min     = ROGSTRIXLC_CONTROLLER_BRIGHTNESS_MIN;
    Flashing.brightness_max     = ROGSTRIXLC_CONTROLLER_BRIGHTNESS_MAX;
    Flashing.brightness         = ROGSTRIXLC_CONTROLLER_BRIGHTNESS_MAX;
    Flashing.speed_min          = ROGSTRIXLC_CONTROLLER_SPEED_SLOWEST;
    Flashing.speed_max          = ROGSTRIXLC_CONTROLLER_SPEED_FASTEST;
    Flashing.color_mode         = MODE_COLORS_MODE_SPECIFIC;
    Flashing.speed              = speed;
    modes.push_back(Flashing);

    mode Spectrum;
    Spectrum.name               = "Spectrum";
    Spectrum.value              = ROGSTRIXLC_CONTROLLER_MODE_SPECTRUM;
    Spectrum.flags              = MODE_FLAG_HAS_SPEED;
    Spectrum.brightness_min     = ROGSTRIXLC_CONTROLLER_BRIGHTNESS_MIN;
    Spectrum.brightness_max     = ROGSTRIXLC_CONTROLLER_BRIGHTNESS_MAX;
    Spectrum.brightness         = ROGSTRIXLC_CONTROLLER_BRIGHTNESS_MAX;
    Spectrum.speed_min          = ROGSTRIXLC_CONTROLLER_SPEED_SLOWEST;
    Spectrum.speed_max          = ROGSTRIXLC_CONTROLLER_SPEED_FASTEST;
    Spectrum.color_mode         = MODE_COLORS_NONE;
    Spectrum.speed              = speed;
    modes.push_back(Spectrum);

    mode Rainbow;
    Rainbow.name                = "Rainbow";
    Rainbow.value               = ROGSTRIXLC_CONTROLLER_MODE_RAINBOW;
    Rainbow.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Rainbow.brightness_min      = ROGSTRIXLC_CONTROLLER_BRIGHTNESS_MIN;
    Rainbow.brightness_max      = ROGSTRIXLC_CONTROLLER_BRIGHTNESS_MAX;
    Rainbow.brightness          = ROGSTRIXLC_CONTROLLER_BRIGHTNESS_MAX;
    Rainbow.speed_min           = ROGSTRIXLC_CONTROLLER_SPEED_SLOWEST;
    Rainbow.speed_max           = ROGSTRIXLC_CONTROLLER_SPEED_FASTEST;
    Rainbow.color_mode          = MODE_COLORS_NONE;
    Rainbow.speed               = speed;
    modes.push_back(Rainbow);

    mode FlashAndDash;
    FlashAndDash.name           = "Flash and Dash";
    FlashAndDash.value          = ROGSTRIXLC_CONTROLLER_MODE_FLASHANDDASH;
    FlashAndDash.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    FlashAndDash.brightness_min = ROGSTRIXLC_CONTROLLER_BRIGHTNESS_MIN;
    FlashAndDash.brightness_max = ROGSTRIXLC_CONTROLLER_BRIGHTNESS_MAX;
    FlashAndDash.brightness     = ROGSTRIXLC_CONTROLLER_BRIGHTNESS_MAX;
    FlashAndDash.speed_min      = ROGSTRIXLC_CONTROLLER_SPEED_SLOWEST;
    FlashAndDash.speed_max      = ROGSTRIXLC_CONTROLLER_SPEED_FASTEST;
    FlashAndDash.color_mode     = MODE_COLORS_NONE;
    FlashAndDash.speed          = speed;
    modes.push_back(FlashAndDash);

    SetupZones();
}

RGBController_AsusROGStrixLC::~RGBController_AsusROGStrixLC()
{
    delete controller;
}

void RGBController_AsusROGStrixLC::SetupZones()
{
    /*-------------------------------------------------*\
    | Set up zones                                      |
    \*-------------------------------------------------*/
    LOG_DEBUG("[%s] - Get channel count: %i", name.c_str(), controller->GetChannelCount());

    zones.resize(controller->GetChannelCount());

    LOG_DEBUG("[%s] - Creating Zones and LEDs", name.c_str());
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        AuraDeviceInfo device_info = controller->GetAuraDevices()[zone_idx];
        LOG_INFO("[%s] %s Zone %i - Header Count %i LED Count %i FX %02X Direct %02X", name.c_str(),
                 ((device_info.device_type == AuraDeviceType::FIXED) ? "Fixed" : "Addressable"),
                 zone_idx, device_info.num_headers, device_info.num_leds, device_info.effect_channel, device_info.direct_channel);

        zones[zone_idx].name        = name + " Zone ";
        zones[zone_idx].name.append(std::to_string(zone_idx));
        zones[zone_idx].type        = ZONE_TYPE_LINEAR;
        zones[zone_idx].leds_min    = device_info.num_leds;
        zones[zone_idx].leds_max    = device_info.num_leds;
        zones[zone_idx].leds_count  = device_info.num_leds;

        for(unsigned int lp_idx = 0; lp_idx < zones[zone_idx].leds_count; lp_idx++)
        {
            led new_led;

            new_led.name    = zones[zone_idx].name;
            new_led.name.append(" LED " + std::to_string(lp_idx));
            new_led.value   = lp_idx;

            leds.push_back(new_led);
        }
    }

    LOG_DEBUG("[%s] - Device zones and LEDs set", name.c_str());
    SetupColors();
}

void RGBController_AsusROGStrixLC::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AsusROGStrixLC::DeviceUpdateLEDs()
{
    for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        UpdateZoneLEDs(zone_idx);
    }
}

void RGBController_AsusROGStrixLC::UpdateZoneLEDs(int zone)
{
    controller->SetLedsDirect( zones[zone].colors, zones[zone].leds_count );
}

void RGBController_AsusROGStrixLC::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(GetLED_Zone(led));
}

void RGBController_AsusROGStrixLC::DeviceUpdateMode()
{
    RGBColor colour         = (modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC) ? modes[active_mode].colors[0] : 0;

    controller->SetMode( modes[active_mode].value, modes[active_mode].speed, modes[active_mode].direction, colour );
}

int RGBController_AsusROGStrixLC::GetLED_Zone(int led_idx)
{
    for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        int zone_start  = zones[zone_idx].start_idx;
        int zone_end    = zone_start + zones[zone_idx].leds_count - 1;

        if(zone_start <= led_idx && zone_end >= led_idx)
        {
            return(zone_idx);
        }
    }

    return -1;
}
