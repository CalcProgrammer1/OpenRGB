/*-----------------------------------------*\
|  PoseidonZRGBController.h                 |
|                                           |
|  Definitions and types for Thermaltake    |
|  Poseidon Z RGB Keyboard lighting         |
|  controller                               |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/25/2019  |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include "hidapi.h"

#pragma once

class PoseidonZRGBController
{
public:
    PoseidonZRGBController(hid_device* dev_handle);
    ~PoseidonZRGBController();

    char* GetDeviceName();

    void SetLEDs(std::vector<RGBColor> colors);
    
private:
    char                    device_name[32];
    hid_device*             dev;
};
