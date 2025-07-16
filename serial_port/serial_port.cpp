/*---------------------------------------------------------*\
| serial_port.cpp                                           |
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

#include <algorithm>
#include "filesystem.h"
#include "serial_port.h"

#ifdef __APPLE__
#include <regex>
#endif

/*---------------------------------------------------------*\
|  getSerialPorts                                           |
|                                                           |
|   Returns the list of available serial ports in the       |
|   system                                                  |
\*---------------------------------------------------------*/
std::vector<std::string> serial_port::getSerialPorts()
{
    /*-----------------------------------------------------------------------------------*\
    | Ported from https://github.com/nkinar/GetComPortList/blob/master/GetComPortList.cpp |
    \*-----------------------------------------------------------------------------------*/
    std::vector<std::string> port_list;
#if defined (_WIN32) || defined( _WIN64)
    const uint32_t      CHAR_NUM    = 1024;
    const uint32_t      MAX_PORTS   = 255;
    const std::string   COM_STR     = "COM";

    char path[CHAR_NUM];

    for(uint32_t k = 0; k < MAX_PORTS; k++)
    {
        std::string port_name = COM_STR + std::to_string(k);

        DWORD test = QueryDosDevice(port_name.c_str(), path, CHAR_NUM);

        if(test == 0)
        {
            continue;
        }

        port_list.push_back(port_name);
    }
#endif
#if defined (__linux__)
    const std::string DEV_PATH = "/dev/serial/by-id";
    try
    {
        filesystem::path p(DEV_PATH);

        if(!filesystem::exists(DEV_PATH))
        {
            return port_list;
        }

        for(filesystem::directory_entry de: filesystem::directory_iterator(p))
        {
            if(filesystem::is_symlink(de.symlink_status()))
            {
                filesystem::path symlink_points_at = filesystem::read_symlink(de);
                port_list.push_back(std::string("/dev/")+symlink_points_at.filename().c_str());
            }
        }
    }
    catch(const filesystem::filesystem_error &ex)
    {

    }
#endif
#if defined(__APPLE__)
    const std::string   DEV_PATH = "/dev";
    const std::regex    base_regex(R"(\/dev\/(tty|cu)\..*)");
    try
    {
        filesystem::path p(DEV_PATH);

        if(!filesystem::exists(DEV_PATH))
        {
            return port_list;
        }

        for(filesystem::directory_entry de: filesystem::directory_iterator(p))
        {
            filesystem::path    canonical_path  = filesystem::canonical(de);
            std::string         name            = canonical_path.generic_string();
            std::smatch         res;

            std::regex_search(name, res, base_regex);

            if(res.empty())
            {
                continue;
            }

            port_list.push_back(canonical_path.generic_string());
        }
    }
    catch(const filesystem::filesystem_error &ex)
    {

    }
#endif
    std::sort(port_list.begin(), port_list.end());

    return port_list;
}

/*---------------------------------------------------------*\
|  serial_port (constructor)                                |
|    The default constructor does not initialize the serial |
|    port                                                   |
\*---------------------------------------------------------*/
serial_port::serial_port()
{
    /*-----------------------------------------------------*\
    | Set default port configuration but do not open        |
    \*-----------------------------------------------------*/
    this->baud_rate     = 9600;
    this->parity        = SERIAL_PORT_PARITY_NONE;
    this->size          = SERIAL_PORT_SIZE_8;
    this->stop_bits     = SERIAL_PORT_STOP_BITS_1;
    this->flow_control  = false;
}

/*---------------------------------------------------------*\
|  serial_port (constructor)                                |
|    When created with port information, the constructor    |
|    will automatically open port <name> at baud rate <baud>|
\*---------------------------------------------------------*/
serial_port::serial_port(const char * name, unsigned int baud)
{
    /*-----------------------------------------------------*\
    | Set default port configuration and open               |
    \*-----------------------------------------------------*/
    this->baud_rate     = baud;
    this->parity        = SERIAL_PORT_PARITY_NONE;
    this->size          = SERIAL_PORT_SIZE_8;
    this->stop_bits     = SERIAL_PORT_STOP_BITS_1;
    this->flow_control  = false;

    serial_open(name);
}

/*---------------------------------------------------------*\
|  serial_port (constructor)                                |
|    When created with port information, the constructor    |
|    will automatically open port <name> at baud rate <baud>|
|    with the given port configuration                      |
\*---------------------------------------------------------*/
serial_port::serial_port
    (
    const char *            name,
    unsigned int            baud,
    serial_port_parity      parity,
    serial_port_size        size,
    serial_port_stop_bits   stop_bits,
    bool                    flow_control
    )
{
    /*-----------------------------------------------------*\
    | Set default port configuration and open               |
    \*-----------------------------------------------------*/
    this->baud_rate     = baud;
    this->parity        = parity;
    this->size          = size;
    this->stop_bits     = stop_bits;
    this->flow_control  = flow_control;

    serial_open(name);
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
    // On Windows, ports above 9 need to have "\\.\" prepended to their path. For ports below 9, this is optional.
    // https://support.microsoft.com/en-us/topic/howto-specify-serial-ports-larger-than-com9-db9078a5-b7b6-bf00-240f-f749ebfd913e
    char full_path[100];
    snprintf(full_path, sizeof(full_path), "\\\\.\\%s", port_name);

    /*-----------------------------------------*\
    | Open the port read/write                  |
    \*-----------------------------------------*/
    file_descriptor = CreateFile(full_path, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if(file_descriptor == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    /*-----------------------------------------*\
    | Get the port configuration options        |
    \*-----------------------------------------*/
    SetupComm(file_descriptor, 1, 128);
    GetCommState(file_descriptor, &dcb);

    /*-----------------------------------------*\
    | Configure baud rate                       |
    \*-----------------------------------------*/
    dcb.BaudRate = baud_rate;

    /*-----------------------------------------*\
    | Configure parity                          |
    \*-----------------------------------------*/
    switch(parity)
    {
        case SERIAL_PORT_PARITY_NONE:
            dcb.Parity = NOPARITY;
            break;

        case SERIAL_PORT_PARITY_ODD:
            dcb.Parity = ODDPARITY;
            break;

        case SERIAL_PORT_PARITY_EVEN:
            dcb.Parity = EVENPARITY;
            break;
    }

    /*-----------------------------------------*\
    | Configure size                            |
    \*-----------------------------------------*/
    switch(size)
    {
        case SERIAL_PORT_SIZE_8:
            dcb.ByteSize = 8;
            break;

        case SERIAL_PORT_SIZE_7:
            dcb.ByteSize = 7;
            break;

        case SERIAL_PORT_SIZE_6:
            dcb.ByteSize = 6;
            break;

        case SERIAL_PORT_SIZE_5:
            dcb.ByteSize = 5;
            break;
    }

    /*-----------------------------------------*\
    | Configure stop bits                       |
    \*-----------------------------------------*/
    if(stop_bits == SERIAL_PORT_STOP_BITS_2)
    {
        dcb.StopBits = TWOSTOPBITS;
    }
    else
    {
        dcb.StopBits = ONESTOPBIT;
    }

    /*-----------------------------------------*\
    | Configure flow control                    |
    \*-----------------------------------------*/
    if(flow_control)
    {
        dcb.fRtsControl = RTS_CONTROL_ENABLE;
    }
    else
    {
        dcb.fRtsControl = RTS_CONTROL_DISABLE;
    }

    /*-----------------------------------------*\
    | Configure additional parameters           |
    \*-----------------------------------------*/
    dcb.fAbortOnError = FALSE;                  //Abort on error
    dcb.fOutX = FALSE;                          //XON/XOFF off for transmit
    dcb.fInX = FALSE;                           //XON/XOFF off for receive
    dcb.fOutxCtsFlow = FALSE;                   //Turn off CTS flow control
    dcb.fOutxDsrFlow = FALSE;                   //Turn off DSR flow control
    dcb.fDtrControl = DTR_CONTROL_DISABLE;      //Disable DTR control

    /*-----------------------------------------*\
    | Set the port configuration options        |
    \*-----------------------------------------*/
    SetCommState(file_descriptor, &dcb);

    /*-----------------------------------------*\
    | Set the port timeouts                     |
    \*-----------------------------------------*/
    COMMTIMEOUTS timeouts                   = {0};
    timeouts.ReadIntervalTimeout            = 50;
    timeouts.ReadTotalTimeoutConstant       = 50;
    timeouts.ReadTotalTimeoutMultiplier     = 10;
    timeouts.WriteTotalTimeoutConstant      = 50;
    timeouts.WriteTotalTimeoutMultiplier    = 10;

    SetCommTimeouts(file_descriptor, &timeouts);
#endif

    /*-----------------------------------------------------*\
    | Linux-specific code path for serial port opening      |
    \*-----------------------------------------------------*/
#ifdef __linux__
    /*-----------------------------------------*\
    | Open the port read/write with no delay    |
    \*-----------------------------------------*/
    file_descriptor = open(port_name, O_RDWR | O_NOCTTY | O_NDELAY);

    if(file_descriptor < 0)
    {
        return false;
    }

    /*-----------------------------------------*\
    | Set an advisory lock on the port and      |
    | abort port setup if already locked        |
    \*-----------------------------------------*/
    if(flock(file_descriptor, LOCK_EX | LOCK_NB) < 0)
    {
        close(file_descriptor);
        return false;
    }

    /*-----------------------------------------*\
    | Get the port configuration options        |
    \*-----------------------------------------*/
    struct termios2 options;
    ioctl(file_descriptor, TCGETS2, &options);

    /*-----------------------------------------*\
    | Configure baud rate                       |
    \*-----------------------------------------*/
    options.c_cflag &= ~CBAUD;
    options.c_cflag |= CBAUDEX;

    options.c_ispeed = baud_rate;
    options.c_ospeed = baud_rate;

    /*-----------------------------------------*\
    | Configure parity                          |
    \*-----------------------------------------*/
    switch(parity)
    {
        case SERIAL_PORT_PARITY_NONE:
            options.c_cflag &= ~PARENB;
            options.c_cflag &= ~PARODD;
            break;

        case SERIAL_PORT_PARITY_ODD:
            options.c_cflag |= PARENB;
            options.c_cflag |= PARODD;
            break;

        case SERIAL_PORT_PARITY_EVEN:
            options.c_cflag |= PARENB;
            options.c_cflag &= ~PARODD;
            break;
    }

    /*-----------------------------------------*\
    | Configure size                            |
    \*-----------------------------------------*/
    options.c_cflag &= ~CSIZE;

    switch(size)
    {
        case SERIAL_PORT_SIZE_8:
            options.c_cflag |= CS8;
            break;

        case SERIAL_PORT_SIZE_7:
            options.c_cflag |= CS7;
            break;

        case SERIAL_PORT_SIZE_6:
            options.c_cflag |= CS6;
            break;

        case SERIAL_PORT_SIZE_5:
            options.c_cflag |= CS5;
            break;
    }

    /*-----------------------------------------*\
    | Configure stop bits                       |
    \*-----------------------------------------*/
    if(stop_bits == SERIAL_PORT_STOP_BITS_2)
    {
        options.c_cflag |= CSTOPB;
    }
    else
    {
        options.c_cflag &= ~CSTOPB;
    }

    /*-----------------------------------------*\
    | Configure flow control                    |
    \*-----------------------------------------*/
    if(flow_control)
    {
        options.c_cflag |= CRTSCTS;
    }
    else
    {
        options.c_cflag &= ~CRTSCTS;
    }

    /*-----------------------------------------*\
    | Configure additional parameters           |
    \*-----------------------------------------*/
    options.c_lflag &= ~ICANON;
    options.c_lflag &= ~ECHO;                                               // Disable echo
    options.c_lflag &= ~ECHOE;                                              // Disable erasure
    options.c_lflag &= ~ECHONL;                                             // Disable new-line echo
    options.c_lflag &= ~ISIG;                                               // Disable interpretation of INTR, QUIT and SUSP
    options.c_lflag &= ~IEXTEN;                                             // Disable input processing

    options.c_iflag &= ~(IXON | IXOFF | IXANY);                             // Turn off s/w flow ctrl
    options.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);    // Disable any special handling of received bytes

    options.c_oflag &= ~OPOST;                                              // Disable output processing;

    /*-----------------------------------------*\
    | Set the port configuration options        |
    \*-----------------------------------------*/
    ioctl(file_descriptor, TCSETS2, &options);
#endif

    /*-----------------------------------------------------*\
    | MacOS-specific code path for serial port opening      |
    \*-----------------------------------------------------*/
#ifdef __APPLE__
    /*-----------------------------------------*\
    | Open the port read/write with no delay    |
    \*-----------------------------------------*/
    file_descriptor = open(port_name, O_RDWR | O_NOCTTY | O_NDELAY);

    if(file_descriptor < 0)
    {
        return false;
    }

    /*-----------------------------------------*\
    | Get the port configuration options        |
    \*-----------------------------------------*/
    struct termios options;
    tcgetattr(file_descriptor, &options);

    /*-----------------------------------------*\
    | Configure parity                          |
    \*-----------------------------------------*/
    switch(parity)
    {
        case SERIAL_PORT_PARITY_NONE:
            options.c_cflag &= ~PARENB;
            options.c_cflag &= ~PARODD;
            break;

        case SERIAL_PORT_PARITY_ODD:
            options.c_cflag |= PARENB;
            options.c_cflag |= PARODD;
            break;

        case SERIAL_PORT_PARITY_EVEN:
            options.c_cflag |= PARENB;
            options.c_cflag &= ~PARODD;
            break;
    }

    /*-----------------------------------------*\
    | Configure size                            |
    \*-----------------------------------------*/
    options.c_cflag &= ~CSIZE;

    switch(size)
    {
        case SERIAL_PORT_SIZE_8:
            options.c_cflag |= CS8;
            break;

        case SERIAL_PORT_SIZE_7:
            options.c_cflag |= CS7;
            break;

        case SERIAL_PORT_SIZE_6:
            options.c_cflag |= CS6;
            break;

        case SERIAL_PORT_SIZE_5:
            options.c_cflag |= CS5;
            break;
    }

    /*-----------------------------------------*\
    | Configure stop bits                       |
    \*-----------------------------------------*/
    if(stop_bits == SERIAL_PORT_STOP_BITS_2)
    {
        options.c_cflag |= CSTOPB;
    }
    else
    {
        options.c_cflag &= ~CSTOPB;
    }

    /*-----------------------------------------*\
    | Configure flow control                    |
    \*-----------------------------------------*/
    if(flow_control)
    {
        options.c_cflag |= CRTSCTS;
    }
    else
    {
        options.c_cflag &= ~CRTSCTS;
    }

    /*-----------------------------------------*\
    | Configure additional parameters           |
    \*-----------------------------------------*/
    options.c_lflag &= ~(ICANON | IEXTEN | ISIG | ECHO);

    options.c_iflag &= ~(INLCR | ICRNL);
    options.c_iflag |= IGNPAR | IGNBRK;

    options.c_oflag &= ~(OPOST | ONLCR | OCRNL);

    options.c_cc[VTIME] = 1;
    options.c_cc[VMIN]  = 0;

    /*-----------------------------------------*\
    | Set the port configuration options        |
    \*-----------------------------------------*/
    if(tcsetattr(file_descriptor, TCSANOW, &options) < 0)
    {
        close(file_descriptor);
        return false;
    }

    /*-----------------------------------------*\
    | Configure baud rate                       |
    \*-----------------------------------------*/
    ioctl(file_descriptor, IOSSIOSPEED, &baud_rate);

    printf("Port opened fd %d", file_descriptor);
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
    snprintf(port_name,sizeof(port_name),"%s",name);
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
    CloseHandle(file_descriptor);
#endif

    /*-----------------------------------------------------*\
    | Linux-specific code path for serial close             |
    \*-----------------------------------------------------*/
#ifdef __linux__
    flock(file_descriptor, LOCK_UN | LOCK_NB);
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
    printf("serial write fd %d", file_descriptor);
    printf("tcdrain %d\r\n",tcdrain(file_descriptor));
    printf("write %d\r\n", byteswritten = write(file_descriptor, buffer, length));
    printf("tcdrain %d\r\n", tcdrain(file_descriptor));
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

/*---------------------------------------------------------*\
|  serial_break                                             |
\*---------------------------------------------------------*/
void serial_port::serial_break()
{
    /*-----------------------------------------------------*\
    | Windows-specific code path for serial break           |
    \*-----------------------------------------------------*/
#ifdef _WIN32
    SetCommBreak(file_descriptor);
    Sleep(1);
    ClearCommBreak(file_descriptor);
#endif

    /*-----------------------------------------------------*\
    | Linux-specific code path for serial break             |
    \*-----------------------------------------------------*/
#ifdef __linux__
    //Send break for at least 1 ms
    ioctl(file_descriptor, TIOCSBRK);
    usleep(1000);
    ioctl(file_descriptor, TIOCCBRK);
#endif

    /*-----------------------------------------------------*\
    | MacOS-specific code path for serial break             |
    \*-----------------------------------------------------*/
#ifdef __APPLE__
    //Send break for at least 1 ms
    ioctl(file_descriptor, TIOCSBRK);
    usleep(1000);
    ioctl(file_descriptor, TIOCCBRK);
#endif
}

void serial_port::serial_set_dtr(bool dtr)
{
    /*-----------------------------------------------------*\
    | Windows-specific code path for serial set DTR         |
    \*-----------------------------------------------------*/
#ifdef _WIN32
    if(dtr)
    {
        EscapeCommFunction(file_descriptor, SETDTR);
    }
    else
    {
        EscapeCommFunction(file_descriptor, CLRDTR);
    }
#endif

    /*-----------------------------------------------------*\
    | Linux-specific code path for serial set DTR           |
    \*-----------------------------------------------------*/
#ifdef __linux__
    const int DTRFLAG = TIOCM_DTR;
    if(dtr)
    {
        ioctl(file_descriptor, TIOCMBIS, &DTRFLAG);
    }
    else
    {
        ioctl(file_descriptor, TIOCMBIC, &DTRFLAG);
    }
#endif

    /*-----------------------------------------------------*\
    | MacOS-specific code path for serial set DTR           |
    \*-----------------------------------------------------*/
#ifdef __APPLE__
    const int DTRFLAG = TIOCM_DTR;
    if(dtr)
    {
        ioctl(file_descriptor, TIOCMBIS, &DTRFLAG);
    }
    else
    {
        ioctl(file_descriptor, TIOCMBIC, &DTRFLAG);
    }
#endif
}

void serial_port::serial_set_rts(bool rts)
{
    /*-----------------------------------------------------*\
    | Windows-specific code path for serial set RTS         |
    \*-----------------------------------------------------*/
#ifdef _WIN32
    if(rts)
    {
        EscapeCommFunction(file_descriptor, SETRTS);
    }
    else
    {
        EscapeCommFunction(file_descriptor, CLRRTS);
    }
#endif

    /*-----------------------------------------------------*\
    | Linux-specific code path for serial set RTS           |
    \*-----------------------------------------------------*/
#ifdef __linux__
    const int RTSFLAG = TIOCM_RTS;
    if(rts)
    {
        ioctl(file_descriptor, TIOCMBIS, &RTSFLAG);
    }
    else
    {
        ioctl(file_descriptor, TIOCMBIC, &RTSFLAG);
    }
#endif

    /*-----------------------------------------------------*\
    | MacOS-specific code path for serial set RTS           |
    \*-----------------------------------------------------*/
#ifdef __APPLE__
    const int RTSFLAG = TIOCM_RTS;
    if(rts)
    {
        ioctl(file_descriptor, TIOCMBIS, &RTSFLAG);
    }
    else
    {
        ioctl(file_descriptor, TIOCMBIC, &RTSFLAG);
    }
#endif
}
