/*---------------------------------------------------------*\
|  BlinkyTapeController.cpp                                 |
|                                                           |
|  BlinkyTape controller interface                          |
|                                                           |
|  Matt Mets (matt@blinkinlabs.com), 07/01/2021             |
\*---------------------------------------------------------*/

#include "BlinkyTapeController.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#ifndef WIN32
#define LPSTR           char *
#define strtok_s        strtok_r
#endif

BlinkyTapeController::BlinkyTapeController()
{
}

BlinkyTapeController::~BlinkyTapeController()
{
    if(serialport != nullptr)
    {
        serialport->serial_close();
        delete serialport;
    }
}

void BlinkyTapeController::Initialize(const std::string &portname)
{
    port_name   = portname;

    serialport  = new serial_port();

    if(!serialport->serial_open(port_name.c_str(), 115200))
    {
        delete serialport;
        serialport = nullptr;
    }
}

std::string BlinkyTapeController::GetLocation()
{
    if(serialport == nullptr)
    {
        return("");
    }

    return("COM: " + port_name);
}

char* BlinkyTapeController::GetLEDString()
{
    return(led_string);
}

void BlinkyTapeController::SetLEDs(std::vector<RGBColor> colors)
{
    if(serialport == nullptr)
    {
        return;
    }

    /*-------------------------------------------------------------*\
    | BlinkyTape Protocol                                           |
    |                                                               |
    |   Packet size: Number of data bytes + 1                       |
    |                                                               |
    |   0-n: Data Byte (0-254)                                      |
    |   n+1: Packet End Byte (0xFF)                                 |
    \*-------------------------------------------------------------*/
    const unsigned int payload_size     = (colors.size() * 3);
    const unsigned int packet_size      = payload_size + 1;

    std::vector<unsigned char> serial_buf(packet_size);

    /*-------------------------------------------------------------*\
    | Set up end byte                                               |
    \*-------------------------------------------------------------*/
    serial_buf[packet_size - 1] = 0xFF;

    /*-------------------------------------------------------------*\
    | Copy in color data in RGB order                               |
    \*-------------------------------------------------------------*/
    for(unsigned int color_idx = 0; color_idx < colors.size(); color_idx++)
    {
        const unsigned int color_offset = color_idx * 3;

        serial_buf[0x00 + color_offset] = std::min((unsigned int)254, RGBGetRValue(colors[color_idx]));
        serial_buf[0x01 + color_offset] = std::min((unsigned int)254, RGBGetGValue(colors[color_idx]));
        serial_buf[0x02 + color_offset] = std::min((unsigned int)254, RGBGetBValue(colors[color_idx]));
    }

    /*-------------------------------------------------------------*\
    | Send the packet                                               |
    \*-------------------------------------------------------------*/
    serialport->serial_write((char *)serial_buf.data(), packet_size);
}

