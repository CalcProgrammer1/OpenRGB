/*-----------------------------------------*\
|  PoseidonZRGBController.cpp               |
|                                           |
|  Driver for Thermaltake Poseidon Z RGB    |
|  Keyboard lighting controller             |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/25/2019  |
\*-----------------------------------------*/

#include "PoseidonZRGBController.h"

PoseidonZRGBController::PoseidonZRGBController(hid_device* dev_handle)
{
    dev = dev_handle;
}

PoseidonZRGBController::~PoseidonZRGBController()
{

}

char* PoseidonZRGBController::GetDeviceName()
{
    return device_name;
}

void PoseidonZRGBController::SetLEDs(std::vector<RGBColor> colors)
{

}
