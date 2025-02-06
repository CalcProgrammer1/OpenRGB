/*---------------------------------------------------------*\
| i2c_smbus_nct6775.h                                       |
|                                                           |
|   Nuvoton NCT67xx SMBUS driver for Windows                |
|                                                           |
|   Adam Honse (CalcProgrammer1)                19 May 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "i2c_smbus.h"
#ifdef _WIN32
#include <Windows.h>
#endif

#define SMBHSTDAT       (0 + nct6775_smba)
#define SMBBLKSZ        (1 + nct6775_smba)
#define SMBHSTCMD       (2 + nct6775_smba)
#define SMBHSTIDX       (3 + nct6775_smba)  //Index field is the Command field on other controllers
#define SMBHSTCTL       (4 + nct6775_smba)
#define SMBHSTADD       (5 + nct6775_smba)
#define SMBHSTERR		(9 + nct6775_smba)
#define SMBHSTSTS       (0xE + nct6775_smba)

/* Command register */
#define NCT6775_READ_BYTE      0
#define NCT6775_READ_WORD      1
#define NCT6775_READ_BLOCK     2
#define NCT6775_BLOCK_WRITE_READ_PROC_CALL 3
#define NCT6775_PROC_CALL      4
#define NCT6775_WRITE_BYTE     8
#define NCT6775_WRITE_WORD     9
#define NCT6775_WRITE_BLOCK    10

/* Control register */
#define NCT6775_MANUAL_START   128
#define NCT6775_SOFT_RESET     64

/* Error register */
#define NCT6775_NO_ACK         32

/* Status register */
#define NCT6775_FIFO_EMPTY     1
#define NCT6775_FIFO_FULL      2
#define NCT6775_MANUAL_ACTIVE  4

/* Other settings */
#define NCT6775_MAX_RETRIES    400

#ifdef _WIN32
#define GLOBAL_SMBUS_MUTEX_NAME "Global\\Access_SMBUS.HTP.Method"
#endif

class i2c_smbus_nct6775: public i2c_smbus_interface
{
public:
    u16 nct6775_smba = 0x0290;
    i2c_smbus_nct6775();
    ~i2c_smbus_nct6775();

private:
    s32 nct6775_access(u16 addr, char read_write, u8 command, int size, i2c_smbus_data *data);
    s32 i2c_smbus_xfer(u8 addr, char read_write, u8 command, int size, i2c_smbus_data* data);
    s32 i2c_xfer(u8 addr, char read_write, int* size, u8* data);

#ifdef _WIN32
    HANDLE global_smbus_access_handle = NULL;
#endif
};
