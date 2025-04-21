/*---------------------------------------------------------*\
| i2c_smbus.cpp                                             |
|                                                           |
|   Device-independent i2c/SMBus communication functions    |
|                                                           |
|   Adam Honse (CalcProgrammer1)                08 Aug 2018 |
|   Portions based on Linux source code                     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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
    i2c_smbus_start            = false;
    i2c_smbus_done             = false;
    this->port_id              = -1;
    this->pci_device           = -1;
    this->pci_vendor           = -1;
    this->pci_subsystem_device = -1;
    this->pci_subsystem_vendor = -1;
    this->bus_id               = -1;
    i2c_smbus_thread_running   = true;
    i2c_smbus_thread           = new std::thread(&i2c_smbus_interface::i2c_smbus_thread_function, this);
}

i2c_smbus_interface::~i2c_smbus_interface()
{
    i2c_smbus_thread_running = false;
    i2c_smbus_start = true;
    i2c_smbus_start_cv.notify_all();
    i2c_smbus_thread->join();
    delete i2c_smbus_thread;
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

    i2c_addr        = addr;
    i2c_read_write  = read_write;
    i2c_command     = command;
    i2c_size_smbus  = size;
    i2c_data_smbus  = data;
    smbus_xfer      = true;

    std::unique_lock<std::mutex> start_lock(i2c_smbus_start_mutex);
    i2c_smbus_start = true;
    i2c_smbus_start_cv.notify_all();
    start_lock.unlock();

    std::unique_lock<std::mutex> done_lock(i2c_smbus_done_mutex);

    i2c_smbus_done_cv.wait(done_lock, [this]{ return i2c_smbus_done.load(); });
    i2c_smbus_done  = false;

    i2c_smbus_xfer_mutex.unlock();

    return(i2c_ret);
}

s32 i2c_smbus_interface::i2c_xfer_call(u8 addr, char read_write, int* size, u8 *data)
{
    i2c_smbus_xfer_mutex.lock();

    i2c_addr        = addr;
    i2c_read_write  = read_write;
    i2c_size        = size;
    i2c_data        = data;
    smbus_xfer      = false;

    std::unique_lock<std::mutex> start_lock(i2c_smbus_start_mutex);
    i2c_smbus_start = true;
    i2c_smbus_start_cv.notify_all();
    start_lock.unlock();

    std::unique_lock<std::mutex> done_lock(i2c_smbus_done_mutex);

    i2c_smbus_done_cv.wait(done_lock, [this]{ return i2c_smbus_done.load(); });
    i2c_smbus_done  = false;

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

void i2c_smbus_interface::i2c_smbus_thread_function()
{
    while(1)
    {
        std::unique_lock<std::mutex> start_lock(i2c_smbus_start_mutex);

        i2c_smbus_start_cv.wait(start_lock, [this]{ return i2c_smbus_start.load(); });
        i2c_smbus_start = false;

        if (!i2c_smbus_thread_running.load())
        {
            break;
        }

        if(smbus_xfer)
        {
            i2c_ret = i2c_smbus_xfer(i2c_addr, i2c_read_write, i2c_command, i2c_size_smbus, i2c_data_smbus);
        }
        else
        {
            i2c_ret = i2c_xfer(i2c_addr, i2c_read_write, i2c_size, i2c_data);
        }

        std::unique_lock<std::mutex> done_lock(i2c_smbus_done_mutex);
        i2c_smbus_done  = true;
        i2c_smbus_done_cv.notify_all();
        done_lock.unlock();
    }
}
