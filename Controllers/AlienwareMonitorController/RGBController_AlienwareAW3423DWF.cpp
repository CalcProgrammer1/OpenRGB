/*---------------------------------------------------------*\
| RGBController_AlienwareAW3423DWF.cpp                      |
|                                                           |
|   RGBController for the Alienware AW3423DWF monitor       |
|                                                           |
|   Ferréol DUBOIS COLI (Fefe_du_973)           23 Jan 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_AlienwareAW3423DWF.h"
#include <thread>

using namespace std::chrono_literals;

/**------------------------------------------------------------------*\
    @name AW3423DWF
    @category Accessory
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectAlienwareAW3423DWFControllers
    @comment The prefered way of interacting with this device is
    to use the direct mode, in this mode you cannot choose the
    individual LED colors (all the LED will be the same color) BUT
    the frame rate of the color will be more than 3 times higher
    (better for dynamic effects). However, If you still want to
    individually control your LEDs set the mode to Static, here
    you will be able to individually set a different color for each
    LED but don't expect a frame rate higher than 1fps.
\*-------------------------------------------------------------------*/
RGBController_AlienwareAW3423DWF::RGBController_AlienwareAW3423DWF(AlienwareAW3423DWFController* controller_ptr) :
    controller(controller_ptr)
{
    name        = "Alienware AW3423DWF";
    vendor      = "Alienware";
    type        = DEVICE_TYPE_ACCESSORY;
    description = "Alienware QD-OLED Monitor";
    location    = controller->GetLocation();
    serial      = controller->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name       = "Static (indivdual LED control)";
    Static.value      = 1;
    Static.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    SetupZones();
}

void RGBController_AlienwareAW3423DWF::SetupZones()
{
    std::vector<std::string> led_names = {"Alien", "Number", "Switch", "All"};

    zone new_zone;
    new_zone.name          = "Monitor Lighting";
    new_zone.type          = ZONE_TYPE_LINEAR;
    new_zone.leds_min      = led_names.size();
    new_zone.leds_max      = led_names.size();
    new_zone.leds_count    = led_names.size();
    new_zone.matrix_map    = NULL;
    zones.push_back(new_zone);

    for(const std::string& name : led_names)
    {
        led new_led;
        new_led.name = name;
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_AlienwareAW3423DWF::DeviceUpdateLEDs()
{
    if(modes[active_mode].value == 0)
    {
        UpdateSingleLED(3);
    }
    else
    {
        for(unsigned int led_idx = 0; led_idx < leds.size() - 1; led_idx++)
        {
            UpdateSingleLED(led_idx);
            std::this_thread::sleep_for(200ms);
        }
    }
}

void RGBController_AlienwareAW3423DWF::UpdateSingleLED(int led)
{
    unsigned char led_id = 0x00;

    switch(led)
    {
    case 0: led_id = 0x01; break;
    case 1: led_id = 0x02; break;
    case 2: led_id = 0x08; break;
    case 3: led_id = 0x0b; break;
    }

    RGBColor color = colors[led];
    controller->SendColor(led_id,
                          RGBGetRValue(color),
                          RGBGetGValue(color),
                          RGBGetBValue(color));
}

RGBController_AlienwareAW3423DWF::~RGBController_AlienwareAW3423DWF()
{
    delete controller;
}

void RGBController_AlienwareAW3423DWF::UpdateZoneLEDs(int zone)
{
}

void RGBController_AlienwareAW3423DWF::DeviceUpdateMode()
{
}

void RGBController_AlienwareAW3423DWF::ResizeZone(int, int) {}
