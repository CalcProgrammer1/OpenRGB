/******************************************************************************************\
*                                                                                          *
*   i2c_smbus.cpp                                                                          *
*                                                                                          *
*       Device-independent i2c/SMBus communication functions                               *
*                                                                                          *
\******************************************************************************************/

#include "i2c_smbus.h"
#include <string.h>

#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>

static void Sleep(unsigned int milliseconds)
{
    usleep(1000 * milliseconds);
}
#endif

i2c_smbus_interface::i2c_smbus_interface()
{
    i2c_smbus_done   = true;
    i2c_smbus_inuse  = false;
    i2c_smbus_thread = new std::thread(&i2c_smbus_interface::i2c_smbus_thread_function, this);
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
    s32 ret_val = 0;

    printf("i2c smbus xfer call\r\n");
    while(i2c_smbus_inuse.load() == true || i2c_smbus_done.load() == false)
    {
        Sleep(1);
    }

    i2c_smbus_inuse = true;

    i2c_addr        = addr;
    i2c_read_write  = read_write;
    i2c_command     = command;
    i2c_size        = size;
    i2c_data        = data;

    i2c_smbus_done  = false;

    while(i2c_smbus_done.load() == false)
    {
        Sleep(1);
    }

    ret_val         = i2c_ret;

    i2c_smbus_inuse = false;

    return(i2c_ret);
}

void i2c_smbus_interface::i2c_smbus_thread_function()
{
    printf( "i2c thread function start\r\n" );

    while(1)
    {
        while(i2c_smbus_inuse.load() == false || i2c_smbus_done.load() == true)
        {
            Sleep(1);
        }

        printf("i2c xfer: addr %02X R/W %d command %02X size %02X dataptr %X\r\n", i2c_addr, i2c_read_write, i2c_command, i2c_size, i2c_data);

        i2c_ret = i2c_smbus_xfer(i2c_addr, i2c_read_write, i2c_command, i2c_size, i2c_data);

        i2c_smbus_done = true;
    }
}