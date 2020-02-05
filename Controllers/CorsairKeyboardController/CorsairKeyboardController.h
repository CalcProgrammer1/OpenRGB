/*-----------------------------------------*\
|  CorsairKeyboardController.h              |
|                                           |
|  Definitions and types for Corsair RGB    |
|  keyboard lighting controller             |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/9/2020    |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

class CorsairKeyboardController
{
public:
    CorsairKeyboardController(hid_device* dev_handle);
    ~CorsairKeyboardController();

    void SetLEDsDirect(std::vector<RGBColor> colors);
    void SetLEDs(std::vector<RGBColor> colors);

private:
    hid_device*             dev;
};
