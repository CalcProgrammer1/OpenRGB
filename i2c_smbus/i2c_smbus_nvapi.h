/*-----------------------------------------*\
|  i2c_smbus_nvapi.h                        |
|                                           |
|  Definitions and types for NVidia NvAPI   |
|  I2C driver                               |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/21/2020   |
\*-----------------------------------------*/

#include "i2c_smbus.h"

#pragma once

class i2c_smbus_nvapi : public i2c_smbus_interface
{
public:
    i2c_smbus_nvapi();

private:
    s32 i2c_smbus_xfer(u8 addr, char read_write, u8 command, int size, i2c_smbus_data* data);
};