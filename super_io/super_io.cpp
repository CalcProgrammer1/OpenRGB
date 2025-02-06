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

#include "super_io.h"

#ifdef _WIN32
#include <Windows.h>
#include "OlsApi.h"
#elif _MACOSX_X86_X64
#include "macUSPCIOAccess.h"
#else
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int dev_port_fd;
#endif

/******************************************************************************************\
*                                                                                          *
*   superio_enter                                                                          *
*                                                                                          *
*   Put the Super IO chip into Extended Function Mode                                      *
*                                                                                          *
\******************************************************************************************/

void superio_enter(int ioreg)
{
#if defined(WIN32) || defined(_MACOSX_X86_X64)
    WriteIoPortByte(ioreg, 0x87);
    WriteIoPortByte(ioreg, 0x87);
#else
    unsigned char temp = 0x87;
    dev_port_fd = open("/dev/port", O_RDWR, "rw");

    if (dev_port_fd >= 0)
    {
        lseek(dev_port_fd, ioreg, SEEK_SET);
        if(write(dev_port_fd, &temp, 1) == -1)
        {
            return;
        }

        lseek(dev_port_fd, ioreg, SEEK_SET);
        if(write(dev_port_fd, &temp, 1) == -1)
        {
            return;
        }

        close(dev_port_fd);
    }
#endif
}


/******************************************************************************************\
*                                                                                          *
*   superio_outb                                                                           *
*                                                                                          *
*   Write a byte to the Super IO configuration register                                    *
*                                                                                          *
\******************************************************************************************/

void superio_outb(int ioreg, int reg, int val)
{
#if defined(WIN32) || defined(_MACOSX_X86_X64)
    WriteIoPortByte(ioreg, reg);
    WriteIoPortByte(ioreg + 1, val);
#else
    dev_port_fd = open("/dev/port", O_RDWR, "rw");

    if (dev_port_fd >= 0)
    {
        lseek(dev_port_fd, ioreg, SEEK_SET);
        if(write(dev_port_fd, &reg, 1) == -1)
        {
            return;
        }

        if(write(dev_port_fd, &val, 1) == -1)
        {
            return;
        }

        close(dev_port_fd);
    }
#endif
}


/******************************************************************************************\
*                                                                                          *
*   superio_inb                                                                            *
*                                                                                          *
*   Read a byte to the Super IO configuration register                                     *
*                                                                                          *
\******************************************************************************************/

int superio_inb(int ioreg, int reg)
{
#if defined(WIN32) || defined(_MACOSX_X86_X64)
    WriteIoPortByte(ioreg, reg);
    return ReadIoPortByte(ioreg + 1);
#else
    unsigned char temp = 0;
    dev_port_fd = open("/dev/port", O_RDWR, "rw");

    if (dev_port_fd >= 0)
    {
        lseek(dev_port_fd, ioreg, SEEK_SET);
        if(write(dev_port_fd, &reg, 1) == -1)
        {
            return -1;
        }

        if(read(dev_port_fd, &temp, 1) == -1)
        {
            return -1;
        }

        close(dev_port_fd);

        return((int)temp);
    }
    else
    {
        return -1;
    }
#endif
}
