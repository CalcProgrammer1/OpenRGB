/*---------------------------------------------------------*\
| RGBController_CorsairVengeancePro.cpp                     |
|                                                           |
|   RGBController for Corsair Vengeance Pro RGB RAM         |
|                                                           |
|   Adam Honse (CalcProgrammer1)                30 Jun 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_CorsairVengeancePro.h"

/**------------------------------------------------------------------*\
    @name Corsair Vengeance Pro
    @category RAM
    @type SMBus
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectCorsairVengeanceProControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_CorsairVengeancePro::RGBController_CorsairVengeancePro(CorsairVengeanceProController* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller->GetDeviceName();
    vendor      = "Corsair";
    type        = DEVICE_TYPE_DRAM;
    description = "Corsair Vengeance Pro RGB Device";
    location    = controller->GetDeviceLocation();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = CORSAIR_PRO_MODE_DIRECT;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.speed_min  = 0;
    Direct.speed_max  = 0;
    Direct.speed      = 0;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name       = "Static";
    Static.value      = CORSAIR_PRO_MODE_STATIC;
    Static.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.speed_min  = 0;
    Static.speed_max  = 0;
    Static.speed      = 0;
    Static.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode ColorShift;
    ColorShift.name       = "Color Shift";
    ColorShift.value      = CORSAIR_PRO_MODE_COLOR_SHIFT;
    ColorShift.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    ColorShift.speed_min  = CORSAIR_PRO_SPEED_SLOW;
    ColorShift.speed_max  = CORSAIR_PRO_SPEED_FAST;
    ColorShift.colors_min = 2;
    ColorShift.colors_max = 2;
    ColorShift.speed      = CORSAIR_PRO_SPEED_SLOW;
    ColorShift.color_mode = MODE_COLORS_MODE_SPECIFIC;
    ColorShift.colors.resize(2);
    modes.push_back(ColorShift);

    mode ColorPulse;
    ColorPulse.name       = "Color Pulse";
    ColorPulse.value      = CORSAIR_PRO_MODE_COLOR_PULSE;
    ColorPulse.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    ColorPulse.speed_min  = CORSAIR_PRO_SPEED_SLOW;
    ColorPulse.speed_max  = CORSAIR_PRO_SPEED_FAST;
    ColorPulse.colors_min = 2;
    ColorPulse.colors_max = 2;
    ColorPulse.speed      = CORSAIR_PRO_SPEED_SLOW;
    ColorPulse.color_mode = MODE_COLORS_MODE_SPECIFIC;
    ColorPulse.colors.resize(2);
    modes.push_back(ColorPulse);

    mode RainbowWave;
    RainbowWave.name       = "Rainbow Wave";
    RainbowWave.value      = CORSAIR_PRO_MODE_RAINBOW_WAVE;
    RainbowWave.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD;
    RainbowWave.speed_min  = CORSAIR_PRO_SPEED_SLOW;
    RainbowWave.speed_max  = CORSAIR_PRO_SPEED_FAST;
    RainbowWave.speed      = CORSAIR_PRO_SPEED_SLOW;
    RainbowWave.direction  = MODE_DIRECTION_DOWN;
    RainbowWave.color_mode = MODE_COLORS_NONE;
    modes.push_back(RainbowWave);

    mode ColorWave;
    ColorWave.name       = "Color Wave";
    ColorWave.value      = CORSAIR_PRO_MODE_COLOR_WAVE;
    ColorWave.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    ColorWave.speed_min  = CORSAIR_PRO_SPEED_SLOW;
    ColorWave.speed_max  = CORSAIR_PRO_SPEED_FAST;
    ColorWave.colors_min = 2;
    ColorWave.colors_max = 2;
    ColorWave.speed      = CORSAIR_PRO_SPEED_SLOW;
    ColorWave.direction  = MODE_DIRECTION_DOWN;
    ColorWave.color_mode = MODE_COLORS_MODE_SPECIFIC;
    ColorWave.colors.resize(2);
    modes.push_back(ColorWave);

    mode Visor;
    Visor.name       = "Visor";
    Visor.value      = CORSAIR_PRO_MODE_VISOR;
    Visor.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_HV | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Visor.speed_min  = CORSAIR_PRO_SPEED_SLOW;
    Visor.speed_max  = CORSAIR_PRO_SPEED_FAST;
    Visor.colors_min = 2;
    Visor.colors_max = 2;
    Visor.speed      = CORSAIR_PRO_SPEED_SLOW;
    Visor.direction  = MODE_DIRECTION_VERTICAL;
    Visor.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Visor.colors.resize(2);
    modes.push_back(Visor);

    mode Rain;
    Rain.name       = "Rain";
    Rain.value      = CORSAIR_PRO_MODE_RAIN;
    Rain.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Rain.speed_min  = CORSAIR_PRO_SPEED_SLOW;
    Rain.speed_max  = CORSAIR_PRO_SPEED_FAST;
    Rain.colors_min = 2;
    Rain.colors_max = 2;
    Rain.speed      = CORSAIR_PRO_SPEED_SLOW;
    Rain.direction  = MODE_DIRECTION_DOWN;
    Rain.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Rain.colors.resize(2);
    modes.push_back(Rain);

    mode Marquee;
    Marquee.name       = "Marquee";
    Marquee.value      = CORSAIR_PRO_MODE_MARQUEE;
    Marquee.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Marquee.speed_min  = CORSAIR_PRO_SPEED_SLOW;
    Marquee.speed_max  = CORSAIR_PRO_SPEED_FAST;
    Marquee.colors_min = 1;
    Marquee.colors_max = 1;
    Marquee.speed      = CORSAIR_PRO_SPEED_SLOW;
    Marquee.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Marquee.colors.resize(1);
    modes.push_back(Marquee);

    mode Rainbow;
    Rainbow.name       = "Rainbow";
    Rainbow.value      = CORSAIR_PRO_MODE_RAINBOW;
    Rainbow.flags      = MODE_FLAG_HAS_SPEED;
    Rainbow.speed_min  = CORSAIR_PRO_SPEED_SLOW;
    Rainbow.speed_max  = CORSAIR_PRO_SPEED_FAST;
    Rainbow.speed      = CORSAIR_PRO_SPEED_SLOW;
    Rainbow.color_mode = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode Sequential;
    Sequential.name       = "Sequential";
    Sequential.value      = CORSAIR_PRO_MODE_SEQUENTIAL;
    Sequential.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Sequential.speed_min  = CORSAIR_PRO_SPEED_SLOW;
    Sequential.speed_max  = CORSAIR_PRO_SPEED_FAST;
    Sequential.colors_min = 1;
    Sequential.colors_max = 1;
    Sequential.speed      = CORSAIR_PRO_SPEED_SLOW;
    Sequential.direction  = MODE_DIRECTION_DOWN;
    Sequential.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Sequential.colors.resize(1);
    modes.push_back(Sequential);

    SetupZones();

    active_mode = 9;
}

RGBController_CorsairVengeancePro::~RGBController_CorsairVengeancePro()
{
    delete controller;
}

void RGBController_CorsairVengeancePro::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zone                                               |
    \*---------------------------------------------------------*/
    zone new_zone;
    new_zone.name           = "Corsair Pro Zone";
    new_zone.type           = ZONE_TYPE_LINEAR;
    new_zone.leds_min       = controller->GetLEDCount();
    new_zone.leds_max       = controller->GetLEDCount();
    new_zone.leds_count     = controller->GetLEDCount();
    new_zone.matrix_map     = NULL;
    zones.push_back(new_zone);

    /*---------------------------------------------------------*\
    | Set up LEDs                                               |
    \*---------------------------------------------------------*/
    for(std::size_t led_idx = 0; led_idx < zones[0].leds_count; led_idx++)
    {
        led* new_led        = new led();
        new_led->name       = "Corsair Pro LED ";
        new_led->name.append(std::to_string(led_idx));
        leds.push_back(*new_led);
    }

    SetupColors();
}

void RGBController_CorsairVengeancePro::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_CorsairVengeancePro::DeviceUpdateLEDs()
{
    for(unsigned int led = 0; led < (unsigned int)colors.size(); led++)
    {
        RGBColor      color = colors[led];
        unsigned char red   = RGBGetRValue(color);
        unsigned char grn   = RGBGetGValue(color);
        unsigned char blu   = RGBGetBValue(color);
        controller->SetLEDColor(led, red, grn, blu);
    }

    controller->ApplyColors();
}

void RGBController_CorsairVengeancePro::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CorsairVengeancePro::UpdateSingleLED(int led)
{
    RGBColor      color = colors[led];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    controller->SetLEDColor(led, red, grn, blu);
    controller->ApplyColors();
}

void RGBController_CorsairVengeancePro::DeviceUpdateMode()
{
    unsigned int  corsair_direction = 0;
    bool          random            = (modes[active_mode].color_mode == MODE_COLORS_RANDOM);
    unsigned char mode_colors[6];

    switch(modes[active_mode].direction)
    {
        case MODE_DIRECTION_LEFT:
            corsair_direction = CORSAIR_PRO_DIRECTION_LEFT;
            break;
        case MODE_DIRECTION_RIGHT:
            corsair_direction = CORSAIR_PRO_DIRECTION_RIGHT;
            break;
        case MODE_DIRECTION_UP:
            corsair_direction = CORSAIR_PRO_DIRECTION_UP;
            break;
        case MODE_DIRECTION_DOWN:
            corsair_direction = CORSAIR_PRO_DIRECTION_DOWN;
            break;
        case MODE_DIRECTION_HORIZONTAL:
            corsair_direction = CORSAIR_PRO_DIRECTION_HORIZONTAL;
            break;
        case MODE_DIRECTION_VERTICAL:
            corsair_direction = CORSAIR_PRO_DIRECTION_VERTICAL;
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

    if (modes[active_mode].name == "Direct")
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
                          mode_colors[0],
                          mode_colors[1],
                          mode_colors[2],
                          mode_colors[3],
                          mode_colors[4],
                          mode_colors[5]);

    std::this_thread::sleep_for(std::chrono::milliseconds(15));
}
