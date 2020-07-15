/*-----------------------------------------*\
|  SapphireGPUController.h                  |
|                                           |
|  Definitions and types for Sapphire Nitro |
|  Glow GPU RGB lighting controller         |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/15/2020   |
\*-----------------------------------------*/

#include <string>
#include "i2c_smbus.h"

#pragma once

typedef unsigned char	sapphire_dev_id;

enum
{
    SAPPHIRE_GPU_REG_MODE               = 0x00,
    SAPPHIRE_GPU_REG_BRIGHTNESS         = 0x01,
    SAPPHIRE_GPU_REG_RED                = 0x03,
    SAPPHIRE_GPU_REG_GREEN              = 0x04,
    SAPPHIRE_GPU_REG_BLUE               = 0x05,
};

class SapphireGPUController
{
public:
    SapphireGPUController(i2c_smbus_interface* bus, sapphire_dev_id dev);
    ~SapphireGPUController();

    std::string     GetDeviceLocation();

    void            SetColor(unsigned char red, unsigned char green, unsigned char blue);
    void            SetMode(unsigned char mode, unsigned char speed);

private:
    i2c_smbus_interface*    bus;
    sapphire_dev_id         dev;

};
