/*---------------------------------------------------------*\
| RGBController_AOCMousemat.cpp                             |
|                                                           |
|   RGBController for AOC mousemat                          |
|                                                           |
|   Adam Honse (CalcProgrammer1)                15 Apr 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_AOCMousemat.h"

/**------------------------------------------------------------------*\
    @name AOC Mousemat
    @category Mousemat
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectAOCMousematControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_AOCMousemat::RGBController_AOCMousemat(AOCMousematController* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = controller->GetDeviceName();
    vendor                          = "AOC";
    type                            = DEVICE_TYPE_MOUSEMAT;
    description                     = "AOC Mousemat Device";
    location                        = controller->GetDeviceLocation();
    serial                          = controller->GetSerialString();

    mode Direct;
    Direct.name                     = "Direct";
    Direct.value                    = AOC_MOUSEMAT_MODE_STATIC_SINGLE_COLOR;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode               = MODE_COLORS_PER_LED;
    Direct.brightness_min           = AOC_MOUSEMAT_BRIGHTNESS_OFF;
    Direct.brightness_max           = AOC_MOUSEMAT_BRIGHTNESS_HIGH;
    Direct.brightness               = AOC_MOUSEMAT_BRIGHTNESS_HIGH;
    modes.push_back(Direct);

    mode SpectrumCycle;
    SpectrumCycle.name              = "Spectrum Cycle";
    SpectrumCycle.value             = AOC_MOUSEMAT_MODE_STATIC_RANDOM;
    SpectrumCycle.flags             = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    SpectrumCycle.color_mode        = MODE_COLORS_RANDOM;
    SpectrumCycle.brightness_min    = AOC_MOUSEMAT_BRIGHTNESS_OFF;
    SpectrumCycle.brightness_max    = AOC_MOUSEMAT_BRIGHTNESS_HIGH;
    SpectrumCycle.brightness        = AOC_MOUSEMAT_BRIGHTNESS_HIGH;
    SpectrumCycle.speed_min         = AOC_MOUSEMAT_SPEED_SLOW;
    SpectrumCycle.speed_max         = AOC_MOUSEMAT_SPEED_FAST;
    SpectrumCycle.speed             = AOC_MOUSEMAT_SPEED_MEDIUM;
    modes.push_back(SpectrumCycle);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = AOC_MOUSEMAT_MODE_BREATHING_SINGLE_COLOR;
    Breathing.flags                 = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Breathing.color_mode            = MODE_COLORS_PER_LED;
    Breathing.brightness_min        = AOC_MOUSEMAT_BRIGHTNESS_OFF;
    Breathing.brightness_max        = AOC_MOUSEMAT_BRIGHTNESS_HIGH;
    Breathing.brightness            = AOC_MOUSEMAT_BRIGHTNESS_HIGH;
    Breathing.speed_min             = AOC_MOUSEMAT_SPEED_SLOW;
    Breathing.speed_max             = AOC_MOUSEMAT_SPEED_FAST;
    Breathing.speed                 = AOC_MOUSEMAT_SPEED_MEDIUM;
    modes.push_back(Breathing);

    mode Blink;
    Blink.name                      = "Flashing";
    Blink.value                     = AOC_MOUSEMAT_MODE_BLINK_SINGLE_COLOR;
    Blink.flags                     = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Blink.color_mode                = MODE_COLORS_PER_LED;
    Blink.brightness_min            = AOC_MOUSEMAT_BRIGHTNESS_OFF;
    Blink.brightness_max            = AOC_MOUSEMAT_BRIGHTNESS_HIGH;
    Blink.brightness                = AOC_MOUSEMAT_BRIGHTNESS_HIGH;
    Blink.speed_min                 = AOC_MOUSEMAT_SPEED_SLOW;
    Blink.speed_max                 = AOC_MOUSEMAT_SPEED_FAST;
    Blink.speed                     = AOC_MOUSEMAT_SPEED_MEDIUM;
    modes.push_back(Blink);

    mode Wave;
    Wave.name                       = "Wave";
    Wave.value                      = AOC_MOUSEMAT_MODE_WAVE_SINGLE_COLOR;
    Wave.flags                      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Wave.color_mode                 = MODE_COLORS_PER_LED;
    Wave.brightness_min             = AOC_MOUSEMAT_BRIGHTNESS_OFF;
    Wave.brightness_max             = AOC_MOUSEMAT_BRIGHTNESS_HIGH;
    Wave.brightness                 = AOC_MOUSEMAT_BRIGHTNESS_HIGH;
    Wave.speed_min                  = AOC_MOUSEMAT_SPEED_SLOW;
    Wave.speed_max                  = AOC_MOUSEMAT_SPEED_FAST;
    Wave.speed                      = AOC_MOUSEMAT_SPEED_MEDIUM;
    modes.push_back(Wave);

    mode RainbowWave;
    RainbowWave.name                = "Rainbow Wave";
    RainbowWave.value               = AOC_MOUSEMAT_MODE_WAVE_RANDOM;
    RainbowWave.flags               = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    RainbowWave.color_mode          = MODE_COLORS_RANDOM;
    RainbowWave.brightness_min      = AOC_MOUSEMAT_BRIGHTNESS_OFF;
    RainbowWave.brightness_max      = AOC_MOUSEMAT_BRIGHTNESS_HIGH;
    RainbowWave.brightness          = AOC_MOUSEMAT_BRIGHTNESS_HIGH;
    RainbowWave.speed_min           = AOC_MOUSEMAT_SPEED_SLOW;
    RainbowWave.speed_max           = AOC_MOUSEMAT_SPEED_FAST;
    RainbowWave.speed               = AOC_MOUSEMAT_SPEED_MEDIUM;
    modes.push_back(RainbowWave);

    SetupZones();
};

RGBController_AOCMousemat::~RGBController_AOCMousemat()
{
    delete controller;
}

void RGBController_AOCMousemat::SetupZones()
{
    zone mousemat_zone;
    mousemat_zone.name          = "Mousemat";
    mousemat_zone.type          = ZONE_TYPE_SINGLE;
    mousemat_zone.leds_min      = 1;
    mousemat_zone.leds_max      = 1;
    mousemat_zone.leds_count    = 1;
    mousemat_zone.matrix_map    = NULL;
    zones.push_back(mousemat_zone);

    led mousemat_led;
    mousemat_led.name           = "Mousemat";
    leds.push_back(mousemat_led);

    SetupColors();
}

void RGBController_AOCMousemat::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AOCMousemat::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_AOCMousemat::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AOCMousemat::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AOCMousemat::DeviceUpdateMode()
{
    if(modes[active_mode].value == AOC_MOUSEMAT_MODE_STATIC_SINGLE_COLOR)
    {
        controller->SendDirect(&colors[0]);
    }
    else
    {
        unsigned char   aoc_direction   = AOC_MOUSEMAT_DIRECTION_CLOCKWISE;
        unsigned int    aoc_mode        = modes[active_mode].value;

        if(modes[active_mode].direction == MODE_DIRECTION_LEFT)
        {
            aoc_direction = AOC_MOUSEMAT_DIRECTION_COUNTERCLOCKWISE;
        }

        if(modes[active_mode].color_mode == MODE_COLORS_RANDOM)
        {
            switch(modes[active_mode].value)
            {
                case AOC_MOUSEMAT_MODE_BREATHING_SINGLE_COLOR:
                    aoc_mode = AOC_MOUSEMAT_MODE_BREATHING_RANDOM;
                    break;

                case AOC_MOUSEMAT_MODE_BLINK_SINGLE_COLOR:
                    aoc_mode = AOC_MOUSEMAT_MODE_BLINK_RANDOM;
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
