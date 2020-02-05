/*-----------------------------------------*\
|  RGBController_CorsairKeyboard.cpp        |
|                                           |
|  Generic RGB Interface for Corsair RGB    |
|  keyboards                                |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/9/2020    |
\*-----------------------------------------*/

#include "RGBController_CorsairKeyboard.h"

RGBController_CorsairKeyboard::RGBController_CorsairKeyboard(CorsairKeyboardController* corsair_ptr)
{
    corsair = corsair_ptr;

    name = "Corsair RGB Keyboard";
    type = DEVICE_TYPE_KEYBOARD;

    for(int i = 0; i < 144; i++)
    {
        colors.push_back(0x00000000);
    }
}

RGBController_CorsairKeyboard::~RGBController_CorsairKeyboard()
{

}

void RGBController_CorsairKeyboard::UpdateLEDs()
{
    corsair->SetLEDs(colors);
}

void RGBController_CorsairKeyboard::UpdateZoneLEDs(int zone)
{
    corsair->SetLEDs(colors);
}

void RGBController_CorsairKeyboard::UpdateSingleLED(int led)
{
    corsair->SetLEDs(colors);
}

void RGBController_CorsairKeyboard::SetCustomMode()
{

}

void RGBController_CorsairKeyboard::UpdateMode()
{

}
