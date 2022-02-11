/*-----------------------------------------*\
|  EVGAGPUv1Controller.h                    |
|                                           |
|  Definitions and types for EVGA GPU RGB   |
|  V1 (Pascal) lighting controller          |
|                                           |
|  Adam Honse (CalcProgrammer1) 9/11/2020   |
\*-----------------------------------------*/

#include <string>
#include "i2c_smbus.h"

#pragma once

typedef unsigned char	evga_dev_id;

#define EVGAGPUV1_CONTROLLER_NAME   "EVGAv1"

enum
{
    EVGA_GPU_V1_REG_MODE                = 0x0C,
    EVGA_GPU_V1_REG_RED                 = 0x09,
    EVGA_GPU_V1_REG_GREEN               = 0x0A,
    EVGA_GPU_V1_REG_BLUE                = 0x0B,
};

enum
{
    EVGA_GPU_V1_MODE_OFF                = 0x00,
    EVGA_GPU_V1_MODE_CUSTOM             = 0x01,
    EVGA_GPU_V1_MODE_RAINBOW            = 0x02,
    EVGA_GPU_V1_MODE_BREATHING          = 0x05,
};

class EVGAGPUv1Controller
{
public:
    EVGAGPUv1Controller(i2c_smbus_interface* bus, evga_dev_id dev);
    ~EVGAGPUv1Controller();

    std::string     GetDeviceLocation();

    unsigned char   GetMode();
    unsigned char   GetRed();
    unsigned char   GetGreen();
    unsigned char   GetBlue();

    void            SetColor(unsigned char red, unsigned char green, unsigned char blue);
    void            SetMode(unsigned char mode);
    void            SaveSettings();

private:
    i2c_smbus_interface*    bus;
    evga_dev_id             dev;

};
