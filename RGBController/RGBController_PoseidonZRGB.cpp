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
}

RGBController_PoseidonZRGB::~RGBController_PoseidonZRGB()
{

}

int RGBController_PoseidonZRGB::GetMode()
{
    return 0;
}

void RGBController_PoseidonZRGB::SetMode(int mode)
{

}

void RGBController_PoseidonZRGB::SetCustomMode()
{

}

void RGBController_PoseidonZRGB::SetAllLEDs(RGBColor color)
{
    for(int i = 0; i < colors.size(); i++)
    {
        colors[i] = color;
    }

    UpdateLEDs();
}

void RGBController_PoseidonZRGB::SetAllZoneLEDs(int zone, RGBColor color)
{
    for (int x = 0; x < zones[zone].map.size(); x++)
    {
        for (int y = 0; y < zones[zone].map[x].size(); y++)
        {
            colors[zones[zone].map[x][y]] = color;
        }
    }

    UpdateLEDs();
}

void RGBController_PoseidonZRGB::SetLED(int led, RGBColor color)
{
    colors[led] = color;

    UpdateLEDs();
}

void RGBController_PoseidonZRGB::UpdateLEDs()
{
    poseidon->SetLEDs(colors);
}