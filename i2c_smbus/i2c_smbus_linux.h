/*---------------------------------------------------------*\
| i2c_smbus_linux.h                                         |
|                                                           |
|   Linux i2c/smbus driver                                  |
|                                                           |
|   Adam Honse (CalcProgrammer1)                14 Feb 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "i2c_smbus.h"

class i2c_smbus_linux : public i2c_smbus_interface
{
public:
    int handle;

private:
    s32 i2c_smbus_xfer(u8 addr, char read_write, u8 command, int size, i2c_smbus_data* data);
    s32 i2c_xfer(u8 addr, char read_write, int* size, u8* data);
};
