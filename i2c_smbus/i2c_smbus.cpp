/*---------------------------------------------------------*\
| i2c_smbus.cpp                                             |
|                                                           |
|   Device-independent i2c/SMBus communication functions    |
|                                                           |
|   Adam Honse (CalcProgrammer1)                08 Aug 2018 |
|   Portions based on Linux source code                     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "i2c_smbus.h"
#include <string.h>

#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

i2c_smbus_interface::i2c_smbus_interface()
{
    this->port_id              = -1;
    this->pci_device           = -1;
    this->pci_vendor           = -1;
    this->pci_subsystem_device = -1;
    this->pci_subsystem_vendor = -1;
    this->bus_id               = -1;
}

i2c_smbus_interface::~i2c_smbus_interface()
{
}

s32 i2c_smbus_interface::i2c_smbus_write_quick(u8 addr, u8 value)
{
    return i2c_smbus_xfer_call(addr, value, 0, I2C_SMBUS_QUICK, NULL);
}

s32 i2c_smbus_interface::i2c_smbus_read_byte(u8 addr)
{
    i2c_smbus_data data;
    if (i2c_smbus_xfer_call(addr, I2C_SMBUS_READ, 0, I2C_SMBUS_BYTE, &data))
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
    return i2c_smbus_xfer_call(addr, I2C_SMBUS_WRITE, value, I2C_SMBUS_BYTE, NULL);
}

s32 i2c_smbus_interface::i2c_smbus_read_byte_data(u8 addr, u8 command)
{
    i2c_smbus_data data;
    if (i2c_smbus_xfer_call(addr, I2C_SMBUS_READ, command, I2C_SMBUS_BYTE_DATA, &data))
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
    return i2c_smbus_xfer_call(addr, I2C_SMBUS_WRITE, command, I2C_SMBUS_BYTE_DATA, &data);
}

s32 i2c_smbus_interface::i2c_smbus_read_word_data(u8 addr, u8 command)
{
    i2c_smbus_data data;
    if (i2c_smbus_xfer_call(addr, I2C_SMBUS_READ, command, I2C_SMBUS_WORD_DATA, &data))
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
    return i2c_smbus_xfer_call(addr, I2C_SMBUS_WRITE, command, I2C_SMBUS_WORD_DATA, &data);
}

s32 i2c_smbus_interface::i2c_smbus_read_block_data(u8 addr, u8 command, u8 *values)
{
    i2c_smbus_data data;
    if (i2c_smbus_xfer_call(addr, I2C_SMBUS_READ, command, I2C_SMBUS_BLOCK_DATA, &data))
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
    return i2c_smbus_xfer_call(addr, I2C_SMBUS_WRITE, command, I2C_SMBUS_BLOCK_DATA, &data);
}

s32 i2c_smbus_interface::i2c_smbus_read_i2c_block_data(u8 addr, u8 command, u8 length, u8 *values)
{
    i2c_smbus_data data;
    if (length > I2C_SMBUS_BLOCK_MAX)
    {
        length = I2C_SMBUS_BLOCK_MAX;
    }
    data.block[0] = length;
    if (i2c_smbus_xfer_call(addr, I2C_SMBUS_READ, command, I2C_SMBUS_I2C_BLOCK_DATA, &data))
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
    return i2c_smbus_xfer_call(addr, I2C_SMBUS_WRITE, command, I2C_SMBUS_I2C_BLOCK_DATA, &data);
}

s32 i2c_smbus_interface::i2c_smbus_xfer_call(u8 addr, char read_write, u8 command, int size, i2c_smbus_data* data)
{
    i2c_smbus_xfer_mutex.lock();

    s32 i2c_ret = i2c_smbus_xfer(addr, read_write, command, size, data);

    i2c_smbus_xfer_mutex.unlock();

    return(i2c_ret);
}

s32 i2c_smbus_interface::i2c_xfer_call(u8 addr, char read_write, int* size, u8 *data)
{
    i2c_smbus_xfer_mutex.lock();

    s32 i2c_ret = i2c_xfer(addr, read_write, size, data);

    i2c_smbus_xfer_mutex.unlock();

    return(i2c_ret);
}

s32 i2c_smbus_interface::i2c_read_block(u8 addr, int* size, u8* data)
{
    return i2c_xfer_call(addr, I2C_SMBUS_READ, size, data);
}

s32 i2c_smbus_interface::i2c_write_block(u8 addr, int size, u8 *data)
{
    return i2c_xfer_call(addr, I2C_SMBUS_WRITE, &size, data);
}
