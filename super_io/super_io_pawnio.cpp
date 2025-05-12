/*---------------------------------------------------------*\
| super_io_pawnio.cpp                                       |
|                                                           |
|   Functions for interfacing with Super-IO using PawnIO    |
|                                                           |
|   Stephen Horvath (Steve-Tech)                05 May 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "super_io_pawnio.h"

#ifdef _WIN32
#include <Windows.h>
#include "PawnIOLib.h"
#include "i2c_smbus_pawnio.h"
#endif

#if defined(WIN32)
static HANDLE pawnio_handle = NULL;
#endif

static int pawnio_chip_type = 0;

static int addr_to_pawnio(int addr)
{
    switch (addr)
    {
        case 0x2E:
            return 0;
        case 0x4E:
            return 1;
        default:
            return -1;
    }
}


/******************************************************************************************\
*                                                                                          *
*   pawnio_superio_enter                                                                   *
*                                                                                          *
*   Put the Super IO chip into Extended Function Mode                                      *
*                                                                                          *
\******************************************************************************************/

void pawnio_superio_enter(int ioreg)
{
#if defined(WIN32)
    HRESULT status;
    SIZE_T return_size;

    if (pawnio_handle == NULL)
    {
        status = i2c_smbus_pawnio::start_pawnio("superio", &pawnio_handle);
        if (status != S_OK)
        {
            // TODO: Figure out how to handle errors
            return;
        }
    }

    if (pawnio_chip_type == 0)
    {
        int in_reg = addr_to_pawnio(ioreg);
        if (in_reg == -1) {
            return;
        }

        const SIZE_T in_size = 1;
        ULONG64 in[in_size] = {(ULONG64)in_reg};
        const SIZE_T out_size = 1;
        ULONG64 out[out_size];
        status = pawnio_execute(pawnio_handle, "ioctl_detect", in, in_size, out, out_size, &return_size);
        if (status != S_OK || out[0] == 0)
        {
            return;
        }
        pawnio_chip_type = (int)out[0];
    }

    status = pawnio_execute(pawnio_handle, "ioctl_enter", NULL, 0, NULL, 0, &return_size);

    // return status;

#endif
}


/******************************************************************************************\
*                                                                                          *
*   pawnio_superio_outb                                                                    *
*                                                                                          *
*   Write a byte to the Super IO configuration register                                    *
*                                                                                          *
\******************************************************************************************/

void pawnio_superio_outb([[maybe_unused]] int ioreg, int reg, int val)
{
#if defined(WIN32)
    const SIZE_T in_size = 2;
    ULONG64 in[in_size] = {(ULONG64)reg, (ULONG64)val};
    SIZE_T return_size;
    HRESULT status = pawnio_execute(pawnio_handle, "ioctl_write", in, in_size, NULL, 0, &return_size);
    if (status != S_OK)
    {
        return;
    }
#endif
}


/******************************************************************************************\
*                                                                                          *
*   pawnio_superio_inb                                                                     *
*                                                                                          *
*   Read a byte from the Super IO configuration register                                   *
*                                                                                          *
\******************************************************************************************/

int pawnio_superio_inb([[maybe_unused]] int ioreg, int reg)
{
#if defined(WIN32)
    const SIZE_T in_size = 1;
    ULONG64 in[in_size] = {(ULONG64)reg};
    const SIZE_T out_size = 1;
    ULONG64 out[out_size];
    SIZE_T return_size;
    HRESULT status = pawnio_execute(pawnio_handle, "ioctl_read", in, in_size, out, out_size, &return_size);
    if (status != S_OK)
    {
        return -1;
    }
    return (int)out[0];
#endif
}
