/*-----------------------------------------*\
|  RGBController_Crucial.cpp                |
|                                           |
|  Generic RGB Interface for Crucial        |
|  Ballistix RGB controller                 |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/19/2020   |
\*-----------------------------------------*/

#include "RGBController_Crucial.h"

RGBController_Crucial::RGBController_Crucial(CrucialController * crucial_ptr)
{
    crucial = crucial_ptr;

    name        = "Crucial DRAM";
    vendor      = "Crucial";
    type        = DEVICE_TYPE_DRAM;
    description = "Crucial DRAM Device";
    location    = crucial->GetDeviceLocation();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0xFFFF;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Shift;
    Shift.name       = "Shift";
    Shift.value      = CRUCIAL_MODE_SHIFT;
    Shift.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Shift.colors_min = 1;
    Shift.colors_max = 1;
    Shift.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Shift.colors.resize(1);
    modes.push_back(Shift);
    
    mode GradientShift;
    GradientShift.name       = "Gradient Shift";
    GradientShift.value      = CRUCIAL_MODE_GRADIENT_SHIFT;
    GradientShift.flags      = 0;
    GradientShift.color_mode = MODE_COLORS_NONE;
    modes.push_back(GradientShift);

    mode Fill;
    Fill.name       = "Fill";
    Fill.value      = CRUCIAL_MODE_FILL;
    Fill.flags      = 0;
    Fill.color_mode = MODE_COLORS_NONE;
    modes.push_back(Fill);

    mode Stack;
    Stack.name       = "Stack";
    Stack.value      = CRUCIAL_MODE_STACK;
    Stack.flags      = 0;
    Stack.color_mode = MODE_COLORS_NONE;
    modes.push_back(Stack);

    mode DoubleStack;
    DoubleStack.name       = "Double Stack";
    DoubleStack.value      = CRUCIAL_MODE_DOUBLE_STACK;
    DoubleStack.flags      = 0;
    DoubleStack.color_mode = MODE_COLORS_NONE;
    modes.push_back(DoubleStack);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = CRUCIAL_MODE_BREATHING;
    Breathing.flags      = 0;
    Breathing.color_mode = MODE_COLORS_NONE;
    modes.push_back(Breathing);

    mode MotionPoint;
    MotionPoint.name       = "Motion Point";
    MotionPoint.value      = CRUCIAL_MODE_MOTION_POINT;
    MotionPoint.flags      = 0;
    MotionPoint.color_mode = MODE_COLORS_NONE;
    modes.push_back(MotionPoint);

    mode InsideOut;
    InsideOut.name       = "Inside Out";
    InsideOut.value      = CRUCIAL_MODE_INSIDE_OUT;
    InsideOut.flags      = 0;
    InsideOut.color_mode = MODE_COLORS_NONE;
    modes.push_back(InsideOut);

    mode ColorStep;
    ColorStep.name       = "Color Step";
    ColorStep.value      = CRUCIAL_MODE_COLOR_STEP;
    ColorStep.flags      = 0;
    ColorStep.color_mode = MODE_COLORS_NONE;
    modes.push_back(ColorStep);

    mode WaterWave;
    WaterWave.name       = "Water Wave (Color Blending)";
    WaterWave.value      = CRUCIAL_MODE_WATER_WAVE;
    WaterWave.flags      = 0;
    WaterWave.color_mode = MODE_COLORS_NONE;
    modes.push_back(WaterWave);

    mode Flashing;
    Flashing.name       = "Flashing";
    Flashing.value      = CRUCIAL_MODE_FLASHING;
    Flashing.flags      = 0;
    Flashing.color_mode = MODE_COLORS_NONE;
    modes.push_back(Flashing);

    mode Static;
    Static.name       = "Static";
    Static.value      = CRUCIAL_MODE_STATIC;
    Static.flags      = 0;
    Static.color_mode = MODE_COLORS_NONE;
    modes.push_back(Static);

    SetupZones();
}

RGBController_Crucial::~RGBController_Crucial()
{
    delete crucial;
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
        new_led.name = "DRAM LED ";
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
        crucial->SetAllColorsDirect(&colors[0]);
    }
    else
    {
        crucial->SetAllColorsEffect(&colors[0]);
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

void RGBController_Crucial::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_Crucial::DeviceUpdateMode()
{
    crucial->SetMode(modes[active_mode].value);
}
