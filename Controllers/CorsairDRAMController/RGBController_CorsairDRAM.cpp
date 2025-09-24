/*---------------------------------------------------------*\
| RGBController_CorsairDRAM.cpp                             |
|                                                           |
|   RGBController for Corsair DRAM RGB controllers          |
|                                                           |
|   Adam Honse (CalcProgrammer1)                30 Jun 2019 |
|   Erik Gilling (konkers)                      25 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_CorsairDRAM.h"

/**------------------------------------------------------------------*\
    @name Corsair DRAM
    @category RAM
    @type SMBus
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectCorsairDRAMControllers
    @comment
        The Corsair DRAM RGB controller chip can be found on several
        Corsair memory sticks which have different LED counts. This can be controlled
        by editing the Part Number in OpenRGB.json with values in the below table.

        | Part Number | LED Count |
        | :---------: | --------: |
        | CMG         |  6        |
        | CMH         | 10        |
        | CMN         | 10        |
        | CMT         | 12        |
\*-------------------------------------------------------------------*/

RGBController_CorsairDRAM::RGBController_CorsairDRAM(CorsairDRAMController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetDeviceName();
    vendor                      = "Corsair";
    type                        = DEVICE_TYPE_DRAM;
    description                 = "Corsair DRAM RGB Device";
    location                    = controller->GetDeviceLocation();
    version                     = controller->GetDeviceVersion();

    if(controller->GetProtocolVersion() >= 4)
    {
        mode Direct;
        Direct.name             = "Direct";
        Direct.value            = CORSAIR_DRAM_MODE_DIRECT;
        Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
        Direct.color_mode       = MODE_COLORS_PER_LED;
        modes.push_back(Direct);
    }

    mode Custom;
    Custom.name                 = "Custom";
    Custom.value                = CORSAIR_DRAM_MODE_STATIC;
    Custom.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Custom.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Custom);

    mode ColorShift;
    ColorShift.name             = "Color Shift";
    ColorShift.value            = CORSAIR_DRAM_MODE_COLOR_SHIFT;
    ColorShift.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    ColorShift.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    ColorShift.speed_min        = CORSAIR_DRAM_SPEED_SLOW;
    ColorShift.speed_max        = CORSAIR_DRAM_SPEED_FAST;
    ColorShift.speed            = CORSAIR_DRAM_SPEED_SLOW;
    ColorShift.brightness_min   = CORSAIR_DRAM_BRIGHTNESS_MIN;
    ColorShift.brightness_max   = CORSAIR_DRAM_BRIGHTNESS_MAX;
    ColorShift.brightness       = CORSAIR_DRAM_BRIGHTNESS_DEFAULT;
    ColorShift.colors_min       = 2;
    ColorShift.colors_max       = 2;
    ColorShift.colors.resize(2);
    modes.push_back(ColorShift);

    mode ColorPulse;
    ColorPulse.name             = "Color Pulse";
    ColorPulse.value            = CORSAIR_DRAM_MODE_COLOR_PULSE;
    ColorPulse.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    ColorPulse.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    ColorPulse.speed_min        = CORSAIR_DRAM_SPEED_SLOW;
    ColorPulse.speed_max        = CORSAIR_DRAM_SPEED_FAST;
    ColorPulse.speed            = CORSAIR_DRAM_SPEED_SLOW;
    ColorPulse.brightness_min   = CORSAIR_DRAM_BRIGHTNESS_MIN;
    ColorPulse.brightness_max   = CORSAIR_DRAM_BRIGHTNESS_MAX;
    ColorPulse.brightness       = CORSAIR_DRAM_BRIGHTNESS_DEFAULT;
    ColorPulse.colors_min       = 2;
    ColorPulse.colors_max       = 2;
    ColorPulse.colors.resize(2);
    modes.push_back(ColorPulse);

    mode RainbowWave;
    RainbowWave.name            = "Rainbow Wave";
    RainbowWave.value           = CORSAIR_DRAM_MODE_RAINBOW_WAVE;
    RainbowWave.flags           = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_AUTOMATIC_SAVE;
    RainbowWave.color_mode      = MODE_COLORS_NONE;
    RainbowWave.speed_min       = CORSAIR_DRAM_SPEED_SLOW;
    RainbowWave.speed_max       = CORSAIR_DRAM_SPEED_FAST;
    RainbowWave.speed           = CORSAIR_DRAM_SPEED_SLOW;
    RainbowWave.direction       = MODE_DIRECTION_DOWN;
    modes.push_back(RainbowWave);

    mode ColorWave;
    ColorWave.name              = "Color Wave";
    ColorWave.value             = CORSAIR_DRAM_MODE_COLOR_WAVE;
    ColorWave.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    ColorWave.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    ColorWave.speed_min         = CORSAIR_DRAM_SPEED_SLOW;
    ColorWave.speed_max         = CORSAIR_DRAM_SPEED_FAST;
    ColorWave.speed             = CORSAIR_DRAM_SPEED_SLOW;
    ColorWave.direction         = MODE_DIRECTION_DOWN;
    ColorWave.brightness_min    = CORSAIR_DRAM_BRIGHTNESS_MIN;
    ColorWave.brightness_max    = CORSAIR_DRAM_BRIGHTNESS_MAX;
    ColorWave.brightness        = CORSAIR_DRAM_BRIGHTNESS_DEFAULT;
    ColorWave.colors_min        = 2;
    ColorWave.colors_max        = 2;
    ColorWave.colors.resize(2);
    modes.push_back(ColorWave);

    mode Visor;
    Visor.name                  = "Visor";
    Visor.value                 = CORSAIR_DRAM_MODE_VISOR;
    Visor.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_HV | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Visor.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Visor.speed_min             = CORSAIR_DRAM_SPEED_SLOW;
    Visor.speed_max             = CORSAIR_DRAM_SPEED_FAST;
    Visor.speed                 = CORSAIR_DRAM_SPEED_SLOW;
    Visor.direction             = MODE_DIRECTION_VERTICAL;
    Visor.brightness_min        = CORSAIR_DRAM_BRIGHTNESS_MIN;
    Visor.brightness_max        = CORSAIR_DRAM_BRIGHTNESS_MAX;
    Visor.brightness            = CORSAIR_DRAM_BRIGHTNESS_DEFAULT;
    Visor.colors_min            = 2;
    Visor.colors_max            = 2;
    Visor.colors.resize(2);
    modes.push_back(Visor);

    mode Rain;
    Rain.name                   = "Rain";
    Rain.value                  = CORSAIR_DRAM_MODE_RAIN;
    Rain.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Rain.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    Rain.speed_min              = CORSAIR_DRAM_SPEED_SLOW;
    Rain.speed_max              = CORSAIR_DRAM_SPEED_FAST;
    Rain.speed                  = CORSAIR_DRAM_SPEED_SLOW;
    Rain.direction              = MODE_DIRECTION_DOWN;
    Rain.brightness_min         = CORSAIR_DRAM_BRIGHTNESS_MIN;
    Rain.brightness_max         = CORSAIR_DRAM_BRIGHTNESS_MAX;
    Rain.brightness             = CORSAIR_DRAM_BRIGHTNESS_DEFAULT;
    Rain.colors_min             = 2;
    Rain.colors_max             = 2;
    Rain.colors.resize(2);
    modes.push_back(Rain);

    mode Marquee;
    Marquee.name                = "Marquee";
    Marquee.value               = CORSAIR_DRAM_MODE_MARQUEE;
    Marquee.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Marquee.color_mode          = MODE_COLORS_MODE_SPECIFIC;
    Marquee.speed_min           = CORSAIR_DRAM_SPEED_SLOW;
    Marquee.speed_max           = CORSAIR_DRAM_SPEED_FAST;
    Marquee.speed               = CORSAIR_DRAM_SPEED_SLOW;
    Marquee.brightness_min      = CORSAIR_DRAM_BRIGHTNESS_MIN;
    Marquee.brightness_max      = CORSAIR_DRAM_BRIGHTNESS_MAX;
    Marquee.brightness          = CORSAIR_DRAM_BRIGHTNESS_DEFAULT;
    Marquee.colors_min          = 1;
    Marquee.colors_max          = 1;
    Marquee.colors.resize(1);
    modes.push_back(Marquee);

    mode Rainbow;
    Rainbow.name                = "Rainbow";
    Rainbow.value               = CORSAIR_DRAM_MODE_RAINBOW;
    Rainbow.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Rainbow.color_mode          = MODE_COLORS_NONE;
    Rainbow.speed_min           = CORSAIR_DRAM_SPEED_SLOW;
    Rainbow.speed_max           = CORSAIR_DRAM_SPEED_FAST;
    Rainbow.speed               = CORSAIR_DRAM_SPEED_SLOW;
    modes.push_back(Rainbow);

    mode Sequential;
    Sequential.name             = "Sequential";
    Sequential.value            = CORSAIR_DRAM_MODE_SEQUENTIAL;
    Sequential.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Sequential.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    Sequential.speed_min        = CORSAIR_DRAM_SPEED_SLOW;
    Sequential.speed_max        = CORSAIR_DRAM_SPEED_FAST;
    Sequential.speed            = CORSAIR_DRAM_SPEED_SLOW;
    Sequential.direction        = MODE_DIRECTION_DOWN;
    Sequential.brightness_min   = CORSAIR_DRAM_BRIGHTNESS_MIN;
    Sequential.brightness_max   = CORSAIR_DRAM_BRIGHTNESS_MAX;
    Sequential.brightness       = CORSAIR_DRAM_BRIGHTNESS_DEFAULT;
    Sequential.colors_min       = 1;
    Sequential.colors_max       = 1;
    Sequential.colors.resize(1);
    modes.push_back(Sequential);

    SetupZones();
}

RGBController_CorsairDRAM::~RGBController_CorsairDRAM()
{
    delete controller;
}

void RGBController_CorsairDRAM::SetupZones()
{
    /*-----------------------------------------------------*\
    | Set up zone                                           |
    \*-----------------------------------------------------*/
    zone new_zone;
    new_zone.name           = "Corsair DRAM";
    new_zone.type           = ZONE_TYPE_LINEAR;
    new_zone.leds_min       = controller->GetLEDCount();
    new_zone.leds_max       = controller->GetLEDCount();
    new_zone.leds_count     = controller->GetLEDCount();
    zones.push_back(new_zone);

    /*-----------------------------------------------------*\
    | Set up LEDs                                           |
    \*-----------------------------------------------------*/
    for(std::size_t led_idx = 0; led_idx < zones[0].leds_count; led_idx++)
    {
        led new_led;
        new_led.name        = "Corsair DRAM LED ";
        new_led.name.append(std::to_string(led_idx));
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_CorsairDRAM::DeviceUpdateLEDs()
{
    controller->SetColorsPerLED(colors.data());
}

void RGBController_CorsairDRAM::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CorsairDRAM::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CorsairDRAM::DeviceUpdateMode()
{
    unsigned int  corsair_direction = 0;
    bool          random            = (modes[active_mode].color_mode == MODE_COLORS_RANDOM);
    unsigned char mode_colors[6];

    switch(modes[active_mode].direction)
    {
        case MODE_DIRECTION_LEFT:
            corsair_direction = CORSAIR_DRAM_DIRECTION_LEFT;
            break;
        case MODE_DIRECTION_RIGHT:
            corsair_direction = CORSAIR_DRAM_DIRECTION_RIGHT;
            break;
        case MODE_DIRECTION_UP:
            corsair_direction = CORSAIR_DRAM_DIRECTION_UP;
            break;
        case MODE_DIRECTION_DOWN:
            corsair_direction = CORSAIR_DRAM_DIRECTION_DOWN;
            break;
        case MODE_DIRECTION_HORIZONTAL:
            corsair_direction = CORSAIR_DRAM_DIRECTION_HORIZONTAL;
            break;
        case MODE_DIRECTION_VERTICAL:
            corsair_direction = CORSAIR_DRAM_DIRECTION_VERTICAL;
            break;
    }

    mode_colors[0] = 0;
    mode_colors[1] = 0;
    mode_colors[2] = 0;
    mode_colors[3] = 0;
    mode_colors[4] = 0;
    mode_colors[5] = 0;

    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        mode_colors[0] = RGBGetRValue(modes[active_mode].colors[0]);
        mode_colors[1] = RGBGetGValue(modes[active_mode].colors[0]);
        mode_colors[2] = RGBGetBValue(modes[active_mode].colors[0]);

        if(modes[active_mode].colors.size() == 2)
        {
            mode_colors[3] = RGBGetRValue(modes[active_mode].colors[1]);
            mode_colors[4] = RGBGetGValue(modes[active_mode].colors[1]);
            mode_colors[5] = RGBGetBValue(modes[active_mode].colors[1]);
        }
    }

    if(modes[active_mode].name == "Direct")
    {
        controller->SetDirect(true);
    }
    else
    {
        controller->SetDirect(false);
    }

    controller->SetEffect(modes[active_mode].value,
                          modes[active_mode].speed,
                          corsair_direction,
                          random,
                          (unsigned char)modes[active_mode].brightness,
                          mode_colors[0],
                          mode_colors[1],
                          mode_colors[2],
                          mode_colors[3],
                          mode_colors[4],
                          mode_colors[5]);

    std::this_thread::sleep_for(std::chrono::milliseconds(15));
}
