/*---------------------------------------------------------*\
| RGBController_BlinkyTape.cpp                              |
|                                                           |
|   RGBController for BlinkyTape                            |
|                                                           |
|   Matt Mets (matt@blinkinlabs.com)            01 Jul 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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
    delete controller;
}

void RGBController_BlinkyTape::SetupZones()
{
    zones.clear();
    leds.clear();

    zone led_zone;
    led_zone.name       = "LED Strip";
    led_zone.type       = ZONE_TYPE_LINEAR;
    led_zone.leds_min   = 0;
    led_zone.leds_max   = 512;
    led_zone.leds_count = 0;
    led_zone.matrix_map = NULL;
    zones.push_back(led_zone);

    ResizeZone(0, led_zone.leds_count);
}

void RGBController_BlinkyTape::ResizeZone(int zone, int new_size)
{
    /*-------------------------------------------------*\
    | Explicitly cast these to avoid compiler warnings  |
    \*-------------------------------------------------*/
    const unsigned int zone_u       = static_cast<unsigned int>(zone);
    const unsigned int new_size_u   = static_cast<unsigned int>(new_size);

    /*-------------------------------------------------*\
    | Check that the zone is in bounds                  |
    \*-------------------------------------------------*/
    if((zone_u > zones.size()) || (zone < 0))
    {
        return;
    }

    /*-------------------------------------------------*\
    | And that the new size is in bounds                |
    \*-------------------------------------------------*/
    if((new_size_u > zones.at(zone).leds_max) || (new_size_u < zones.at(zone).leds_min))
    {
        return;
    }

    /*-------------------------------------------------*\
    | And that there's actually a change                |
    \*-------------------------------------------------*/
    if(zones.at(zone).leds_count == new_size_u)
    {
        return;
    }

    /*-------------------------------------------------*\
    | If the new size is less than the current size,    |
    | just chop off the end                             |
    \*-------------------------------------------------*/
    if(leds.size() > new_size_u)
    {
        leds.resize(new_size);
    }

    /*-------------------------------------------------*\
    | Otherwise, add new LEDs to the end                |
    \*-------------------------------------------------*/
    if(leds.size() < new_size_u)
    {
        for(size_t led_idx = leds.size(); led_idx < new_size_u; led_idx++)
        {
            led new_led;
            new_led.name = "LED ";
            new_led.name.append(std::to_string(led_idx));

            leds.push_back(new_led);
        }
    }

    zones.at(zone).leds_count = new_size;

    SetupColors();
}

void RGBController_BlinkyTape::DeviceUpdateLEDs()
{
    controller->SetLEDs(colors);
}

void RGBController_BlinkyTape::UpdateZoneLEDs(int /*zone*/)
{
    controller->SetLEDs(colors);
}

void RGBController_BlinkyTape::UpdateSingleLED(int /*led*/)
{
    controller->SetLEDs(colors);
}

void RGBController_BlinkyTape::DeviceUpdateMode()
{

}
