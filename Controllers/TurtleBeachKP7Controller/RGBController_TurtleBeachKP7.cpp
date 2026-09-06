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
#include "RGBControllerKeyNames.h"

#define NA                                          0xFFFFFFFF

/*---------------------------------------------------------*\
| The 33 LEDs are exposed as three zones: the Num Lock and  |
| Shift Mode indicators along the top, the main keypad, and |
| the two wrist rest LEDs, mirroring the split used for the |
| KB7.                                                      |
\*---------------------------------------------------------*/
#define TURTLE_BEACH_KP7_INDICATOR_COUNT            3
#define TURTLE_BEACH_KP7_KEYPAD_COUNT               28
#define TURTLE_BEACH_KP7_WRIST_COUNT                2

/*---------------------------------------------------------*\
| The device numbers its LEDs 0-32 in an order that does    |
| not follow the zone layout, so this table maps each LED   |
| in zone/presentation order to its slot in the device's    |
| color buffer.  Order: the three indicator LEDs, then the  |
| keypad (the F13-F15 / profile row, then the number grid   |
| row by row with that row's mode key last, then 0 / . /    |
| Enter, then the space bar), then the two wrist rest LEDs. |
\*---------------------------------------------------------*/
static const unsigned int turtle_beach_kp7_led_order[TURTLE_BEACH_KP7_LED_COUNT] =
{
    28, 29, 30,
    21,  0,  7, 14,
     8, 22, 15,  1, 11,
    23,  2,  9,  4, 18,
    10, 24, 16, 25,  5,
    17,  3, 13, 12, 19,
    27,  6, 20,
    26,
    31, 32,
};

/*---------------------------------------------------------*\
| Physical grid for the keypad zone.  [y][x] holds the LED  |
| index within that zone (0-27, an index into the keypad    |
| slice of the LED list, not the device LED number) and NA  |
| marks a cell with no LED behind it.  Row 0 is the F13-F15 |
| and profile switch row; the mode keys form the rightmost  |
| column.  The two + keys and the two Enter keys are each   |
| a stacked pair in one column, all normal size; the space  |
| bar sits directly under the lower Enter so the device     |
| view does not auto-grow that Enter downward.  The 0 key   |
| is double width, filling the empty cell to its right.     |
\*---------------------------------------------------------*/
#define TURTLE_BEACH_KP7_MATRIX_HEIGHT              7
#define TURTLE_BEACH_KP7_MATRIX_WIDTH               5

static unsigned int turtle_beach_kp7_matrix_map[TURTLE_BEACH_KP7_MATRIX_HEIGHT][TURTLE_BEACH_KP7_MATRIX_WIDTH] =
{
    {   0,   1,   2,   3,  NA },
    {   4,   5,   6,   7,   8 },
    {   9,  10,  11,  12,  13 },
    {  14,  15,  16,  17,  18 },
    {  19,  20,  21,  22,  23 },
    {  24,  NA,  25,  26,  NA },
    {  NA,  NA,  NA,  27,  NA },
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
             dock exposes as PID 0x503E are not handled.  LEDs are
             split into Indicators, Keypad, and Wrist Rest zones; the
             keypad zone carries a matrix map.
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
    /*-----------------------------------------------------*\
    | LED names in zone/presentation order.  Local arrays   |
    | rather than file scope ones because the KEY_EN_       |
    | values live in another translation unit and are not   |
    | safe to read during static initialization.  The two   |
    | indicator LEDs and the shift mode key are status      |
    | lights rather than typing keys, so they get plain     |
    | names.                                                |
    \*-----------------------------------------------------*/
    const char* indicator_names[TURTLE_BEACH_KP7_INDICATOR_COUNT] =
    {
        "Num Lock Indicator", "Shift Mode Indicator", "Key: Shift Mode",
    };

    const char* keypad_names[TURTLE_BEACH_KP7_KEYPAD_COUNT] =
    {
        KEY_EN_F13,         KEY_EN_F14,          KEY_EN_F15,          "Key: Profile Switch",

        KEY_EN_NUMPAD_LOCK, KEY_EN_NUMPAD_DIVIDE, KEY_EN_NUMPAD_TIMES, KEY_EN_NUMPAD_MINUS, "Key: Mode 1",
        KEY_EN_NUMPAD_7,    KEY_EN_NUMPAD_8,     KEY_EN_NUMPAD_9,     KEY_EN_NUMPAD_PLUS,   "Key: Mode 2",
        KEY_EN_NUMPAD_4,    KEY_EN_NUMPAD_5,     KEY_EN_NUMPAD_6,     KEY_EN_NUMPAD_PLUS,   "Key: Mode 3",
        KEY_EN_NUMPAD_1,    KEY_EN_NUMPAD_2,     KEY_EN_NUMPAD_3,     KEY_EN_NUMPAD_ENTER,  "Key: Mode 4",
        KEY_EN_NUMPAD_0,    KEY_EN_NUMPAD_PERIOD, KEY_EN_NUMPAD_ENTER,

        KEY_EN_SPACE,
    };

    /*-----------------------------------------------------*\
    | Three zones: the indicator LEDs, the keypad, and the  |
    | wrist rest.  Only the keypad carries a matrix map;    |
    | the other two are short linear strips.                |
    \*-----------------------------------------------------*/
    zone indicator_zone;

    indicator_zone.name         = "Indicators";
    indicator_zone.type         = ZONE_TYPE_LINEAR;
    indicator_zone.leds_min     = TURTLE_BEACH_KP7_INDICATOR_COUNT;
    indicator_zone.leds_max     = TURTLE_BEACH_KP7_INDICATOR_COUNT;
    indicator_zone.leds_count   = TURTLE_BEACH_KP7_INDICATOR_COUNT;

    zones.push_back(indicator_zone);

    zone keypad_zone;

    keypad_zone.name            = "Keypad";
    keypad_zone.type            = ZONE_TYPE_MATRIX;
    keypad_zone.leds_min        = TURTLE_BEACH_KP7_KEYPAD_COUNT;
    keypad_zone.leds_max        = TURTLE_BEACH_KP7_KEYPAD_COUNT;
    keypad_zone.leds_count      = TURTLE_BEACH_KP7_KEYPAD_COUNT;

    keypad_zone.matrix_map.Set(TURTLE_BEACH_KP7_MATRIX_HEIGHT, TURTLE_BEACH_KP7_MATRIX_WIDTH, (unsigned int*)&turtle_beach_kp7_matrix_map);

    zones.push_back(keypad_zone);

    zone wrist_zone;

    wrist_zone.name             = "Wrist Rest";
    wrist_zone.type             = ZONE_TYPE_LINEAR;
    wrist_zone.leds_min         = TURTLE_BEACH_KP7_WRIST_COUNT;
    wrist_zone.leds_max         = TURTLE_BEACH_KP7_WRIST_COUNT;
    wrist_zone.leds_count       = TURTLE_BEACH_KP7_WRIST_COUNT;

    zones.push_back(wrist_zone);

    for(unsigned int led_idx = 0; led_idx < TURTLE_BEACH_KP7_INDICATOR_COUNT; led_idx++)
    {
        led new_led;

        new_led.name            = indicator_names[led_idx];
        new_led.value           = led_idx;

        leds.push_back(new_led);
    }

    for(unsigned int led_idx = 0; led_idx < TURTLE_BEACH_KP7_KEYPAD_COUNT; led_idx++)
    {
        led new_led;

        new_led.name            = keypad_names[led_idx];
        new_led.value           = TURTLE_BEACH_KP7_INDICATOR_COUNT + led_idx;

        leds.push_back(new_led);
    }

    for(unsigned int led_idx = 0; led_idx < TURTLE_BEACH_KP7_WRIST_COUNT; led_idx++)
    {
        led new_led;

        new_led.name            = "Wrist Rest " + std::to_string(led_idx + 1);
        new_led.value           = TURTLE_BEACH_KP7_INDICATOR_COUNT + TURTLE_BEACH_KP7_KEYPAD_COUNT + led_idx;

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_TurtleBeachKP7::DeviceUpdateLEDs()
{
    unsigned char led_data[TURTLE_BEACH_KP7_LED_COUNT * 3] = { 0 };

    /*-----------------------------------------------------*\
    | colors[] is in zone order; turtle_beach_kp7_led_order |
    | gives the device color-buffer slot for each one.      |
    \*-----------------------------------------------------*/
    for(unsigned int led_idx = 0; (led_idx < colors.size()) && (led_idx < TURTLE_BEACH_KP7_LED_COUNT); led_idx++)
    {
        unsigned int dev_led = turtle_beach_kp7_led_order[led_idx];

        led_data[(dev_led * 3) + 0] = RGBGetRValue(colors[led_idx]);
        led_data[(dev_led * 3) + 1] = RGBGetGValue(colors[led_idx]);
        led_data[(dev_led * 3) + 2] = RGBGetBValue(colors[led_idx]);
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
