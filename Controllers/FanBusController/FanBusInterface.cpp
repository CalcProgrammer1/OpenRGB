/*---------------------------------------------------------*\
| FanBusInterface.cpp                                       |
|                                                           |
|   Interface for FanBus devices                            |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      12 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <chrono>
#include <thread>
#include "FanBusInterface.h"

using namespace std::chrono_literals;

FanBusInterface::FanBusInterface(const char* portname)
{
    port_name  = portname;
    serialport = new serial_port(portname, 38400);

    /*-----------------------------------------------------*\
    | Flush any data in the receive queue                   |
    \*-----------------------------------------------------*/
    unsigned char read_buf[6];

    while(serialport->serial_read((char *)read_buf, 6) > 0)
    {

    }

    read_buf[0] = 0xFF;

    serialport->serial_write((char *)read_buf, 1);

    std::this_thread::sleep_for(10ms);

    int test = serialport->serial_read((char *)read_buf, 1);

    if(test > 0)
    {
        half_duplex = true;
    }
    else
    {
        half_duplex = false;
    }
}

FanBusInterface::~FanBusInterface()
{
    serialport->serial_close();
    delete serialport;
}

std::string FanBusInterface::GetPort()
{
    return(port_name);
}

int FanBusInterface::read
    (
        unsigned char dev_addr,
        unsigned char int_addr
    )
{
    unsigned char fanbus_msg[] = { 0x01, int_addr, dev_addr, 0x00, 0xFF };

    serialport->serial_write((char *)fanbus_msg, 5);

    std::this_thread::sleep_for(10ms);

    char read_buf[6];

    if(half_duplex)
    {
        if(serialport->serial_read(read_buf, 6) == 6)
        {
            return(read_buf[5]);
        }
        else
        {
            return(-1);
        }
    }
    else
    {
        if(serialport->serial_read(read_buf, 1) == 1)
        {
            return(read_buf[0]);
        }
        else
        {
            return(-1);
        }
    }
}

int FanBusInterface::write
    (
        unsigned char dev_addr,
        unsigned char int_addr,
        unsigned char val
    )
{
    unsigned char fanbus_msg[] = { 0x00, int_addr, dev_addr, val, 0xFF };

    return(serialport->serial_write((char *)fanbus_msg, 5));
}

void FanBusInterface::write_queue
    (
        unsigned char dev_addr,
        unsigned char int_addr,
        unsigned char val
    )
{
    unsigned char fanbus_msg[] = { 0x00, int_addr, dev_addr, val, 0xFF };

    for(unsigned int i = 0; i < sizeof(fanbus_msg); i++)
    {
        fanbus_msg_queued.push_back(fanbus_msg[i]);
    }
}

int FanBusInterface::process_queue()
{
    int return_val = serialport->serial_write((char *)&fanbus_msg_queued[0], (int)fanbus_msg_queued.size());

    fanbus_msg_queued.clear();

    return(return_val);
}

std::vector<unsigned char> FanBusInterface::DetectControllers()
{
    std::vector<unsigned char> detected_controllers;

    for(unsigned int dev_addr = 0x03; dev_addr < 0xFF; dev_addr++)
    {
        if(read(dev_addr, 0x00) >= 0)
        {
            detected_controllers.push_back(dev_addr);
        }
    }

    return(detected_controllers);
}
