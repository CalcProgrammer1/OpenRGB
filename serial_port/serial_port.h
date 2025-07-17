/*---------------------------------------------------------*\
| serial_port.h                                             |
|                                                           |
|   Cross Platform Serial COM Library for Windows and Linux |
|   This library provides access to serial ports with a     |
|   common API for both Windows and Linux systems           |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      21 Jan 2013 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <string.h>
#include <stdio.h>
#include <vector>
#include <string>

#ifdef _WIN32
/*---------------------------------------------------------*\
| Windows interferes with std::max unless NOMINMAX defined  |
\*---------------------------------------------------------*/
#define NOMINMAX
#include <windows.h>

#endif /* _WIN32 */

#ifdef __linux__

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/file.h>
#include <sys/ioctl.h>

//these types are redefined in asm/termios.h
//to prevent compiler errors from multply
//defining them, use a #define to rename them -
//essentially to undef them before they are redefined
#define winsize undefine_winsize
#define termio undefine_termio
#define termios undefine_termios
#define sgttyb undefine_sgttyb
#define tchars undefine_tchars
#define ltchars undefine_ltchars

#include <asm/termios.h>
#include <asm/ioctls.h>

//ppc has c_ispeed/c_ospeed in termios and termios2 doesn't exist
#if defined(__powerpc__)
#define termios2 termios
#define TCGETS2 TCGETS
#define TCSETS2 TCSETS
#endif

#endif /* __linux__ */

#ifdef __APPLE__

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <IOKit/serial/ioss.h>

#endif /* __APPLE__ */

/*-------------------------------------------------------------------------*\
|  Serial Port Enums                                                        |
\*-------------------------------------------------------------------------*/
typedef unsigned int serial_port_parity;
enum
{
    SERIAL_PORT_PARITY_NONE = 0,    /* No parity                           */
    SERIAL_PORT_PARITY_ODD  = 1,    /* Odd parity                          */
    SERIAL_PORT_PARITY_EVEN = 2,    /* Even parity                         */
};

typedef unsigned int serial_port_size;
enum
{
    SERIAL_PORT_SIZE_8      = 0,    /* 8 bits per byte                     */
    SERIAL_PORT_SIZE_7      = 1,    /* 7 bits per byte                     */
    SERIAL_PORT_SIZE_6      = 2,    /* 6 bits per byte                     */
    SERIAL_PORT_SIZE_5      = 3,    /* 5 bits per byte                     */
};

typedef unsigned int serial_port_stop_bits;
enum
{
    SERIAL_PORT_STOP_BITS_1 = 0,    /* 1 stop bit                          */
    SERIAL_PORT_STOP_BITS_2 = 1,    /* 2 stop bits                         */
};

/*-------------------------------------------------------------------------*\
|  Serial Port Class                                                        |
|    The reason for this class is that serial ports are treated differently |
|    on Windows and Linux.  By creating a class, those differences can be   |
|    made invisible to the program and make cross-platform usage easy       |
\*-------------------------------------------------------------------------*/
class serial_port
{
public:
    static std::vector<std::string> getSerialPorts();

    serial_port();
    serial_port(const char * name, unsigned int baud);
    serial_port(const char *            name,
                unsigned int            baud,
                serial_port_parity      parity,
                serial_port_size        size,
                serial_port_stop_bits   stop_bits,
                bool                    flow_control);

    ~serial_port();

    bool serial_open();
    bool serial_open(const char* name);
    bool serial_open(const char* name, unsigned int baud);

    void serial_close();

    void serial_set_baud(unsigned int baud);
    int serial_get_baud();

    int serial_read(char * buffer, int length);

    int serial_write(char * buffer, int length);

    void serial_flush_rx();
    void serial_flush_tx();
    void serial_break();

    void serial_set_dtr(bool dtr);
    void serial_set_rts(bool rts);

    int serial_available();

private:
    char                    port_name[1024];
    unsigned int            baud_rate;
    serial_port_parity      parity;
    serial_port_size        size;
    serial_port_stop_bits   stop_bits;
    bool                    flow_control;

#ifdef _WIN32
    HANDLE file_descriptor;
    DCB dcb;
#else
    int file_descriptor;
#endif
};

#endif
