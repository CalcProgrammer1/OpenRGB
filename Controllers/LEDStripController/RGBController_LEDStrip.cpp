/*---------------------------------------------------------*\
| RGBController_LEDStrip.cpp                                |
|                                                           |
|   RGBController for serial LED strips                     |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      20 Jun 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_LEDStrip.h"

/**------------------------------------------------------------------*\
    @name Serial LED Strip
    @category LEDStrip
    @type Serial
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectLEDStripControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_LEDStrip::RGBController_LEDStrip(LEDStripController* controller_ptr)
{
    controller          = controller_ptr;

    name                = controller->GetName();
    type                = DEVICE_TYPE_LEDSTRIP;
    description         = "Serial LED Strip Device";
    location            = controller->GetLocation();

    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = 0;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_LEDStrip::~RGBController_LEDStrip()
{
    Shutdown();

    delete controller;
}

void RGBController_LEDStrip::SetupZones()
{
    /*-----------------------------------------------------*\
    | Only set LED count on the first run                   |
    \------------------------------------------------------*/
    bool first_run = false;

    if(zones.size() == 0)
    {
        first_run = true;
    }

    /*-----------------------------------------------------*\
    | Clear any existing color/LED configuration            |
    \------------------------------------------------------*/
    leds.clear();
    colors.clear();
    zones.resize(1);

    /*-----------------------------------------------------*\
    | Set zones and leds                                    |
    \------------------------------------------------------*/
    zones[0].leds_min               = controller->num_leds;
    zones[0].leds_max               = controller->num_leds;

    if(first_run)
    {
        zones[0].flags              = ZONE_FLAG_MANUALLY_CONFIGURABLE_NAME
                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_TYPE
                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_MATRIX_MAP
                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_SEGMENTS;
    }

    if(!(zones[0].flags & ZONE_FLAG_MANUALLY_CONFIGURED_NAME))
    {
        zones[0].name               = "LED Strip";
    }

    zones[0].leds_count             = controller->num_leds;

    if(!(zones[0].flags & ZONE_FLAG_MANUALLY_CONFIGURED_TYPE))
    {
        zones[0].type               = ZONE_TYPE_LINEAR;
    }

    if(!(zones[0].flags & ZONE_FLAG_MANUALLY_CONFIGURED_MATRIX_MAP))
    {
        zones[0].matrix_map.width   = 0;
        zones[0].matrix_map.height  = 0;
        zones[0].matrix_map.map.resize(0);
    }

    for(unsigned int led_idx = 0; led_idx < zones[0].leds_count; led_idx++)
    {
        led new_led;
        new_led.name                = zones[0].name + ", LED ";
        new_led.name.append(std::to_string(led_idx));

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_LEDStrip::DeviceConfigureZone(int zone_idx)
{
    if((size_t)zone_idx < zones.size())
    {
        SetupZones();
    }
}

void RGBController_LEDStrip::DeviceUpdateLEDs()
{
    controller->SetLEDs(colors);
}

void RGBController_LEDStrip::DeviceUpdateZoneLEDs(int /*zone*/)
{
    controller->SetLEDs(colors);
}

void RGBController_LEDStrip::DeviceUpdateSingleLED(int /*led*/)
{
    controller->SetLEDs(colors);
}

void RGBController_LEDStrip::DeviceUpdateMode()
{

}
