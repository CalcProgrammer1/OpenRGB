/*---------------------------------------------------------*\
| RGBController_HPOmenLaptopWMI_Windows.cpp                 |
|                                                           |
|   RGBController for HP Omen laptop                        |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_HPOmenLaptopWMI_Windows.h"

/**------------------------------------------------------------------*\
    @name Omen 4-Zone Laptop Keyboard
    @category Keyboard
    @type WMI
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectHPOmenLaptopWMIControllers
    @comment Currently only supported on Windows (requires admin privileges) due to the WMI interface.
\*-------------------------------------------------------------------*/

RGBController_HPOmenLaptopWMI_Windows::RGBController_HPOmenLaptopWMI_Windows(HPOmenLaptopController_Windows *controller)
{
    /*-----------------------------------------------------*\
    | Configure the keyboard modes                          |
    \*-----------------------------------------------------*/
    this->controller = controller;

    this->name        = "Omen 4-Zone Laptop Keyboard";
    this->vendor      = "HP";
    this->description = "WMI Device";
    this->location    = "ROOT\\\\WMI:hpqBIntM";
    this->type        = DEVICE_TYPE_KEYBOARD;

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = KeyboardMode::DIRECT;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    this->modes.push_back(Direct);

    mode Off;
    Off.name       = "Off";
    Off.value      = KeyboardMode::OFF;
    Off.flags      = 0;
    Off.color_mode = MODE_COLORS_NONE;
    this->modes.push_back(Off);

    SetupZones();
}

RGBController_HPOmenLaptopWMI_Windows::~RGBController_HPOmenLaptopWMI_Windows()
{
    delete this->controller;
}

void RGBController_HPOmenLaptopWMI_Windows::SetupZones()
{
    /*-----------------------------------------------------*\
    | Set up the zone                                       |
    \*-----------------------------------------------------*/
    zone keyboard_zone;
    keyboard_zone.leds_count = 4;
    keyboard_zone.leds_min   = 0;
    keyboard_zone.leds_max   = 4;
    keyboard_zone.name       = "Keyboard";
    keyboard_zone.matrix_map = NULL;
    keyboard_zone.type       = ZONE_TYPE_LINEAR;
    this->zones.push_back(keyboard_zone);

    /*-----------------------------------------------------*\
    | Set up the LEDs                                       |
    \*-----------------------------------------------------*/
    led wasd_led;
    wasd_led.name  = "Keyboard WASD";
    this->leds.push_back(wasd_led);

    led left_led;
    left_led.name  = "Keyboard Left";
    this->leds.push_back(left_led);

    led mid_led;
    mid_led.name  = "Keyboard Middle";
    this->leds.push_back(mid_led);

    led right_led;
    right_led.name  = "Keyboard Right";
    this->leds.push_back(right_led);

    SetupColors();
}

void RGBController_HPOmenLaptopWMI_Windows::ResizeZone(int zone, int new_size)
{
    /*-----------------------------------------------------*\
    | Not Supported                                         |
    \*-----------------------------------------------------*/
}

void RGBController_HPOmenLaptopWMI_Windows::DeviceUpdateLEDs()
{
    /*-----------------------------------------------------*\
    | Set new colors                                        |
    \*-----------------------------------------------------*/
    controller->setColors(this->colors);
}

void RGBController_HPOmenLaptopWMI_Windows::UpdateZoneLEDs(int zone)
{
    /*-----------------------------------------------------*\
    | Set new colors                                        |
    \*-----------------------------------------------------*/
    controller->setColors(this->colors);
}

void RGBController_HPOmenLaptopWMI_Windows::UpdateSingleLED(int led)
{
    /*-----------------------------------------------------*\
    | Set new colors                                        |
    \*-----------------------------------------------------*/
    controller->setColors(this->colors);
}

void RGBController_HPOmenLaptopWMI_Windows::DeviceUpdateMode()
{
    /*-----------------------------------------------------*\
    | Change keyboard rgb mode                              |
    \*-----------------------------------------------------*/
    controller->changeMode((KeyboardMode)this->modes[active_mode].value);
}
