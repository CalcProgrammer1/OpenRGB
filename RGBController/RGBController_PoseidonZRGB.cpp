/*-----------------------------------------*\
|  RGBController_PoseidonZRGB.cpp           |
|                                           |
|  Generic RGB Interface for Thermaltake    |
|  Poseidon Z RGB Keyboard                  |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/25/2019  |
\*-----------------------------------------*/

#include "RGBController_PoseidonZRGB.h"

RGBController_PoseidonZRGB::RGBController_PoseidonZRGB(PoseidonZRGBController* poseidon_ptr)
{
    poseidon = poseidon_ptr;

    name = "Thermaltake Poseidon Z RGB";
    type = DEVICE_TYPE_KEYBOARD;

    mode Direct;
    Direct.name   = "Direct";
    Direct.value  = POSEIDONZ_MODE_STATIC;
    Direct.flags  = MODE_FLAG_HAS_COLOR | MODE_FLAG_PER_LED_COLOR;
    Direct.random = false;
    modes.push_back(Direct);

    mode Static;
    Static.name   = "Static";
    Static.value  = POSEIDONZ_MODE_STATIC;
    Static.flags  = MODE_FLAG_HAS_COLOR | MODE_FLAG_PER_LED_COLOR;
    Static.random = false;
    modes.push_back(Static);

    mode Wave;
    Wave.name      = "Wave";
    Wave.value     = POSEIDONZ_MODE_WAVE;
    Wave.flags     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Wave.speed_min = POSEIDONZ_SPEED_SLOW;
    Wave.speed_max = POSEIDONZ_SPEED_FAST;
    Wave.random    = false;
    Wave.speed     = POSEIDONZ_SPEED_FAST;
    Wave.direction = MODE_DIRECTION_LEFT;
    modes.push_back(Wave);

    mode Ripple;
    Ripple.name   = "Ripple";
    Ripple.value  = POSEIDONZ_MODE_RIPPLE;
    Ripple.flags  = 0;
    Ripple.random = false;
    modes.push_back(Ripple);

    mode Reactive;
    Reactive.name   = "Reactive";
    Reactive.value  = POSEIDONZ_MODE_REACTIVE;
    Reactive.flags  = 0;
    Reactive.random = false;
    modes.push_back(Reactive);

    for(int i = 0; i < 104; i++)
    {
        colors.push_back(0x00000000);
    }
}

RGBController_PoseidonZRGB::~RGBController_PoseidonZRGB()
{

}

void RGBController_PoseidonZRGB::UpdateLEDs()
{
    if(active_mode == 0)
    {
        poseidon->SetLEDsDirect(colors);
    }
    else
    {
        poseidon->SetLEDs(colors);
    }
}

void RGBController_PoseidonZRGB::UpdateZoneLEDs(int zone)
{
    UpdateLEDs();
}

void RGBController_PoseidonZRGB::UpdateSingleLED(int led)
{
    UpdateLEDs();
}

void RGBController_PoseidonZRGB::SetCustomMode()
{
    SetMode(0);
}

void RGBController_PoseidonZRGB::UpdateMode()
{
    poseidon->SetMode(modes[active_mode].value, modes[active_mode].direction, modes[active_mode].speed);
}
