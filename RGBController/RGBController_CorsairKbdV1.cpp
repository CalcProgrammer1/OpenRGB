/*-----------------------------------------*\
|  RGBController_CorsairKbdV1.cpp           |
|                                           |
|  Generic RGB Interface for first-gen      |
|  Corsair RGB keyboard                     |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/9/2020    |
\*-----------------------------------------*/

#include "RGBController_CorsairKbdV1.h"

RGBController_CorsairKbdV1::RGBController_CorsairKbdV1(CorsairKbdV1Controller* corsair_ptr)
{
    corsair = corsair_ptr;

    name = "Corsair RGB Keyboard";
    type = DEVICE_TYPE_KEYBOARD;

    for(int i = 0; i < 144; i++)
    {
        colors.push_back(0x00000000);
    }
}

RGBController_CorsairKbdV1::~RGBController_CorsairKbdV1()
{

}

int RGBController_CorsairKbdV1::GetMode()
{
    return 0;
}

void RGBController_CorsairKbdV1::SetMode(int mode)
{

}

void RGBController_CorsairKbdV1::SetCustomMode()
{

}

void RGBController_CorsairKbdV1::UpdateLEDs()
{
    corsair->SetLEDs(colors);
}

void RGBController_CorsairKbdV1::UpdateZoneLEDs(int zone)
{
    corsair->SetLEDs(colors);
}

void RGBController_CorsairKbdV1::UpdateSingleLED(int led)
{
    corsair->SetLEDs(colors);
}