/*-----------------------------------------*\
|  RGBController_AMDWraithPrism.cpp         |
|                                           |
|  Generic RGB Interface for AMD Wraith     |
|  Prism                                    |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/25/2019  |
\*-----------------------------------------*/

#include "RGBController_AMDWraithPrism.h"

RGBController_AMDWraithPrism::RGBController_AMDWraithPrism(AMDWraithPrismController* wraith_ptr)
{
    wraith = wraith_ptr;

    name = "AMD Wraith Prism";
}

RGBController_AMDWraithPrism::~RGBController_AMDWraithPrism()
{

}

int RGBController_AMDWraithPrism::GetMode()
{

}

void RGBController_AMDWraithPrism::SetMode(int mode)
{

}

void RGBController_AMDWraithPrism::SetCustomMode()
{

}

void RGBController_AMDWraithPrism::SetAllLEDs(RGBColor color)
{

}

void RGBController_AMDWraithPrism::SetAllZoneLEDs(int zone, RGBColor color)
{

}

void RGBController_AMDWraithPrism::SetLED(int led, RGBColor color)
{

}

void RGBController_AMDWraithPrism::UpdateLEDs()
{

}