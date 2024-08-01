/*---------------------------------------------------------*\
| RGBController_BlinkController.cpp                         |
|                                                           |
|   RGBController for ThingM Blink                          |
|                                                           |
|   Eric S (edbgon)                             01 Oct 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_BlinkController.h"

/**------------------------------------------------------------------*\
    @name ThingM Blink
    @category LEDStrip
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectThingMBlink
    @comment
\*-------------------------------------------------------------------*/

RGBController_BlinkController::RGBController_BlinkController(BlinkController* controller_ptr)
{
    controller          = controller_ptr;

    name                = "Blink";
    vendor              = "ThingM";
    type                = DEVICE_TYPE_LEDSTRIP;
    description         = controller->GetDeviceName();
    serial              = controller->GetSerial();
    location            = controller->GetLocation();

    mode Off;
    Off.name            = "Off";
    Off.flags           = 0;
    Off.value           = BLINK_MODE_OFF;
    Off.color_mode      = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = BLINK_MODE_DIRECT;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    Direct.colors_min       = 1;
    Direct.colors_max       = 1;
    Direct.colors.resize(1);
    modes.push_back(Direct);

    mode Fade;
    Fade.name               = "Fade";
    Fade.value              = BLINK_MODE_FADE;
    Fade.flags              = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED;
    Fade.color_mode         = MODE_COLORS_PER_LED;
    Fade.speed_min          = 0xFFFF;
    Fade.speed              = 0x0000;
    Fade.speed_max          = 0x0000;
    Fade.colors_min         = 1;
    Fade.colors_max         = 1;
    Fade.colors.resize(1);
    modes.push_back(Fade);

    SetupZones();
    active_mode = 1;
}

RGBController_BlinkController::~RGBController_BlinkController()
{
    delete controller;
}

void RGBController_BlinkController::SetupZones()
{
    zone Blink_zone;
    Blink_zone.name          = "blink(1) mk2";
    Blink_zone.type          = ZONE_TYPE_SINGLE;
    Blink_zone.leds_min      = 2;
    Blink_zone.leds_max      = 2;
    Blink_zone.leds_count    = 2;
    Blink_zone.matrix_map    = NULL;
    zones.push_back(Blink_zone);

    led Blink_led;
    Blink_led.name = "LED A";
    Blink_led.value = 1;
    leds.push_back(Blink_led);

    Blink_led.name = "LED B";
    Blink_led.value = 2;
    leds.push_back(Blink_led);

    SetupColors();

}

void RGBController_BlinkController::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_BlinkController::DeviceUpdateLEDs()
{
    for(std::size_t led = 0; led < colors.size(); led++)
    {
        UpdateSingleLED((int)led);
    }
}

void RGBController_BlinkController::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_BlinkController::UpdateSingleLED(int led)
{
    unsigned char red = RGBGetRValue(colors[led]);
    unsigned char grn = RGBGetGValue(colors[led]);
    unsigned char blu = RGBGetBValue(colors[led]);

    controller->SendUpdate(leds[led].value, red, grn, blu, modes[active_mode].speed);
}

void RGBController_BlinkController::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
