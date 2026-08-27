/*---------------------------------------------------------*\
| RGBController_TurtleBeachKP7.cpp                          |
|                                                           |
|   RGBController for Turtle Beach Command Series KP7       |
|                                                           |
|   Joe Peterson (joevpeterson@gmail.com)       05 Aug 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <string>
#include "RGBController_TurtleBeachKP7.h"

/*---------------------------------------------------------*\
| Physical LED grid, mapped against the real device.  Six   |
| columns by ten rows, [y][x] holds the LED index and NA    |
| marks a grid cell with no LED behind it.                  |
\*---------------------------------------------------------*/
#define NA                                          0xFFFFFFFF

#define TURTLE_BEACH_KP7_MATRIX_HEIGHT              10
#define TURTLE_BEACH_KP7_MATRIX_WIDTH               6

static unsigned int turtle_beach_kp7_matrix_map[TURTLE_BEACH_KP7_MATRIX_HEIGHT][TURTLE_BEACH_KP7_MATRIX_WIDTH] =
{
    {  28,  NA,  29,  NA,  NA,  30 },
    {  NA,  NA,  NA,  NA,  NA,  NA },
    {  21,   0,   7,  14,  NA,  NA },
    {  NA,  NA,  NA,  NA,  NA,  NA },
    {   8,  22,  15,   1,  NA,  11 },
    {  23,   2,   9,   4,  NA,  18 },
    {  10,  24,  16,  25,  NA,   5 },
    {  17,   3,  13,  12,  NA,  19 },
    {  27,  NA,   6,  20,  NA,  NA },
    {  31,  32,  NA,  26,  NA,  NA },
};

/**------------------------------------------------------------------*\
    @name Turtle Beach Command Series KP7
    @category Keypad
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectTurtleBeachKP7Controllers
    @comment Only the raw per-LED color stream is implemented.  The
             onboard effects, the macro and profile feature report
             channel on report ID 0x11, and the internal hub the KB7
             dock exposes as PID 0x503E are not handled.
\*-------------------------------------------------------------------*/

RGBController_TurtleBeachKP7::RGBController_TurtleBeachKP7(TurtleBeachKP7Controller* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = "Turtle Beach Command Series KP7";
    vendor                      = "Turtle Beach";
    type                        = DEVICE_TYPE_KEYPAD;
    description                 = "Turtle Beach Command Series KP7 Keypad";
    location                    = controller->GetLocation();
    serial                      = controller->GetSerialString();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = TURTLE_BEACH_KP7_MODE_DIRECT;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_TurtleBeachKP7::~RGBController_TurtleBeachKP7()
{
    Shutdown();

    delete controller;
}

void RGBController_TurtleBeachKP7::SetupZones()
{
    zone kp7_zone;

    kp7_zone.name               = "Keypad";
    kp7_zone.type               = ZONE_TYPE_MATRIX;
    kp7_zone.leds_min           = TURTLE_BEACH_KP7_LED_COUNT;
    kp7_zone.leds_max           = TURTLE_BEACH_KP7_LED_COUNT;
    kp7_zone.leds_count         = TURTLE_BEACH_KP7_LED_COUNT;

    kp7_zone.matrix_map.Set(TURTLE_BEACH_KP7_MATRIX_HEIGHT, TURTLE_BEACH_KP7_MATRIX_WIDTH, (unsigned int*)&turtle_beach_kp7_matrix_map);

    zones.push_back(kp7_zone);

    for(unsigned int led_idx = 0; led_idx < TURTLE_BEACH_KP7_LED_COUNT; led_idx++)
    {
        led new_led;

        new_led.name            = "LED " + std::to_string(led_idx + 1);
        new_led.value           = led_idx;

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_TurtleBeachKP7::DeviceUpdateLEDs()
{
    unsigned char led_data[TURTLE_BEACH_KP7_LED_COUNT * 3] = { 0 };

    for(unsigned int led_idx = 0; (led_idx < colors.size()) && (led_idx < TURTLE_BEACH_KP7_LED_COUNT); led_idx++)
    {
        led_data[(led_idx * 3) + 0] = RGBGetRValue(colors[led_idx]);
        led_data[(led_idx * 3) + 1] = RGBGetGValue(colors[led_idx]);
        led_data[(led_idx * 3) + 2] = RGBGetBValue(colors[led_idx]);
    }

    controller->SendDirect(led_data, sizeof(led_data));
}

void RGBController_TurtleBeachKP7::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_TurtleBeachKP7::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_TurtleBeachKP7::DeviceUpdateMode()
{
    /*-----------------------------------------------------*\
    | Only Direct mode is implemented                       |
    \*-----------------------------------------------------*/
}
