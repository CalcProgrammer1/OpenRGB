/*---------------------------------------------------------*\
| super_io_pawnio.h                                         |
|                                                           |
|   Functions for interfacing with Super-IO using PawnIO    |
|                                                           |
|   Stephen Horvath (Steve-Tech)                05 May 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

void pawnio_superio_enter(int ioreg);

void pawnio_superio_outb(int ioreg, int reg, int val);

int pawnio_superio_inb(int ioreg, int reg);
