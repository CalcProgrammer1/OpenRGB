/*-----------------------------------------*\
|  i2c_smbus_nuvoton_nct6793d.h             |
|                                           |
|  Nuvoton NCT6793D SMBUS driver for Windows|
|                                           |
|  Adam Honse (CalcProgrammer1) 5/19/2019   |
\*-----------------------------------------*/

#include "i2c_smbus.h"

#pragma once

#define SMBHSTDAT       (0 + nuvoton_nct6793d_smba)
#define SMBBLKSZ        (1 + nuvoton_nct6793d_smba)
#define SMBHSTCMD       (2 + nuvoton_nct6793d_smba)
#define SMBHSTIDX       (3 + nuvoton_nct6793d_smba)  //Index field is the Command field on other controllers
#define SMBHSTCTL       (4 + nuvoton_nct6793d_smba)
#define SMBHSTADD       (5 + nuvoton_nct6793d_smba)
#define SMBHSTERR		(9 + nuvoton_nct6793d_smba)
#define SMBHSTSTS       (0xE + nuvoton_nct6793d_smba)

/* Command register */
#define NCT6793D_READ_BYTE      0
#define NCT6793D_READ_WORD      1
#define NCT6793D_READ_BLOCK     2
#define NCT6793D_BLOCK_WRITE_READ_PROC_CALL 3
#define NCT6793D_PROC_CALL      4
#define NCT6793D_WRITE_BYTE     8
#define NCT6793D_WRITE_WORD     9
#define NCT6793D_WRITE_BLOCK    10

/* Control register */
#define NCT6793D_MANUAL_START   128
#define NCT6793D_SOFT_RESET     64

/* Error register */
#define NCT6793D_NO_ACK         32

/* Status register */
#define NCT6793D_FIFO_EMPTY     1
#define NCT6793D_FIFO_FULL      2
#define NCT6793D_MANUAL_ACTIVE  4

class i2c_smbus_nuvoton_nct6793d : public i2c_smbus_interface
{
public:
    u16 nuvoton_nct6793d_smba = 0x0290;

private:
    s32 nct6793d_access(u16 addr, char read_write, u8 command, int size, i2c_smbus_data *data);
    s32 i2c_smbus_xfer(u8 addr, char read_write, u8 command, int size, i2c_smbus_data* data);

};