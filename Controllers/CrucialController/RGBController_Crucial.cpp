/*---------------------------------------------------------*\
| RGBController_Crucial.cpp                                 |
|                                                           |
|   RGBController for Crucial Ballistix RAM                 |
|                                                           |
|   Adam Honse (CalcProgrammer1)                19 Jan 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_Crucial.h"

/**------------------------------------------------------------------*\
    @name Crucial RAM
    @category RAM
    @type SMBus
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectCrucialControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_Crucial::RGBController_Crucial(CrucialController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "Crucial DRAM";
    vendor      = "Crucial";
    type        = DEVICE_TYPE_DRAM;
    description = "Crucial DRAM Device";
    version     = controller->GetDeviceVersion();
    location    = controller->GetDeviceLocation();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0xFFFF;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Shift;
    Shift.name       = "Shift";
    Shift.value      = CRUCIAL_MODE_SHIFT;
    Shift.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Shift.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Shift);

    mode GradientShift;
    GradientShift.name       = "Gradient Shift";
    GradientShift.value      = CRUCIAL_MODE_GRADIENT_SHIFT;
    GradientShift.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    GradientShift.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(GradientShift);

    mode Fill;
    Fill.name       = "Fill";
    Fill.value      = CRUCIAL_MODE_FILL;
    Fill.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Fill.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Fill);

    mode Stack;
    Stack.name       = "Stack";
    Stack.value      = CRUCIAL_MODE_STACK;
    Stack.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Stack.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Stack);

    mode DoubleStack;
    DoubleStack.name       = "Double Stack";
    DoubleStack.value      = CRUCIAL_MODE_DOUBLE_STACK;
    DoubleStack.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    DoubleStack.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(DoubleStack);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = CRUCIAL_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode MotionPoint;
    MotionPoint.name       = "Motion Point";
    MotionPoint.value      = CRUCIAL_MODE_MOTION_POINT;
    MotionPoint.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    MotionPoint.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(MotionPoint);

    mode InsideOut;
    InsideOut.name       = "Inside Out";
    InsideOut.value      = CRUCIAL_MODE_INSIDE_OUT;
    InsideOut.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    InsideOut.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(InsideOut);

    mode ColorStep;
    ColorStep.name       = "Color Step";
    ColorStep.value      = CRUCIAL_MODE_COLOR_STEP;
    ColorStep.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    ColorStep.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(ColorStep);

    mode WaterWave;
    WaterWave.name       = "Water Wave (Color Blending)";
    WaterWave.value      = CRUCIAL_MODE_WATER_WAVE;
    WaterWave.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    WaterWave.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(WaterWave);

    mode Flashing;
    Flashing.name       = "Flashing";
    Flashing.value      = CRUCIAL_MODE_FLASHING;
    Flashing.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Flashing.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Flashing);

    mode Static;
    Static.name       = "Static";
    Static.value      = CRUCIAL_MODE_STATIC;
    Static.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    SetupZones();
}

RGBController_Crucial::~RGBController_Crucial()
{
    delete controller;
}

void RGBController_Crucial::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zone                                               |
    \*---------------------------------------------------------*/
    zone new_zone;
    new_zone.name           = "DRAM";
    new_zone.type           = ZONE_TYPE_LINEAR;
    new_zone.leds_min       = 8;
    new_zone.leds_max       = 8;
    new_zone.leds_count     = 8;
    new_zone.matrix_map     = NULL;
    zones.push_back(new_zone);

    /*---------------------------------------------------------*\
    | Set up LEDs                                               |
    \*---------------------------------------------------------*/
    for(std::size_t led_idx = 0; led_idx < zones[0].leds_count; led_idx++)
    {
        led new_led;
        new_led.name        = "DRAM LED ";
        new_led.name.append(std::to_string(led_idx));
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_Crucial::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_Crucial::DeviceUpdateLEDs()
{
    if(modes[active_mode].value == 0xFFFF)
    {
        controller->SetAllColorsDirect(&colors[0]);
    }
    else
    {
        controller->SetAllColorsEffect(&colors[0]);

        if(modes[active_mode].value == CRUCIAL_MODE_STATIC)
        {
            controller->SetMode(modes[active_mode].value);
        }
    }
}

void RGBController_Crucial::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_Crucial::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_Crucial::DeviceUpdateMode()
{
    if(modes[active_mode].value == 0xFFFF)
    {
        controller->SetMode(CRUCIAL_MODE_STATIC);
        controller->SetAllColorsEffect(&colors[0]);
        return;
    }

    if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        controller->SetAllColorsEffect(&colors[0]);
    }

    controller->SetMode(modes[active_mode].value);
}
