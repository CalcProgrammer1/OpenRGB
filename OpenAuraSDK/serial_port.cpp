/*---------------------------------------------------------*\
|  Cross Platform Serial COM Library for Windows and Linux  |
|  This library provides access to serial ports with a      |
|  common API for both Windows and Linux systems.  It       |
|  features read and write as well as tx/rx buffer flush.   |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 1/21/2013        |
\*---------------------------------------------------------*/

#include "serial_port.h"

//serial_port (constructor)
//	The default constructor does not initialize the serial port
serial_port::serial_port()
{
    //Set a default baud rate
    baud_rate = 9600;
}

//serial_port (constructor)
//	When created with port information, the constructor
//	will automatically open port <name> at baud rate <baud>
serial_port::serial_port(const char * name, unsigned int baud)
{
    serial_open(name, baud);
}

//~serial_port (destructor)
//	Closes the port before destroying the object
serial_port::~serial_port()
{
    serial_close();
}

//open
//	Opens the serial port using stored information
//	Sets the baud rate to the stored baud rate
//	8 data bits, no parity, one stop bit
bool serial_port::serial_open()
{
//    printf("SerialPort: Opening serial port %s at baud rate %d.\n", port_name, baud_rate);

    #ifdef WIN32
    file_descriptor = CreateFile(port_name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if((int)file_descriptor < 0)
    {
//      printf("SerialPort: Port %s could not be opened: %d.\n", port_name, file_descriptor);
        return false;
    }

    SetupComm(file_descriptor, 1, 128);
    GetCommState(file_descriptor, &dcb);

    dcb.BaudRate = baud_rate;                   //Set baud rate
    dcb.ByteSize = 8;                           //8 data bits
    dcb.Parity = NOPARITY;                      //Parity = none
    dcb.StopBits = ONESTOPBIT;                  //One stop bit
    dcb.fAbortOnError = TRUE;                   //Abort on error
    dcb.fOutX = FALSE;                          //XON/XOFF off for transmit
    dcb.fInX = FALSE;                           //XON/XOFF off for receive
    dcb.fOutxCtsFlow = FALSE;                   //Turn off CTS flow control
    dcb.fRtsControl = RTS_CONTROL_DISABLE;      //Options DISABLE, ENABLE, HANDSHAKE
    dcb.fOutxDsrFlow = FALSE;                   //Turn off DSR flow control
    dcb.fDtrControl = DTR_CONTROL_DISABLE;      //Disable DTR control

    SetCommState(file_descriptor, &dcb);

    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant=50;
    timeouts.ReadTotalTimeoutMultiplier=10;
    timeouts.WriteTotalTimeoutConstant=50;
    timeouts.WriteTotalTimeoutMultiplier=10;
    SetCommTimeouts(file_descriptor, &timeouts);

    #else

    file_descriptor = open(port_name, O_RDWR | O_NOCTTY | O_NDELAY);

    if(file_descriptor < 0)
    {
//      printf("SerialPort: Port %s could not be opened: %d.\n", port_name, file_descriptor);
        return false;
    }

    struct termios2 options;
    ioctl(file_descriptor, TCGETS2, &options);
    options.c_cflag &= ~CBAUD;
    options.c_cflag |= BOTHER;
    options.c_ispeed = baud_rate;
    options.c_ospeed = baud_rate;
    ioctl(file_descriptor, TCSETS2, &options);

    //serial_struct ss;
    //int closestSpeed;
    //ioctl(file_descriptor, TIOCGSERIAL, &ss);
    //ss.flags = (ss.flags & ~ASYNC_SPD_MASK) | ASYNC_SPD_CUST | ASYNCB_LOW_LATENCY;
    //ss.custom_divisor = (ss.baud_base + (baud_rate / 2)) / baud_rate;
    //if(ss.custom_divisor == 0)
    //{
    //    closestSpeed = baud_rate;
    //}
    //else
    //{
    //    closestSpeed = ss.baud_base / ss.custom_divisor;
    //}

    //if((float)closestSpeed < ((float)baud_rate * (98.0f/100.0f)) || (float)closestSpeed > ((float)baud_rate * (102.0f/100.0f)))
    //{
//      printf("SerialPort: Cannot set %s to %d.  Closest possible speed is %d.\n", port_name, baud_rate, closestSpeed);
    //}
    //else
    //{
//      printf("SerialPort: %s speed set to %d.\n", port_name, baud_rate);
    //}

    //fcntl(file_descriptor, F_SETFL, 0);
    #endif

//  printf("SerialPort: Serial port %s opened successfully.\n", port_name);
    return true;
}

//open
//	Opens the serial port <name> without changing stored baud rate
bool serial_port::serial_open(const char * name)
{
    return serial_open(name, baud_rate);
}

//open
//	Opens the serial port <name> at baud rate <baud>
bool serial_port::serial_open(const char* name, unsigned int baud)
{
    strcpy(port_name, name);
    baud_rate = baud;
    return serial_open();
}

//close
//	Closes the serial port
void serial_port::serial_close()
{
//  printf("SerialPort: Closing port %s.\n", port_name);
    #ifdef WIN32

    #else

    close(file_descriptor);
    #endif
}

// read
//	Reads <length> bytes from the serial port into <buffer>
//	Returns the number of bytes actually read
//	If less than <length> bytes are available, it will read all
//	available bytes
int serial_port::serial_read(char * buffer, int length)
{
    #ifdef WIN32
    DWORD bytesread;
    ReadFile(file_descriptor, buffer, length, &bytesread, NULL);

    #else

    int bytesread;
    bytesread = read(file_descriptor, buffer, length);
    #endif

    //printf("SerialPort: Read %d bytes on port %s.\n", bytesread, port_name);
    return bytesread;
}

//write
//	Writes <length> bytes to the serial port from <buffer>
//	Returns the number of bytes actually written
//	Does not check for null-termination, so if <length> is
//	greater than the number of bytes in <buffer>, it will read
//	past <buffer> and may cause a segfault
int serial_port::serial_write(char * buffer, int length)
{
    #ifdef WIN32
    DWORD byteswritten;
    WriteFile(file_descriptor, buffer, length, &byteswritten, NULL);

    #else

    int byteswritten;
    byteswritten = write(file_descriptor, buffer, length);
    #endif

    //printf("SerialPort: Wrote %d bytes on port %s.\n", byteswritten, port_name);
    return byteswritten;
}

//flush
void serial_port::serial_flush_rx()
{
    #ifdef WIN32
    PurgeComm(file_descriptor, PURGE_RXABORT | PURGE_RXCLEAR);

    #else

    tcflush(file_descriptor, TCIFLUSH);
    #endif
}

void serial_port::serial_flush_tx()
{

    #ifdef WIN32
    PurgeComm(file_descriptor, PURGE_TXABORT | PURGE_TXCLEAR);

    #else

    tcflush(file_descriptor, TCOFLUSH);
    #endif
}
