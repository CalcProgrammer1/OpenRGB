/*-----------------------------------------*\
|  EVGAGPUv2Controller.h                    |
|                                           |
|  Definitions and types for EVGA GPU RGB   |
|  V2 (Turing) lighting controller          |
|                                           |
|  Adam Honse (CalcProgrammer1) 9/13/2020   |
\*-----------------------------------------*/

#include <string>
#include "i2c_smbus.h"

#pragma once

typedef unsigned char	evga_dev_id;

enum
{
    EVGA_GPU_V2_REG_MODE                = 0x60,
    EVGA_GPU_V2_REG_COLOR_A_RED         = 0x6C,
    EVGA_GPU_V2_REG_COLOR_A_GREEN       = 0x6D,
    EVGA_GPU_V2_REG_COLOR_A_BLUE        = 0x6E,
    EVGA_GPU_V2_REG_COLOR_A_BRIGHTNESS  = 0x6F,
    EVGA_GPU_V2_REG_COLOR_B_RED         = 0x70,
    EVGA_GPU_V2_REG_COLOR_B_GREEN       = 0x71,
    EVGA_GPU_V2_REG_COLOR_B_BLUE        = 0x72,
    EVGA_GPU_V2_REG_COLOR_B_BRIGHTNESS  = 0x73,
};

enum
{
    EVGA_GPU_V2_MODE_OFF                = 0x00,
    EVGA_GPU_V2_MODE_CUSTOM             = 0x01,
    EVGA_GPU_V2_MODE_RAINBOW            = 0x0F,
    EVGA_GPU_V2_MODE_BREATHING          = 0x22,
};

class EVGAGPUv2Controller
{
public:
    EVGAGPUv2Controller(i2c_smbus_interface* bus, evga_dev_id dev);
    ~EVGAGPUv2Controller();

    std::string     GetDeviceLocation();

    unsigned char   GetRed();
    unsigned char   GetGreen();
    unsigned char   GetBlue();

    void            SetColor(unsigned char red, unsigned char green, unsigned char blue);
    void            SetMode(unsigned char mode);

private:
    i2c_smbus_interface*    bus;
    evga_dev_id             dev;

};
