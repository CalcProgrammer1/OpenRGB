/*-----------------------------------------*\
|  RGBController_Crucial.cpp                |
|                                           |
|  Generic RGB Interface for Crucial        |
|  Ballistix RGB controller                 |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/19/2020   |
\*-----------------------------------------*/

#include "RGBController_Crucial.h"

int RGBController_Crucial::GetMode()
{
    return 0;
}

void RGBController_Crucial::SetMode(int mode)
{
    mode = ((mode+1) << 4) | 0x0F;
    crucial->SetMode(mode);
}

void RGBController_Crucial::SetCustomMode()
{

}

void RGBController_Crucial::UpdateLEDs()
{
crucial->SetAllColorsDirect(RGBGetRValue(colors[0]),RGBGetGValue(colors[0]),RGBGetBValue(colors[0]));
}

void RGBController_Crucial::UpdateZoneLEDs(int zone)
{

}

void RGBController_Crucial::UpdateSingleLED(int led)
{

}

RGBController_Crucial::RGBController_Crucial(CrucialController * crucial_ptr)
{
    crucial = crucial_ptr;

    location = crucial->GetDeviceLocation();
    type = DEVICE_TYPE_DRAM;
    name = "Crucial DRAM";

    mode Shift;
    Shift.name = "Shift";
    //Shift.value = CRUCIAL_MODE_SHIFT;
    //Shift.flags = 0;
    modes.push_back(Shift);
    
    mode GradientShift;
    GradientShift.name = "Gradient Shift";
    //GradientShift.value = CRUCIAL_MODE_GRADIENT_SHIFT;
    //GradientShift.flags = 0;
    modes.push_back(GradientShift);

    mode Fill;
    Fill.name = "Fill";
    //Fill.value = CRUCIAL_MODE_FILL;
    //Fill.flags = 0;
    modes.push_back(Fill);

    mode Stack;
    Stack.name = "Stack";
    //Stack.value = CRUCIAL_MODE_STACK;
    //Stack.flags = 0;
    modes.push_back(Stack);

    mode DoubleStack;
    DoubleStack.name = "Double Stack";
    //DoubleStack.value = CRUCIAL_MODE_DOUBLE_STACK;
    //DoubleStack.flags = 0;
    modes.push_back(DoubleStack);

    mode Breathing;
    Breathing.name = "Breathing";
    //Breathing.value = CRUCIAL_MODE_BREATHING;
    //Breathing.flags = 0;
    modes.push_back(Breathing);

    mode MotionPoint;
    MotionPoint.name = "Motion Point";
    //MotionPoint.value = CRUCIAL_MODE_MOTION_POINT;
    //MotionPoint.flags = 0;
    modes.push_back(MotionPoint);

    mode InsideOut;
    InsideOut.name = "Inside Out";
    //InsideOut.value = CRUCIAL_MODE_INSIDE_OUT;
    //InsideOut.flags = 0;
    modes.push_back(InsideOut);

    mode ColorStep;
    ColorStep.name = "Color Step";
    //ColorStep.value = CRUCIAL_MODE_COLOR_STEP;
    //ColorStep.flags = 0;
    modes.push_back(ColorStep);

    mode WaterWave;
    WaterWave.name = "Water Wave (Color Blending)";
    //WaterWave.value = CRUCIAL_MODE_WATER_WAVE;
    //WaterWave.flags = 0;
    modes.push_back(WaterWave);

    mode Flashing;
    Flashing.name = "Flashing";
    //Flashing.value = CRUCIAL_MODE_FLASHING;
    //Flashing.flags = 0;
    modes.push_back(Flashing);

    mode Static;
    Static.name = "Static";
    //Static.value = CRUCIAL_MODE_STATIC;
    //Static.flags = 0;
    modes.push_back(Static);

    colors.push_back(0x00000000);
}
