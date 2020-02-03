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
    poseidon->SetLEDsDirect(colors);
}

void RGBController_PoseidonZRGB::UpdateZoneLEDs(int zone)
{
    poseidon->SetLEDs(colors);
}

void RGBController_PoseidonZRGB::UpdateSingleLED(int led)
{
    poseidon->SetLEDs(colors);
}

void RGBController_PoseidonZRGB::SetCustomMode()
{

}

void RGBController_PoseidonZRGB::UpdateMode()
{

}