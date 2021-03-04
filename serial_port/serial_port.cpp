/*---------------------------------------------------------*\
|  Cross Platform Serial COM Library for Windows and Linux  |
|  This library provides access to serial ports with a      |
|  common API for both Windows and Linux systems.  It       |
|  features read and write as well as tx/rx buffer flush.   |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 1/21/2013        |
\*---------------------------------------------------------*/

#include "serial_port.h"

/*---------------------------------------------------------*\
|  serial_port (constructor)                                |
|    The default constructor does not initialize the serial |
|    port                                                   |
\*---------------------------------------------------------*/
serial_port::serial_port()
{
    /*-----------------------------------------------------*\
    | Set a default baud rate                               |
    \*-----------------------------------------------------*/
    baud_rate = 9600;
}

/*---------------------------------------------------------*\
|  serial_port (constructor)                                |
|    When created with port information, the constructor    |
|    will automatically open port <name> at baud rate <baud>|
\*---------------------------------------------------------*/
serial_port::serial_port(const char * name, unsigned int baud)
{
    serial_open(name, baud);
}

/*---------------------------------------------------------*\
|  ~serial_port (destructor)                                |
|    Closes the port before destroying the object           |
\*---------------------------------------------------------*/
serial_port::~serial_port()
{
    serial_close();
}

/*---------------------------------------------------------*\
|  serial_open                                              |
|    Opens the serial port using stored information         |
|    Sets the baud rate to the stored baud rate             |
|    8 data bits, no parity, one stop bit                   |
\*---------------------------------------------------------*/
bool serial_port::serial_open()
{
    /*-----------------------------------------------------*\
    | Windows-specific code path for serial port opening    |
    \*-----------------------------------------------------*/
#ifdef _WIN32
    file_descriptor = CreateFile(port_name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if((int)file_descriptor < 0)
    {
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
#endif

    /*-----------------------------------------------------*\
    | Linux-specific code path for serial port opening      |
    \*-----------------------------------------------------*/
#ifdef __linux__
    file_descriptor = open(port_name, O_RDWR | O_NOCTTY | O_NDELAY);

    if(file_descriptor < 0)
    {
        return false;
    }

    struct termios2 options;
    ioctl(file_descriptor, TCGETS2, &options);
    options.c_cflag &= ~CBAUD;
    options.c_cflag |= BOTHER;
    options.c_lflag &= ~ICANON;
    options.c_lflag &= ~ECHO;                                               // Disable echo
    options.c_lflag &= ~ECHOE;                                              // Disable erasure
    options.c_lflag &= ~ECHONL;                                             // Disable new-line echo
    options.c_lflag &= ~ISIG;                                               // Disable interpretation of INTR, QUIT and SUSP
    options.c_iflag &= ~(IXON | IXOFF | IXANY);                             // Turn off s/w flow ctrl
    options.c_ispeed = baud_rate;
    options.c_ospeed = baud_rate;
    options.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);    // Disable any special handling of received bytes
    ioctl(file_descriptor, TCSETS2, &options);
#endif

    /*-----------------------------------------------------*\
    | MacOS-specific code path for serial port opening      |
    \*-----------------------------------------------------*/
#ifdef __APPLE__
    file_descriptor = open(port_name, O_RDWR | O_NOCTTY | O_NDELAY);

    if(file_descriptor < 0)
    {
        return false;
    }

    struct termios options;
    tcgetattr(file_descriptor, &options);
    switch(baud_rate)
    {
        case 9600:
            cfsetispeed(&options, B9600);
            cfsetospeed(&options, B9600);
            break;
        case 19200:
            cfsetispeed(&options, B19200);
            cfsetospeed(&options, B19200);
            break;
        case 115200:
            cfsetispeed(&options, B115200);
            cfsetospeed(&options, B115200);
            break;
        case 38400:
            cfsetispeed(&options, B38400);
            cfsetospeed(&options, B38400);
            break;
        case 57600:
            cfsetispeed(&options, B57600);
            cfsetospeed(&options, B57600);
            break;
        default:
            cfsetispeed(&options, B9600);
            cfsetospeed(&options, B9600);
            break;
    }

    /*-----------------------------------------------------*\
    | Configure other settings                              |
    \*-----------------------------------------------------*/
    options.c_iflag &= ~(INLCR | ICRNL);
    options.c_iflag |= IGNPAR | IGNBRK;
    options.c_oflag &= ~(OPOST | ONLCR | OCRNL);
    options.c_cflag &= ~(PARENB | PARODD | CSTOPB | CSIZE | CRTSCTS);
    options.c_cflag |= CLOCAL | CREAD | CS8;
    options.c_lflag &= ~(ICANON | ISIG | ECHO);
    options.c_cc[VTIME] = 1;
    options.c_cc[VMIN]  = 0;

    if(tcsetattr(file_descriptor, TCSANOW, &options) < 0)
    {
        close(file_descriptor);
        return false;
    }
#endif

    /*-----------------------------------------------------*\
    | Return true if successful                             |
    \*-----------------------------------------------------*/
    return true;
}

/*---------------------------------------------------------*\
|  serial_open                                              |
|    Opens the serial port <name> without changing stored   |
|    baud rate                                              |
\*---------------------------------------------------------*/
bool serial_port::serial_open(const char * name)
{
    return serial_open(name, baud_rate);
}

/*---------------------------------------------------------*\
|  serial_open                                              |
|    Opens the serial port <name> at baud rate <baud>       |
\*---------------------------------------------------------*/
bool serial_port::serial_open(const char* name, unsigned int baud)
{
    strcpy(port_name, name);
    baud_rate = baud;
    return serial_open();
}

/*---------------------------------------------------------*\
|  serial_close                                             |
|    Closes the serial port                                 |
\*---------------------------------------------------------*/
void serial_port::serial_close()
{
    /*-----------------------------------------------------*\
    | Windows-specific code path for serial close           |
    \*-----------------------------------------------------*/
#ifdef _WIN32

#endif

    /*-----------------------------------------------------*\
    | Linux-specific code path for serial close             |
    \*-----------------------------------------------------*/
#ifdef __linux__
    close(file_descriptor);
#endif

    /*-----------------------------------------------------*\
    | MacOS-specific code path for serial close             |
    \*-----------------------------------------------------*/
#ifdef __APPLE__
    close(file_descriptor);
#endif
}

/*---------------------------------------------------------*\
|  serial_read                                              |
|    Reads <length> bytes from the serial port into <buffer>|
|    Returns the number of bytes actually read              |
|    If less than <length> bytes are available, it will read|
|    all available bytes                                    |
\*---------------------------------------------------------*/
int serial_port::serial_read(char * buffer, int length)
{
    /*-----------------------------------------------------*\
    | Windows-specific code path for serial read            |
    \*-----------------------------------------------------*/
#ifdef _WIN32
    DWORD bytesread;
    ReadFile(file_descriptor, buffer, length, &bytesread, NULL);
    return bytesread;
#endif

    /*-----------------------------------------------------*\
    | Linux-specific code path for serial read              |
    \*-----------------------------------------------------*/
#ifdef __linux__
    int bytesread;
    bytesread = read(file_descriptor, buffer, length);
    return bytesread;
#endif

    /*-----------------------------------------------------*\
    | MacOS-specific code path for serial read              |
    \*-----------------------------------------------------*/
#ifdef __APPLE__
    int bytesread;
    bytesread = read(file_descriptor, buffer, length);
    return bytesread;
#endif

    /*-----------------------------------------------------*\
    | Return 0 on unsupported platforms                     |
    \*-----------------------------------------------------*/
    return 0;
}

/*---------------------------------------------------------*\
|  serial_write                                             |
|    Writes <length> bytes to the serial port from <buffer> |
|    Returns the number of bytes actually written           |
|    Does not check for null-termination, so if <length> is |
|    greater than the number of bytes in <buffer>, it will  |
|    read past <buffer> and may cause a segfault            |
\*---------------------------------------------------------*/
int serial_port::serial_write(char * buffer, int length)
{
    /*-----------------------------------------------------*\
    | Windows-specific code path for serial write           |
    \*-----------------------------------------------------*/
#ifdef _WIN32
    DWORD byteswritten;
    WriteFile(file_descriptor, buffer, length, &byteswritten, NULL);
    return byteswritten;
#endif

    /*-----------------------------------------------------*\
    | Linux-specific code path for serial write             |
    \*-----------------------------------------------------*/
#ifdef __linux__
    int byteswritten;
    tcdrain(file_descriptor);
    byteswritten = write(file_descriptor, buffer, length);
    tcdrain(file_descriptor);
    return byteswritten;
#endif

    /*-----------------------------------------------------*\
    | MacOS-specific code path for serial write             |
    \*-----------------------------------------------------*/
#ifdef __APPLE__
    int byteswritten;
    tcdrain(file_descriptor);
    byteswritten = write(file_descriptor, buffer, length);
    tcdrain(file_descriptor);
    return byteswritten;
#endif

    /*-----------------------------------------------------*\
    | Return 0 on unsupported platforms                     |
    \*-----------------------------------------------------*/
    return 0;
}

/*---------------------------------------------------------*\
|  serial_flush                                             |
\*---------------------------------------------------------*/
void serial_port::serial_flush_rx()
{
#ifdef _WIN32
    PurgeComm(file_descriptor, PURGE_RXABORT | PURGE_RXCLEAR);
#endif

#ifdef __linux__
    tcflush(file_descriptor, TCIFLUSH);
#endif

#ifdef __APPLE__
    tcflush(file_descriptor, TCIFLUSH);
#endif
}

/*---------------------------------------------------------*\
|  serial_flush_tx                                          |
\*---------------------------------------------------------*/
void serial_port::serial_flush_tx()
{
#ifdef _WIN32
    PurgeComm(file_descriptor, PURGE_TXABORT | PURGE_TXCLEAR);
#endif

#ifdef __linux__
    tcflush(file_descriptor, TCOFLUSH);
#endif

#ifdef __APPLE__
    tcflush(file_descriptor, TCOFLUSH);
#endif
}
