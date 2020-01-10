/*-----------------------------------------*\
|  CorsairKbdV1Controller.h                 |
|                                           |
|  Definitions and types for first-gene     |
|  Corsair Kxx keyboard lighting controller |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/9/2020    |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

class CorsairKbdV1Controller
{
public:
    CorsairKbdV1Controller(hid_device* dev_handle);
    ~CorsairKbdV1Controller();

    char* GetDeviceName();

    void SetLEDsDirect(std::vector<RGBColor> colors);
    void SetLEDs(std::vector<RGBColor> colors);
    
private:
    char                    device_name[32];
    hid_device*             dev;
};
