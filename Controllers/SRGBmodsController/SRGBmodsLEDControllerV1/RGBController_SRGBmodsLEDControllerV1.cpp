/*---------------------------------------------------------*\
| RGBController_SRGBmodsLEDControllerV1.cpp                 |
|                                                           |
|   RGBController for SRGBmods LED Controller V1            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                30 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_SRGBmodsLEDControllerV1.h"

/**------------------------------------------------------------------*\
    @name SRGBmods LED Controller V1
    @category LEDStrip
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectSRGBmodsControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_SRGBmodsLEDControllerV1::RGBController_SRGBmodsLEDControllerV1(SRGBmodsLEDControllerV1* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetNameString();
    vendor                      = "SRGBmods.net";
    description                 = "SRGBmods LED Controller V1 Device";
    type                        = DEVICE_TYPE_LEDSTRIP;
    location                    = controller->GetLocationString();
    serial                      = controller->GetSerialString();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = SRGBMODS_LED_CONTROLLER_V1_MODE_DIRECT;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Rainbow;
    Rainbow.name                = "Rainbow Wave";
    Rainbow.value               = SRGBMODS_LED_CONTROLLER_V1_MODE_RAINBOW;
    Rainbow.flags               = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Rainbow.color_mode          = MODE_COLORS_RANDOM;
    Rainbow.brightness_min      = 0x00;
    Rainbow.brightness_max      = 0xFF;
    Rainbow.brightness          = 0xFF;
    Rainbow.speed_min           = 0x0A;
    Rainbow.speed_max           = 0xFF;
    Rainbow.speed               = 0x7F;
    modes.push_back(Rainbow);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = SRGBMODS_LED_CONTROLLER_V1_MODE_BREATHING_MODE_SPECIFIC;
    Breathing.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.brightness_min    = 0x00;
    Breathing.brightness_max    = 0xFF;
    Breathing.brightness        = 0xFF;
    Breathing.speed_min         = 0x0A;
    Breathing.speed_max         = 0xFF;
    Breathing.speed             = 0x7F;
    Breathing.colors_min        = 1;
    Breathing.colors_max        = 1;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode Static;
    Static.name                 = "Static";
    Static.value                = SRGBMODS_LED_CONTROLLER_V1_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Static.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Static.brightness_min       = 0x00;
    Static.brightness_max       = 0xFF;
    Static.brightness           = 0xFF;
    Static.colors_min           = 1;
    Static.colors_max           = 1;
    Static.colors.resize(1);
    modes.push_back(Static);

    SetupZones();
}

RGBController_SRGBmodsLEDControllerV1::~RGBController_SRGBmodsLEDControllerV1()
{
    Shutdown();

    delete controller;
}

void RGBController_SRGBmodsLEDControllerV1::SetupZones()
{
    /*-----------------------------------------------------*\
    | Only set LED count on the first run                   |
    \*-----------------------------------------------------*/
    bool first_run = false;

    if(zones.size() == 0)
    {
        first_run = true;
    }

    /*-----------------------------------------------------*\
    | Clear any existing color/LED configuration            |
    \*-----------------------------------------------------*/
    leds.clear();
    colors.clear();
    zones.resize(SRGBMODS_LED_CONTROLLER_V1_NUM_CHANNELS);

    /*-----------------------------------------------------*\
    | Set zones and leds                                    |
    \*-----------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        /*-------------------------------------------------*\
        | The maximum number of LEDs per channel is 800     |
        | according to https://srgbmods.net/lcv1/           |
        \*-------------------------------------------------*/
        zones[zone_idx].leds_min                    = 0;
        zones[zone_idx].leds_max                    = 800;

        if(first_run)
        {
            zones[zone_idx].flags                   = ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_NAME
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_TYPE
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_MATRIX_MAP;
        }

        if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_NAME))
        {
            zones[zone_idx].name                    = "Addressable RGB Header ";
            zones[zone_idx].name.append(std::to_string(zone_idx + 1));
        }

        if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_SIZE))
        {
            zones[zone_idx].leds_count              = 0;
        }

        if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_TYPE))
        {
            zones[zone_idx].type                    = ZONE_TYPE_LINEAR;
        }

        if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_MATRIX_MAP))
        {
            zones[zone_idx].matrix_map.width        = 0;
            zones[zone_idx].matrix_map.height       = 0;
            zones[zone_idx].matrix_map.map.resize(0);
        }

        for(unsigned int led_ch_idx = 0; led_ch_idx < zones[zone_idx].leds_count; led_ch_idx++)
        {
            led new_led;
            new_led.name                            = zones[zone_idx].name + ", LED " + std::to_string(led_ch_idx + 1);

            leds.push_back(new_led);
            leds_channel.push_back(zone_idx);
        }
    }

    SetupColors();
}

void RGBController_SRGBmodsLEDControllerV1::DeviceConfigureZone(int zone_idx)
{
    if((size_t)zone_idx < zones.size())
    {
        SetupZones();
    }
}

void RGBController_SRGBmodsLEDControllerV1::DeviceUpdateLEDs()
{
    if(modes[active_mode].value == SRGBMODS_LED_CONTROLLER_V1_MODE_DIRECT)
    {
        for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
        {
            if(zones[zone_idx].leds_count > 0)
            {
                controller->SetChannelLEDs((unsigned char)zone_idx, zones[zone_idx].colors, zones[zone_idx].leds_count);
            }
        }
    }
    else
    {
        DeviceUpdateMode();
    }
}

void RGBController_SRGBmodsLEDControllerV1::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SRGBmodsLEDControllerV1::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SRGBmodsLEDControllerV1::DeviceUpdateMode()
{
    if(modes[active_mode].value == SRGBMODS_LED_CONTROLLER_V1_MODE_DIRECT)
    {
        controller->SetDirect();

        DeviceUpdateLEDs();
    }
    else
    {
        unsigned int value  = modes[active_mode].value;
        RGBColor     color  = 0;

        if(modes[active_mode].value == SRGBMODS_LED_CONTROLLER_V1_MODE_BREATHING_MODE_SPECIFIC && modes[active_mode].color_mode == MODE_COLORS_RANDOM)
        {
            value           = SRGBMODS_LED_CONTROLLER_V1_MODE_BREATHING_RANDOM;
        }

        if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
        {
            color           = modes[active_mode].colors[0];
        }

        controller->SetConfiguration(value, modes[active_mode].speed, modes[active_mode].brightness, color);
    }
}
