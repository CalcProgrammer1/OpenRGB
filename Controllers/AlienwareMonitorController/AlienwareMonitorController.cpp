/*---------------------------------------------------------*\
| AlienwareMonitorController.cpp                            |
|                                                           |
|   Detector for Alienware monitors                         |
|                                                           |
|   Adam Honse (CalcProgrammer1)                08 May 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <chrono>
#include <cstring>
#include <thread>

#include "AlienwareMonitorController.h"

AlienwareMonitorController::AlienwareMonitorController(hid_device *dev_handle, const char *path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;

    Initialize();
}

AlienwareMonitorController::~AlienwareMonitorController()
{
    hid_close(dev);
}

std::string AlienwareMonitorController::GetLocation()
{
    return("HID: " + location);
}

std::string AlienwareMonitorController::GetName()
{
    return(name);
}

std::string AlienwareMonitorController::GetSerialString()
{
    return("");
}

void fillInChecksum(unsigned char *packet)
{
    unsigned char checksum = 110;

    for(unsigned int index = 5; index <= 13; index++)
    {
      checksum ^= packet[index];
    }

    packet[14] = checksum;
}

void AlienwareMonitorController::SendColor(unsigned char led_id, unsigned char r, unsigned char g, unsigned char b)
{
    unsigned char packet[65];

    memset(packet, 0xFF, sizeof(packet));

    packet[0] = 0x00;
    packet[1] = 0x92;
    packet[2] = 0x37;
    packet[3] = 0x0a;
    packet[4] = 0x00;
    packet[5] = 0x51;
    packet[6] = 0x87;
    packet[7] = 0xd0;
    packet[8] = 0x04;

    packet[9] = led_id;
    packet[10] = r;
    packet[11] = g;
    packet[12] = b;
    packet[13] = 0x64;

    fillInChecksum(packet);

    hid_write(dev, packet, sizeof(packet));

    /*-----------------------------------------------------*\
    | Delay 50 milliseconds                                 |
    \*-----------------------------------------------------*/
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

void AlienwareMonitorController::Initialize()
{
    unsigned char packet[65];

    memset(packet, 0xFF, sizeof(packet));

    packet[0] = 0x00;
    packet[1] = 0x95;
    packet[2] = 0x00;
    packet[3] = 0x00;
    packet[4] = 0x00;

    hid_write(dev, packet, sizeof(packet));

    /*-----------------------------------------------------*\
    | Delay 50 milliseconds                                 |
    \*-----------------------------------------------------*/
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    memset(packet, 0xFF, sizeof(packet));

    packet[0] = 0x00;
    packet[1] = 0x92;
    packet[2] = 0x37;
    packet[3] = 0x08;
    packet[4] = 0x00;
    packet[5] = 0x51;
    packet[6] = 0x85;
    packet[7] = 0x01;
    packet[8] = 0xFE;
    packet[9] = 0x03;
    packet[10] = 0x00;
    packet[11] = 0x06;
    packet[12] = 0x40;

    hid_write(dev, packet, sizeof(packet));

    /*-----------------------------------------------------*\
    | Delay 50 milliseconds                                 |
    \*-----------------------------------------------------*/
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    memset(packet, 0x00, sizeof(packet));

    packet[0] = 0x00;
    packet[1] = 0x93;
    packet[2] = 0x37;
    packet[3] = 0x12;

    hid_write(dev, packet, sizeof(packet));

    /*-----------------------------------------------------*\
    | Delay 50 milliseconds                                 |
    \*-----------------------------------------------------*/
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}
