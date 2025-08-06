/*---------------------------------------------------------*\
| i2c_smbus.h                                               |
|                                                           |
|   Device-independent i2c/SMBus communication functions    |
|                                                           |
|   Adam Honse (CalcProgrammer1)                08 Aug 2018 |
|   Portions based on Linux source code                     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#ifndef I2C_SMBUS_H
#define I2C_SMBUS_H

#include <atomic>
#include <thread>
#include <condition_variable>
#include <mutex>

typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned int    u32;
typedef int             s32;

#ifdef _WIN32

//Data for SMBus Messages
#define I2C_SMBUS_BLOCK_MAX     32

union i2c_smbus_data
{
    u8          byte;
    u16         word;
    u8          block[I2C_SMBUS_BLOCK_MAX + 2];
};

#endif /* _WIN32 */

#ifdef __linux__

#include <linux/i2c.h>

#endif  /* __linux__ */

#if defined(__APPLE__) || defined(__FreeBSD__)

//Data for SMBus Messages
#define I2C_SMBUS_BLOCK_MAX     32

union i2c_smbus_data
{
    u8          byte;
    u16         word;
    u8          block[I2C_SMBUS_BLOCK_MAX + 2];
};

#endif /* __APPLE__ or __FreeBSD__ */

// i2c_smbus_xfer read or write markers
#define I2C_SMBUS_READ  1
#define I2C_SMBUS_WRITE 0

// SMBus transaction types (size parameter in the above functions)
#define I2C_SMBUS_QUICK             0
#define I2C_SMBUS_BYTE              1
#define I2C_SMBUS_BYTE_DATA         2
#define I2C_SMBUS_WORD_DATA         3
#define I2C_SMBUS_PROC_CALL         4
#define I2C_SMBUS_BLOCK_DATA        5
#define I2C_SMBUS_I2C_BLOCK_BROKEN  6
#define I2C_SMBUS_BLOCK_PROC_CALL   7           /* SMBus 2.0 */
#define I2C_SMBUS_I2C_BLOCK_DATA    8


class i2c_smbus_interface
{
public:
    char device_name[512];

    int port_id;
    int pci_device;
    int pci_vendor;
    int pci_subsystem_device;
    int pci_subsystem_vendor;

    int bus_id;

    i2c_smbus_interface();
    virtual ~i2c_smbus_interface();

    //Functions derived from i2c-core.c
    s32 i2c_smbus_write_quick(u8 addr, u8 value);
    s32 i2c_smbus_read_byte(u8 addr);
    s32 i2c_smbus_write_byte(u8 addr, u8 value);
    s32 i2c_smbus_read_byte_data(u8 addr, u8 command);
    s32 i2c_smbus_write_byte_data(u8 addr, u8 command, u8 value);
    s32 i2c_smbus_read_word_data(u8 addr, u8 command);
	s32 i2c_smbus_write_word_data(u8 addr, u8 command, u16 value);
    s32 i2c_smbus_read_block_data(u8 addr, u8 command, u8 *values);
	s32 i2c_smbus_write_block_data(u8 addr, u8 command, u8 length, const u8 *values);
    s32 i2c_smbus_read_i2c_block_data(u8 addr, u8 command, u8 length, u8 *values);
    s32 i2c_smbus_write_i2c_block_data(u8 addr, u8 command, u8 length, const u8 *values);

    //Addtional functions added for pure I2C block operations
    s32 i2c_read_block(u8 addr, int* size, u8* data);
    s32 i2c_write_block(u8 addr, int size, u8* data);

    //Handle SMBus and I2C transfer calls in a single thread
    s32 i2c_smbus_xfer_call(u8 addr, char read_write, u8 command, int size, i2c_smbus_data* data);
    s32 i2c_xfer_call(u8 addr, char read_write, int* size, u8 *data);

    virtual s32 i2c_smbus_xfer(u8 addr, char read_write, u8 command, int size, i2c_smbus_data* data) = 0;
    virtual s32 i2c_xfer(u8 addr, char read_write, int* size, u8* data) = 0;

private:
    std::mutex              i2c_smbus_xfer_mutex;

};

#endif /* I2C_SMBUS_H */
