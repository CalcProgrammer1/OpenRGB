/*---------------------------------------------------------*\
|  Definitions for Philips Hue                              |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 9/15/2020        |
\*---------------------------------------------------------*/

#include "RGBController.h"
#include "Hue.h"

#include <string>
#include <vector>

#pragma once

class PhilipsHueController
{
public:
    PhilipsHueController(HueLight& light_ptr);
    ~PhilipsHueController();

    void SetColor(unsigned char red, unsigned char green, unsigned char blue);

private:
    HueLight    light;
};
