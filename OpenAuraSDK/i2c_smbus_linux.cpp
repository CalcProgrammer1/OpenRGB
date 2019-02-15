/*-----------------------------------------*\
|  i2c_smbus_linux.cpp                      |
|                                           |
|  Linux i2c/smbus driver                   |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/14/2019   |
\*-----------------------------------------*/

#include "i2c_smbus.h"
#include "i2c_smbus_linux.h"

#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>

s32 i2c_smbus_linux::i2c_smbus_xfer(u8 addr, char read_write, u8 command, int size, union i2c_smbus_data* data)
{
    struct i2c_smbus_ioctl_data args;

    //Tell I2C host which slave address to transfer to
    ioctl(handle, I2C_SLAVE, addr);

    args.read_write = read_write;
    args.command = command;
    args.size = size;
    args.data = data;

    return ioctl(handle, I2C_SMBUS, &args);
}
