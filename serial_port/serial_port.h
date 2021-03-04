/*---------------------------------------------------------*\
|  Cross Platform Serial COM Library for Windows and Linux  |
|  This library provides access to serial ports with a      |
|  common API for both Windows and Linux systems.  It       |
|  features read and write as well as tx/rx buffer flush.   |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 1/21/2013        |
\*---------------------------------------------------------*/

#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <string.h>
#include <stdio.h>

#ifdef _WIN32

#include <windows.h>

#endif /* _WIN32 */

#ifdef __linux__

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
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

#endif /* __APPLE__ */

/*-------------------------------------------------------------------------*\
|  Serial Port Class                                                        |
|    The reason for this class is that serial ports are treated differently |
|    on Windows and Linux.  By creating a class, those differences can be   |
|    made invisible to the program and make cross-platform usage easy       |
\*-------------------------------------------------------------------------*/
class serial_port
{
public:
    serial_port();
    serial_port(const char * name, unsigned int baud);

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

    int serial_available();

private:
    char port_name[1024];
    unsigned int baud_rate;

#ifdef _WIN32
    HANDLE file_descriptor;
    DCB dcb;
#else
    int file_descriptor;
#endif
};

#endif
