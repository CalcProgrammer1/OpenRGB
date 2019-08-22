/*-----------------------------------------*\
|  i2c_smbus_linux.h                        |
|                                           |
|  Definitions and types for Linux i2c/smbus|
|  driver                                   |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/14/2019   |
\*-----------------------------------------*/

#include "i2c_smbus.h"

class i2c_smbus_linux : public i2c_smbus_interface
{
public:
    int handle;

private:
    s32 i2c_smbus_xfer(u8 addr, char read_write, u8 command, int size, i2c_smbus_data* data);
};
