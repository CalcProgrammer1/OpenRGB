/******************************************************************************************\
*                                                                                          *
*   i2c_smbus.cpp                                                                          *
*                                                                                          *
*       Device-independent i2c/SMBus communication functions                               *
*                                                                                          *
\******************************************************************************************/

#include "i2c_smbus.h"
#include <string.h>

s32 i2c_smbus_interface::i2c_smbus_write_quick(u8 addr, u8 value)
{
    return i2c_smbus_xfer(addr, value, 0, I2C_SMBUS_QUICK, NULL);
}

s32 i2c_smbus_interface::i2c_smbus_read_byte(u8 addr)
{
    i2c_smbus_data data;
    if (i2c_smbus_xfer(addr, I2C_SMBUS_READ, 0, I2C_SMBUS_BYTE, &data))
    {
        return -1;
    }
    else
    {
        return data.byte;
    }
}

s32 i2c_smbus_interface::i2c_smbus_write_byte(u8 addr, u8 value)
{
    return i2c_smbus_xfer(addr, I2C_SMBUS_WRITE, value, I2C_SMBUS_BYTE, NULL);
}

s32 i2c_smbus_interface::i2c_smbus_read_byte_data(u8 addr, u8 command)
{
    i2c_smbus_data data;
    if (i2c_smbus_xfer(addr, I2C_SMBUS_READ, command, I2C_SMBUS_BYTE_DATA, &data))
    {
        return -1;
    }
    else
    {
        return data.byte;
    }
}

s32 i2c_smbus_interface::i2c_smbus_write_byte_data(u8 addr, u8 command, u8 value)
{
    i2c_smbus_data data;
    data.byte = value;
    return i2c_smbus_xfer(addr, I2C_SMBUS_WRITE, command, I2C_SMBUS_BYTE_DATA, &data);
}

s32 i2c_smbus_interface::i2c_smbus_read_word_data(u8 addr, u8 command)
{
    i2c_smbus_data data;
    if (i2c_smbus_xfer(addr, I2C_SMBUS_READ, command, I2C_SMBUS_WORD_DATA, &data))
    {
        return -1;
    }
    else
    {
        return data.word;
    }
}

s32 i2c_smbus_interface::i2c_smbus_write_word_data(u8 addr, u8 command, u16 value)
{
    i2c_smbus_data data;
    data.word = value;
    return i2c_smbus_xfer(addr, I2C_SMBUS_WRITE, command, I2C_SMBUS_WORD_DATA, &data);
}

s32 i2c_smbus_interface::i2c_smbus_read_block_data(u8 addr, u8 command, u8 *values)
{
    i2c_smbus_data data;
    if (i2c_smbus_xfer(addr, I2C_SMBUS_READ, command, I2C_SMBUS_BLOCK_DATA, &data))
    {
        return -1;
    }
    else
    {
        memcpy(values, &data.block[1], data.block[0]);
        return data.block[0];
    }
}

s32 i2c_smbus_interface::i2c_smbus_write_block_data(u8 addr, u8 command, u8 length, const u8 *values)
{
    i2c_smbus_data data;
    if (length > I2C_SMBUS_BLOCK_MAX)
    {
        length = I2C_SMBUS_BLOCK_MAX;
    }
    data.block[0] = length;
    memcpy(&data.block[1], values, length);
    return i2c_smbus_xfer(addr, I2C_SMBUS_WRITE, command, I2C_SMBUS_BLOCK_DATA, &data);
}

s32 i2c_smbus_interface::i2c_smbus_read_i2c_block_data(u8 addr, u8 command, u8 length, u8 *values)
{
    i2c_smbus_data data;
    if (length > I2C_SMBUS_BLOCK_MAX)
    {
        length = I2C_SMBUS_BLOCK_MAX;
    }
    data.block[0] = length;
    if (i2c_smbus_xfer(addr, I2C_SMBUS_READ, command, I2C_SMBUS_I2C_BLOCK_DATA, &data))
    {
        return -1;
    }
    else
    {
        memcpy(values, &data.block[1], data.block[0]);
        return data.block[0];
    }
}

s32 i2c_smbus_interface::i2c_smbus_write_i2c_block_data(u8 addr, u8 command, u8 length, const u8 *values)
{
    i2c_smbus_data data;
    if (length > I2C_SMBUS_BLOCK_MAX)
    {
        length = I2C_SMBUS_BLOCK_MAX;
    }
    data.block[0] = length;
    memcpy(&data.block[1], values, length);
    return i2c_smbus_xfer(addr, I2C_SMBUS_WRITE, command, I2C_SMBUS_I2C_BLOCK_DATA, &data);
}
