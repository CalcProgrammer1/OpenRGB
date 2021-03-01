/*---------------------------------------------------------*\
|  FanBus Interface                                         |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 1/12/2021        |
\*---------------------------------------------------------*/

#include "serial_port.h"

#include <string>
#include <vector>

class FanBusInterface
{
public:
    FanBusInterface(const char* portname);
    ~FanBusInterface();

    std::vector<unsigned char> DetectControllers();

    std::string GetPort();

    int         read
                (
                    unsigned char dev_addr,
                    unsigned char int_addr
                );

    int         write
                (
                    unsigned char dev_addr,
                    unsigned char int_addr,
                    unsigned char val
                );

    void        write_queue
                (
                    unsigned char dev_addr,
                    unsigned char int_addr,
                    unsigned char val
                );

    int         process_queue();
    
private:
    serial_port *   serialport;
    std::string     port_name;
    bool            half_duplex;

    std::vector<unsigned char>  fanbus_msg_queued;
};
