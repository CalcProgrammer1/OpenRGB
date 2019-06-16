/*-----------------------------------------*\
|  RGBController_OpenRazer.h                |
|                                           |
|  Generic RGB Interface for OpenRazer      |
|  kernel drivers for Chroma peripherals    |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/15/2019   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include <fstream>

class RGBController_OpenRazer : public RGBController
{
public:
    RGBController_OpenRazer(std::string dev_path);
    int GetMode();
    void SetMode(int mode);
    void SetAllLEDs(RGBColor color);
    void SetAllZoneLEDs(int zone, RGBColor color);
    void SetLED(int led, RGBColor color);

private:
    unsigned int type;
    std::vector<RGBColor> color_buffer;

    //OpenRazer Sysfs Entries
    std::ofstream matrix_effect_custom;
    std::ofstream matrix_custom_frame;
};