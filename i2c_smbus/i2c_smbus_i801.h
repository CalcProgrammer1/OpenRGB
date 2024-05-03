/*---------------------------------------------------------*\
| i2c_smbus_i801.h                                          |
|                                                           |
|   i801 SMBUS driver for Windows                           |
|                                                           |
|   Adam Honse (CalcProgrammer1)                29 Jan 2019 |
|   Portions based on Linux source code                     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "i2c_smbus.h"

#ifdef _WIN32
#include <Windows.h>
#endif

/* BIT shifting macro */
#define BIT(x)          ( 1 << x )

/* I801 SMBus address offsets */
#define SMBHSTSTS       (0 + i801_smba)
#define SMBHSTCNT       (2 + i801_smba)
#define SMBHSTCMD       (3 + i801_smba)
#define SMBHSTADD       (4 + i801_smba)
#define SMBHSTDAT0      (5 + i801_smba)
#define SMBHSTDAT1      (6 + i801_smba)
#define SMBBLKDAT       (7 + i801_smba)
#define SMBPEC          (8 + i801_smba)     /* ICH3 and later */
#define SMBAUXSTS       (12 + i801_smba)    /* ICH4 and later */
#define SMBAUXCTL       (13 + i801_smba)    /* ICH4 and later */
#define SMBSLVSTS       (16 + i801_smba)    /* ICH3 and later */
#define SMBSLVCMD       (17 + i801_smba)    /* ICH3 and later */
#define SMBNTFDADD      (20 + i801_smba)    /* ICH3 and later */


/* Auxiliary status register bits, ICH4+ only */
#define SMBAUXSTS_CRCE      BIT(0)
#define SMBAUXSTS_STCO      BIT(1)

/* Auxiliary control register bits, ICH4+ only */
#define SMBAUXCTL_CRC       BIT(0)
#define SMBAUXCTL_E32B      BIT(1)

/* Other settings */
#define MAX_RETRIES 400

/* I801 command constants */
#define I801_QUICK          0x00
#define I801_BYTE           0x04
#define I801_BYTE_DATA      0x08
#define I801_WORD_DATA      0x0C
#define I801_PROC_CALL      0x10    /* unimplemented */
#define I801_BLOCK_DATA     0x14
#define I801_I2C_BLOCK_DATA 0x18    /* ICH5 and later */

/* I801 Host Control register bits */
#define SMBHSTCNT_INTREN    BIT(0)
#define SMBHSTCNT_KILL      BIT(1)
#define SMBHSTCNT_LAST_BYTE BIT(5)
#define SMBHSTCNT_START     BIT(6)
#define SMBHSTCNT_PEC_EN    BIT(7)  /* ICH3 and later */

/* I801 Hosts Status register bits */
#define SMBHSTSTS_BYTE_DONE     BIT(7)
#define SMBHSTSTS_INUSE_STS     BIT(6)
#define SMBHSTSTS_SMBALERT_STS  BIT(5)
#define SMBHSTSTS_FAILED        BIT(4)
#define SMBHSTSTS_BUS_ERR       BIT(3)
#define SMBHSTSTS_DEV_ERR       BIT(2)
#define SMBHSTSTS_INTR          BIT(1)
#define SMBHSTSTS_HOST_BUSY     BIT(0)

/* Host Notify Status register bits */
#define SMBSLVSTS_HST_NTFY_STS  BIT(0)

/* Host Notify Command register bits */
#define SMBSLVCMD_HST_NTFY_INTREN   BIT(0)

#define STATUS_ERROR_FLAGS  (SMBHSTSTS_FAILED | SMBHSTSTS_BUS_ERR | SMBHSTSTS_DEV_ERR)
#define STATUS_FLAGS        (SMBHSTSTS_BYTE_DONE | SMBHSTSTS_INTR | STATUS_ERROR_FLAGS)

/* I801 Host Config */
#define SMBHSTCFG               0x040
#define SMBHSTCFG_HST_EN        BIT(0)

#ifdef _WIN32
#define GLOBAL_SMBUS_MUTEX_NAME "Global\\Access_SMBUS.HTP.Method"
#endif

class i2c_smbus_i801 : public i2c_smbus_interface
{
public:
    u16 i801_smba = 0xF000;
    i2c_smbus_i801();
    ~i2c_smbus_i801();

private:
    s32 i801_access(u16 addr, char read_write, u8 command, int size, i2c_smbus_data *data);
    int i801_block_transaction(i2c_smbus_data *data, char read_write, int command, int hwpec);
    int i801_block_transaction_byte_by_byte(i2c_smbus_data *data, char read_write, int command, int hwpec);
    int i801_check_post(int status);
    int i801_check_pre();
    int i801_transaction(int xact);
    int i801_wait_byte_done();
    int i801_wait_intr();
    s32 i2c_smbus_xfer(u8 addr, char read_write, u8 command, int size, i2c_smbus_data* data);
    s32 i2c_xfer(u8 addr, char read_write, int* size, u8* data);

#ifdef _WIN32
    HANDLE global_smbus_access_handle = NULL;
#endif
};
