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

#ifdef WIN32
#include <windows.h>

#else

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

//winsize, termio, and termios structs are redefined in
//asm/termios.h, to prevent compiler errors from multply
//defining them, use a #define to rename them -
//essentially to undef them before they are redefined
#define winsize undefine_winsize
#define termio undefine_termio
#define termios undefine_termios

#include <asm/termios.h>
#include <asm-generic/ioctls.h>


#endif


//Serial Port Class
//The reason for this class is that serial ports are treated differently
//on Windows and Linux.  By creating a class, those differences can be
//made invisible to the program and make cross-platform usage easy

class serial_port
{
    public:
        serial_port();
        serial_port(const char * name, unsigned int baud);

        ~serial_port();

        //Function to open the port
        bool serial_open();
        bool serial_open(const char* name);
        bool serial_open(const char* name, unsigned int baud);

        //Function to close the port
        void serial_close();

        //Functions for controlling baud rate
        void serial_set_baud(unsigned int baud);
        int serial_get_baud();

        //Function to read data from the port buffer
        int serial_read(char * buffer, int length);

        //Function to write data to the serial port
        int serial_write(char * buffer, int length);

        //Functions to flush the serial port rx and tx buffers
        void serial_flush_rx();
        void serial_flush_tx();

        //Function to list the number of available bytes
        int serial_available();

    private:
        char port_name[1024];
        unsigned int baud_rate;

        #ifdef WIN32
        HANDLE file_descriptor;
        DCB dcb;

        #else

        int file_descriptor;
        #endif
};

#endif
