/*-----------------------------------------*\
|  RGBController_Crucial.cpp                |
|                                           |
|  Generic RGB Interface for Crucial        |
|  Ballistix RGB controller                 |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/19/2020   |
\*-----------------------------------------*/

#include "RGBController_Crucial.h"

void RGBController_Crucial::UpdateLEDs()
{
    if(modes[active_mode].value == 0xFFFF)
    {
        crucial->SetAllColorsDirect(&colors[0]);
    }
    else
    {

    }
}

void RGBController_Crucial::UpdateZoneLEDs(int zone)
{
    UpdateLEDs();
}

void RGBController_Crucial::UpdateSingleLED(int led)
{
    UpdateLEDs();
}

RGBController_Crucial::RGBController_Crucial(CrucialController * crucial_ptr)
{
    crucial = crucial_ptr;

    location = crucial->GetDeviceLocation();
    type = DEVICE_TYPE_DRAM;
    name = "Crucial DRAM";

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0xFFFF;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

#if 0
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
    DoubleStack.name = "Double Stack";
    DoubleStack.value = CRUCIAL_MODE_DOUBLE_STACK;
    DoubleStack.flags = 0;
    modes.push_back(DoubleStack);

    mode Breathing;
    Breathing.name = "Breathing";
    Breathing.value = CRUCIAL_MODE_BREATHING;
    Breathing.flags = 0;
    modes.push_back(Breathing);

    mode MotionPoint;
    MotionPoint.name = "Motion Point";
    MotionPoint.value = CRUCIAL_MODE_MOTION_POINT;
    MotionPoint.flags = 0;
    modes.push_back(MotionPoint);

    mode InsideOut;
    InsideOut.name = "Inside Out";
    InsideOut.value = CRUCIAL_MODE_INSIDE_OUT;
    InsideOut.flags = 0;
    modes.push_back(InsideOut);

    mode ColorStep;
    ColorStep.name = "Color Step";
    ColorStep.value = CRUCIAL_MODE_COLOR_STEP;
    ColorStep.flags = 0;
    modes.push_back(ColorStep);

    mode WaterWave;
    WaterWave.name = "Water Wave (Color Blending)";
    WaterWave.value = CRUCIAL_MODE_WATER_WAVE;
    WaterWave.flags = 0;
    modes.push_back(WaterWave);

    mode Flashing;
    Flashing.name = "Flashing";
    Flashing.value = CRUCIAL_MODE_FLASHING;
    Flashing.flags = 0;
    modes.push_back(Flashing);

    mode Static;
    Static.name = "Static";
    Static.value = CRUCIAL_MODE_STATIC;
    Static.flags = 0;
    modes.push_back(Static);
#endif

    zone new_zone;
    new_zone.name = "DRAM";
    new_zone.type = ZONE_TYPE_LINEAR;
    
    std::vector<int> new_zone_map;

    for(int led_idx = 0; led_idx < 8; led_idx++)
    {
        colors.push_back(0x00000000);

        led new_led;
        new_led.name = "DRAM LED";

        leds.push_back(new_led);
        new_zone_map.push_back(led_idx);
    }

    new_zone.map.push_back(new_zone_map);

    zones.push_back(new_zone);
}

void RGBController_Crucial::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_Crucial::UpdateMode()
{
    crucial->SetMode(modes[active_mode].value);
}
