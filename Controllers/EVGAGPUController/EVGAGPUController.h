/*-----------------------------------------*\
|  EVGAGPUController.h                      |
|                                           |
|  Definitions and types for EVGA GPU RGB   |
|  lighting controller                      |
|                                           |
|  Adam Honse (CalcProgrammer1) 9/11/2020   |
\*-----------------------------------------*/

#include <string>
#include "i2c_smbus.h"

#pragma once

typedef unsigned char	evga_dev_id;

enum
{
    EVGA_GPU_REG_MODE                   = 0x0C,
    EVGA_GPU_REG_RED                    = 0x09,
    EVGA_GPU_REG_GREEN                  = 0x0A,
    EVGA_GPU_REG_BLUE                   = 0x0B,
};

enum
{
    EVGA_GPU_MODE_OFF                   = 0x00,
    EVGA_GPU_MODE_CUSTOM                = 0x01,
    EVGA_GPU_MODE_RAINBOW               = 0x02,
    EVGA_GPU_MODE_BREATHING             = 0x05,
};

class EVGAGPUController
{
public:
    EVGAGPUController(i2c_smbus_interface* bus, evga_dev_id dev);
    ~EVGAGPUController();

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
