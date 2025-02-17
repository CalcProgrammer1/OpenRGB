/*---------------------------------------------------------*\
| i2c_smbus_nvapi.h                                         |
|                                                           |
|   Nvidia NvAPI I2C driver                                 |
|                                                           |
|   Adam Honse (CalcProgrammer1)                21 Feb 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "i2c_smbus.h"
#include "nvapi.h"

class i2c_smbus_nvapi : public i2c_smbus_interface
{
public:
    i2c_smbus_nvapi(NV_PHYSICAL_GPU_HANDLE handle);
    s32 i2c_read_block_data(u8 addr, u8 length, u8 *values);
    s32 i2c_write_block_data(u8 addr, u8 length, u8 *values);

private:
    s32 i2c_smbus_xfer(u8 addr, char read_write, u8 command, int mode, i2c_smbus_data* data);
    s32 i2c_xfer(u8 addr, char read_write, int* size, u8* data);
    NV_PHYSICAL_GPU_HANDLE handle;
};
