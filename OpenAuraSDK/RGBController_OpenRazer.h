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
    void SetCustomMode();
    void SetAllLEDs(RGBColor color);
    void SetAllZoneLEDs(int zone, RGBColor color);
    void SetLED(int led, RGBColor color);

private:
    unsigned int type;
    std::vector<RGBColor> color_buffer;

    void SetupMatrixDevice(std::string dev_path);
    void SetupNonMatrixDevice(std::string dev_path);

    bool matrix_device;

    //OpenRazer Sysfs Entries for Matrix Devices
    std::ofstream matrix_custom_frame;
    std::ofstream matrix_effect_custom;
    std::ofstream matrix_effect_breath;
    std::ofstream matrix_effect_none;
    std::ofstream matrix_effect_reactive;
    std::ofstream matrix_effect_spectrum;
    std::ofstream matrix_effect_static;
    std::ofstream matrix_effect_wave;

    //OpenRazer Sysfs Entries for Non-Matrix Devices
    std::ofstream logo_led_effect;
    std::ofstream logo_led_rgb;
    std::ofstream scroll_led_effect;
    std::ofstream scroll_led_rgb;
};