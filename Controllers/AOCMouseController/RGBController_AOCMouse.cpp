/*---------------------------------------------------------*\
| RGBController_AOCMouse.cpp                                |
|                                                           |
|   RGBController for AOC mouse                             |
|                                                           |
|   Adam Honse (CalcProgrammer1)                20 Apr 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_AOCMouse.h"

/**------------------------------------------------------------------*\
    @name AOC Mouse
    @category Mouse
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectAOCMouseControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_AOCMouse::RGBController_AOCMouse(AOCMouseController* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = controller->GetDeviceName();
    vendor                          = "AOC";
    type                            = DEVICE_TYPE_MOUSE;
    description                     = "AOC Mouse Device";
    location                        = controller->GetDeviceLocation();
    serial                          = controller->GetSerialString();

    mode Direct;
    Direct.name                     = "Direct";
    Direct.value                    = AOC_MOUSE_MODE_STATIC_SINGLE_COLOR;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode               = MODE_COLORS_PER_LED;
    Direct.brightness_min           = AOC_MOUSE_BRIGHTNESS_OFF;
    Direct.brightness_max           = AOC_MOUSE_BRIGHTNESS_HIGH;
    Direct.brightness               = AOC_MOUSE_BRIGHTNESS_HIGH;
    modes.push_back(Direct);

    mode SpectrumCycle;
    SpectrumCycle.name              = "Spectrum Cycle";
    SpectrumCycle.value             = AOC_MOUSE_MODE_STATIC_RANDOM;
    SpectrumCycle.flags             = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    SpectrumCycle.color_mode        = MODE_COLORS_RANDOM;
    SpectrumCycle.brightness_min    = AOC_MOUSE_BRIGHTNESS_OFF;
    SpectrumCycle.brightness_max    = AOC_MOUSE_BRIGHTNESS_HIGH;
    SpectrumCycle.brightness        = AOC_MOUSE_BRIGHTNESS_HIGH;
    SpectrumCycle.speed_min         = AOC_MOUSE_SPEED_SLOW;
    SpectrumCycle.speed_max         = AOC_MOUSE_SPEED_FAST;
    SpectrumCycle.speed             = AOC_MOUSE_SPEED_MEDIUM;
    modes.push_back(SpectrumCycle);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = AOC_MOUSE_MODE_BREATHING_SINGLE_COLOR;
    Breathing.flags                 = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Breathing.color_mode            = MODE_COLORS_PER_LED;
    Breathing.brightness_min        = AOC_MOUSE_BRIGHTNESS_OFF;
    Breathing.brightness_max        = AOC_MOUSE_BRIGHTNESS_HIGH;
    Breathing.brightness            = AOC_MOUSE_BRIGHTNESS_HIGH;
    Breathing.speed_min             = AOC_MOUSE_SPEED_SLOW;
    Breathing.speed_max             = AOC_MOUSE_SPEED_FAST;
    Breathing.speed                 = AOC_MOUSE_SPEED_MEDIUM;
    modes.push_back(Breathing);

    mode Blink;
    Blink.name                      = "Flashing";
    Blink.value                     = AOC_MOUSE_MODE_BLINK_SINGLE_COLOR;
    Blink.flags                     = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Blink.color_mode                = MODE_COLORS_PER_LED;
    Blink.brightness_min            = AOC_MOUSE_BRIGHTNESS_OFF;
    Blink.brightness_max            = AOC_MOUSE_BRIGHTNESS_HIGH;
    Blink.brightness                = AOC_MOUSE_BRIGHTNESS_HIGH;
    Blink.speed_min                 = AOC_MOUSE_SPEED_SLOW;
    Blink.speed_max                 = AOC_MOUSE_SPEED_FAST;
    Blink.speed                     = AOC_MOUSE_SPEED_MEDIUM;
    modes.push_back(Blink);

    mode Wave;
    Wave.name                       = "Wave";
    Wave.value                      = AOC_MOUSE_MODE_WAVE_SINGLE_COLOR;
    Wave.flags                      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Wave.color_mode                 = MODE_COLORS_PER_LED;
    Wave.brightness_min             = AOC_MOUSE_BRIGHTNESS_OFF;
    Wave.brightness_max             = AOC_MOUSE_BRIGHTNESS_HIGH;
    Wave.brightness                 = AOC_MOUSE_BRIGHTNESS_HIGH;
    Wave.speed_min                  = AOC_MOUSE_SPEED_SLOW;
    Wave.speed_max                  = AOC_MOUSE_SPEED_FAST;
    Wave.speed                      = AOC_MOUSE_SPEED_MEDIUM;
    modes.push_back(Wave);

    mode RainbowWave;
    RainbowWave.name                = "Rainbow Wave";
    RainbowWave.value               = AOC_MOUSE_MODE_WAVE_RANDOM;
    RainbowWave.flags               = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    RainbowWave.color_mode          = MODE_COLORS_RANDOM;
    RainbowWave.brightness_min      = AOC_MOUSE_BRIGHTNESS_OFF;
    RainbowWave.brightness_max      = AOC_MOUSE_BRIGHTNESS_HIGH;
    RainbowWave.brightness          = AOC_MOUSE_BRIGHTNESS_HIGH;
    RainbowWave.speed_min           = AOC_MOUSE_SPEED_SLOW;
    RainbowWave.speed_max           = AOC_MOUSE_SPEED_FAST;
    RainbowWave.speed               = AOC_MOUSE_SPEED_MEDIUM;
    modes.push_back(RainbowWave);

    mode DPI;
    DPI.name                        = "DPI";
    DPI.value                       = AOC_MOUSE_MODE_DPI;
    DPI.flags                       = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    DPI.color_mode                  = MODE_COLORS_RANDOM;
    DPI.brightness_min              = AOC_MOUSE_BRIGHTNESS_OFF;
    DPI.brightness_max              = AOC_MOUSE_BRIGHTNESS_HIGH;
    DPI.brightness                  = AOC_MOUSE_BRIGHTNESS_HIGH;
    modes.push_back(DPI);

    SetupZones();
};

RGBController_AOCMouse::~RGBController_AOCMouse()
{
    delete controller;
}

void RGBController_AOCMouse::SetupZones()
{
    zone logo_zone;
    logo_zone.name                  = "Logo";
    logo_zone.type                  = ZONE_TYPE_SINGLE;
    logo_zone.leds_min              = 1;
    logo_zone.leds_max              = 1;
    logo_zone.leds_count            = 1;
    logo_zone.matrix_map            = NULL;
    zones.push_back(logo_zone);

    led logo_led;
    logo_led.name                   = "Logo";
    leds.push_back(logo_led);

    zone scroll_wheel_zone;
    scroll_wheel_zone.name          = "Scroll Wheel";
    scroll_wheel_zone.type          = ZONE_TYPE_SINGLE;
    scroll_wheel_zone.leds_min      = 1;
    scroll_wheel_zone.leds_max      = 1;
    scroll_wheel_zone.leds_count    = 1;
    scroll_wheel_zone.matrix_map    = NULL;
    zones.push_back(scroll_wheel_zone);

    led scroll_wheel_led;
    scroll_wheel_led.name           = "Scroll Wheel";
    leds.push_back(scroll_wheel_led);

    SetupColors();
}

void RGBController_AOCMouse::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AOCMouse::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_AOCMouse::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AOCMouse::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AOCMouse::DeviceUpdateMode()
{
    if(modes[active_mode].value == AOC_MOUSE_MODE_STATIC_SINGLE_COLOR)
    {
        controller->SendDirect(&colors[0]);
    }
    else
    {
        unsigned char   aoc_direction   = AOC_MOUSE_DIRECTION_CLOCKWISE;
        unsigned int    aoc_mode        = modes[active_mode].value;

        if(modes[active_mode].direction == MODE_DIRECTION_LEFT)
        {
            aoc_direction = AOC_MOUSE_DIRECTION_COUNTERCLOCKWISE;
        }

        if(modes[active_mode].color_mode == MODE_COLORS_RANDOM)
        {
            switch(modes[active_mode].value)
            {
                case AOC_MOUSE_MODE_BREATHING_SINGLE_COLOR:
                    aoc_mode = AOC_MOUSE_MODE_BREATHING_RANDOM;
                    break;

                case AOC_MOUSE_MODE_BLINK_SINGLE_COLOR:
                    aoc_mode = AOC_MOUSE_MODE_BLINK_RANDOM;
                    break;
            }
        }

        controller->SendPacket(aoc_mode,
                               modes[active_mode].brightness,
                               modes[active_mode].speed,
                               aoc_direction,
                               &colors[0]);
    }
}
