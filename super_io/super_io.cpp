/*-----------------------------------------*\
|  super_io.cpp                             |
|                                           |
|  Functions for interfacing with Super-IO  |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/11/2020   |
\*-----------------------------------------*/

#include "super_io.h"

#ifdef WIN32
#include <Windows.h>
#include "inpout32.h"

#pragma comment(lib, "inpout32.lib")
#else
#include <sys/io.h>
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
#ifdef WIN32
    Out32(ioreg, 0x87);
    Out32(ioreg, 0x87);
#else
    outb_p(0x87, ioreg);
    outb_p(0x87, ioreg);
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
#ifdef WIN32
    Out32(ioreg, reg);
    Out32(ioreg + 1, val);
#else
    outb_p(reg, ioreg);
    outb_p(val, ioreg + 1);
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
#ifdef WIN32
    Out32(ioreg, reg);
    return Inp32(ioreg + 1);
#else
    outb_p(reg, ioreg);
    return inb_p(ioreg + 1);
#endif
}