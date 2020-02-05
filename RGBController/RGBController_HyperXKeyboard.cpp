/*-----------------------------------------*\
|  RGBController_HyperXKeyboard.cpp         |
|                                           |
|  Generic RGB Interface for HyperX RGB     |
|  Keyboard                                 |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/2/2020    |
\*-----------------------------------------*/

#include "RGBController_HyperXKeyboard.h"

RGBController_HyperXKeyboard::RGBController_HyperXKeyboard(HyperXKeyboardController* hyperx_ptr)
{
    hyperx = hyperx_ptr;

    name = "HyperX RGB Keyboard";
    type = DEVICE_TYPE_KEYBOARD;

    mode Direct;
    Direct.name   = "Direct";
    Direct.value  = HYPERX_MODE_STATIC;
    Direct.flags  = MODE_FLAG_HAS_COLOR | MODE_FLAG_PER_LED_COLOR;
    Direct.random = false;
    modes.push_back(Direct);

    mode Static;
    Static.name   = "Static";
    Static.value  = HYPERX_MODE_STATIC;
    Static.flags  = MODE_FLAG_HAS_COLOR | MODE_FLAG_PER_LED_COLOR;
    Static.random = false;
    modes.push_back(Static);

    mode Wave;
    Wave.name      = "Wave";
    Wave.value     = HYPERX_MODE_WAVE;
    Wave.flags     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Wave.speed_min = 0x00;
    Wave.speed_max = 0x09;
    Wave.random    = false;
    Wave.speed     = 0x09;
    Wave.direction = MODE_DIRECTION_LEFT;
    modes.push_back(Wave);

    mode Breathing;
    Breathing.name = "Breathing";
    Breathing.value = HYPERX_MODE_BREATHING;
    Breathing.flags = MODE_FLAG_HAS_SPEED;
    Breathing.speed_min = 0x00;
    Breathing.speed_max = 0x09;
    Breathing.random = false;
    Breathing.speed = 0x09;
    modes.push_back(Breathing);

    for(int i = 0; i < 126; i++)
    {
        colors.push_back(0x00000000);
    }
}

RGBController_HyperXKeyboard::~RGBController_HyperXKeyboard()
{

}

void RGBController_HyperXKeyboard::UpdateLEDs()
{
    if(active_mode == 0)
    {
        hyperx->SetLEDsDirect(colors);
    }
    else
    {
        hyperx->SetLEDs(colors);
    }
}

void RGBController_HyperXKeyboard::UpdateZoneLEDs(int zone)
{
    UpdateLEDs();
}

void RGBController_HyperXKeyboard::UpdateSingleLED(int led)
{
    UpdateLEDs();
}

void RGBController_HyperXKeyboard::SetCustomMode()
{
    SetMode(0);
}

void RGBController_HyperXKeyboard::UpdateMode()
{
    hyperx->SetMode(modes[active_mode].value, modes[active_mode].direction, modes[active_mode].speed);
}
