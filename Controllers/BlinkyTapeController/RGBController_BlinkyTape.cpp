/*---------------------------------------------------------*\
| RGBController_BlinkyTape.cpp                              |
|                                                           |
|   RGBController for BlinkyTape                            |
|                                                           |
|   Matt Mets (matt@blinkinlabs.com)            01 Jul 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_BlinkyTape.h"

/**------------------------------------------------------------------*\
    @name Blinky Tape
    @category LEDStrip
    @type Serial
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectBlinkyTapeControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_BlinkyTape::RGBController_BlinkyTape(BlinkyTapeController* controller_ptr)
{
    controller          = controller_ptr;

    name                = "BlinkyTape";
    vendor              = "Blinkinlabs";
    type                = DEVICE_TYPE_LEDSTRIP;
    description         = "BlinkyTape Controller Device";
    location            = controller->GetLocation();

    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = 0;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_BlinkyTape::~RGBController_BlinkyTape()
{
    Shutdown();

    delete controller;
}

void RGBController_BlinkyTape::SetupZones()
{
    /*-------------------------------------------------*\
    | Only set LED count on the first run               |
    \*-------------------------------------------------*/
    bool first_run = false;

    if(zones.size() == 0)
    {
        first_run = true;
    }

    /*-------------------------------------------------*\
    | Clear any existing color/LED configuration        |
    \*-------------------------------------------------*/
    leds.clear();
    colors.clear();
    zones.resize(1);

    /*-----------------------------------------------------*\
    | Set up zones                                          |
    \*-----------------------------------------------------*/
    zones[0].leds_min                   = 0;
    zones[0].leds_max                   = 512;

    if(first_run)
    {
        zones[0].flags                  = ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE
                                        | ZONE_FLAG_MANUALLY_CONFIGURABLE_NAME
                                        | ZONE_FLAG_MANUALLY_CONFIGURABLE_TYPE
                                        | ZONE_FLAG_MANUALLY_CONFIGURABLE_MATRIX_MAP;
    }

    if(!(zones[0].flags & ZONE_FLAG_MANUALLY_CONFIGURED_NAME))
    {
        zones[0].name                   = "Addressable RGB Header";
    }

    if(!(zones[0].flags & ZONE_FLAG_MANUALLY_CONFIGURED_SIZE))
    {
        zones[0].leds_count             = 0;
    }

    if(!(zones[0].flags & ZONE_FLAG_MANUALLY_CONFIGURED_TYPE))
    {
        zones[0].type                   = ZONE_TYPE_LINEAR;
    }

    if(!(zones[0].flags & ZONE_FLAG_MANUALLY_CONFIGURED_MATRIX_MAP))
    {
        zones[0].matrix_map.width       = 0;
        zones[0].matrix_map.height      = 0;
        zones[0].matrix_map.map.resize(0);
    }

    /*-----------------------------------------------------*\
    | Set up LEDs                                           |
    \*-----------------------------------------------------*/
    for(size_t led_idx = leds.size(); led_idx < zones[0].leds_count; led_idx++)
    {
        led new_led;
        new_led.name = zones[0].name + ", LED ";
        new_led.name.append(std::to_string(led_idx));

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_BlinkyTape::DeviceConfigureZone(int zone_idx)
{
    if((size_t)zone_idx < zones.size())
    {
        SetupZones();
    }
}

void RGBController_BlinkyTape::DeviceUpdateLEDs()
{
    controller->SetLEDs(colors);
}

void RGBController_BlinkyTape::DeviceUpdateZoneLEDs(int /*zone*/)
{
    controller->SetLEDs(colors);
}

void RGBController_BlinkyTape::DeviceUpdateSingleLED(int /*led*/)
{
    controller->SetLEDs(colors);
}

void RGBController_BlinkyTape::DeviceUpdateMode()
{

}
