/*---------------------------------------------------------*\
| GigabyteRGBController_RGBFusion2DRAM.cpp                  |
|                                                           |
|   RGBController for Gigabyte Aorus RGB Fusion 2 RAM       |
|                                                           |
|   Adam Honse (CalcProgrammer1)                07 Jun 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_GigabyteRGBFusion2DRAM.h"

/**------------------------------------------------------------------*\
    @name Gigabyte Fusion2 DRAM
    @category RAM
    @type I2C
    @save :x:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectGigabyteRGBFusion2DRAMControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_RGBFusion2DRAM::RGBController_RGBFusion2DRAM(RGBFusion2DRAMController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "RGB Fusion 2 DRAM";
    vendor      = "Gigabyte";
    description = "RGB Fusion 2 DRAM Device";
    location    = controller->GetDeviceLocation();

    type = DEVICE_TYPE_DRAM;

    /*-----------------------------------------------------*\
    | Direct mode is achieved through bit of a hack.  Use   |
    | pulse mode but set the configuration such that it does|
    | not actually pulse.  This allows each LED to be set   |
    | independently.                                        |
    | See this Discord conversation:                        |
    | https://discord.com/channels/699861463375937578/      |
    | 699861463887773729/719700736845414453                 |
    \*-----------------------------------------------------*/
    mode Direct;
    Direct.name           = "Direct";
    Direct.value          = RGB_FUSION_2_DRAM_MODE_DIRECT;
    Direct.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode     = MODE_COLORS_PER_LED;
    Direct.brightness_min = 0;
    Direct.brightness_max = 100;
    Direct.brightness     = 100;
    modes.push_back(Direct);

    mode Off;
    Off.name       = "Off";
    Off.value      = RGB_FUSION_2_DRAM_MODE_OFF;
    Off.flags      = 0;
    Off.color_mode = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name           = "Static";
    Static.value          = RGB_FUSION_2_DRAM_MODE_STATIC;
    Static.flags          = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.colors_min     = 1;
    Static.colors_max     = 1;
    Static.colors.resize(1);
    Static.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Static.brightness_min = 0;
    Static.brightness_max = 100;
    Static.brightness     = 100;
    modes.push_back(Static);

    SetupZones();
}

RGBController_RGBFusion2DRAM::~RGBController_RGBFusion2DRAM()
{
    Shutdown();

    delete controller;
}

void RGBController_RGBFusion2DRAM::SetupZones()
{
    /*---------------------------------------------------------*\
    | Search through all LEDs and create zones for each channel |
    | type                                                      |
    \*---------------------------------------------------------*/
    zone* new_zone          = new zone();

    // Set zone name to channel name
    new_zone->name          = "DRAM";
    new_zone->leds_min      = controller->GetLEDCount();
    new_zone->leds_max      = controller->GetLEDCount();
    new_zone->leds_count    = controller->GetLEDCount();

    // Push new zone to zones vector
    zones.push_back(*new_zone);

    for(unsigned int led_idx = 0; led_idx < controller->GetLEDCount(); led_idx++)
    {
        led* new_led        = new led();
        new_led->name       = "DRAM LED";

        // Push new LED to LEDs vector
        leds.push_back(*new_led);
    }

    SetupColors();
}

void RGBController_RGBFusion2DRAM::DeviceUpdateLEDs()
{
    /*---------------------------------------------------------*\
    | Loop through all LEDs and set effect parameters. Must     |
    | apply after every effect set                              |
    \*---------------------------------------------------------*/
    for(unsigned int led_idx = 0; led_idx < colors.size(); led_idx++)
    {
        RGBColor      color         = 0;

        if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
        {
            color                   = colors[led_idx];
        }
        else if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
        {
            color                   = modes[active_mode].colors[0];
        }

        unsigned char red           = RGBGetRValue(color);
        unsigned char grn           = RGBGetGValue(color);
        unsigned char blu           = RGBGetBValue(color);

        int           mode          = modes[active_mode].value;
        unsigned int  speed         = modes[active_mode].speed;
        unsigned int  brightness    = modes[active_mode].brightness;

        controller->SetLEDEffect(led_idx, mode, brightness, speed, red, grn, blu);

        /*---------------------------------------------------------*\
        | Only update once unless in direct mode                    |
        \*---------------------------------------------------------*/
        if(modes[active_mode].value != RGB_FUSION_2_DRAM_MODE_DIRECT)
        {
            break;
        }
    }
}

void RGBController_RGBFusion2DRAM::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RGBFusion2DRAM::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RGBFusion2DRAM::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}

