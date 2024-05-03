/*---------------------------------------------------------*\
| super_io.cpp                                              |
|                                                           |
|   Functions for interfacing with Super-IO                 |
|                                                           |
|   Adam Honse (CalcProgrammer1)                11 Feb 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

/******************************************************************************************\
*                                                                                          *
*   Nuvoton Super IO constants                                                             *
*                                                                                          *
\******************************************************************************************/

#define SIO_NCT5577_ID      0xC330  /* Device ID for NCT5577D (C333)        */
#define SIO_NCT6102_ID      0x1060  /* Device ID for NCT6102D/6106D (1061)  */
#define SIO_NCT6793_ID      0xd120  /* Device ID for NCT6793D (D121)        */
#define SIO_NCT6795_ID      0xd350  /* Device ID for NCT6795D (D350)        */
#define SIO_NCT6796_ID      0xd420  /* Device ID for NCT6796D (D421)        */
#define SIO_NCT6797_ID      0xd450  /* Device ID for NCT6797D (D450)        */
#define SIO_NCT6798_ID      0xd428  /* Device ID for NCT6798D (D428)        */
#define SIO_ITE8688_ID      0x8688  /* Device ID for ITE8688  (8688)        */
#define SIO_REG_LOGDEV      0x07    /* Logical Device Register              */
#define SIO_REG_DEVID       0x20    /* Device ID Register                   */
#define SIO_REG_SMBA        0x62    /* SMBus Base Address Register          */
#define SIO_LOGDEV_SMBUS    0x0B    /* Logical Device for SMBus             */
#define SIO_ID_MASK         0xFFF8  /* Device ID mask                       */

void superio_enter(int ioreg);

void superio_outb(int ioreg, int reg, int val);

int superio_inb(int ioreg, int reg);
