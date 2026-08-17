/*---------------------------------------------------------*\
| GameSirController.cpp                                     |
|                                                           |
|   GameSir RGB Device                                      |
|                                                           |
|   Added by OpenRGB Community                  08 Aug 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "GameSirController.h"
#include <cstring>
#include "StringUtils.h"

GameSirController::GameSirController(hid_device* dev_handle, const char* path)
{
    dev      = dev_handle;
    location = path;
}

GameSirController::~GameSirController()
{
    hid_close(dev);
}

std::string GameSirController::GetLocation()
{
    return("HID: " + location);
}

std::string GameSirController::GetSerialString()
{
    wchar_t serial_string[128];
    memset(serial_string, 0x00, sizeof(serial_string));
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void GameSirController::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char buf[64];
    memset(buf, 0x00, sizeof(buf));

    buf[0] = 0x05;
    buf[1] = 0x08;
    buf[2] = 0x0A;
    buf[3] = 0x01;
    buf[4] = 0x03;
    buf[5] = red;
    buf[6] = green;
    buf[7] = blue;
    buf[8] = 0x00;

    unsigned int checksum = 0;
    for(int i = 0; i < 9; i++)
    {
        checksum += buf[i];
    }
    
    buf[9] = checksum & 0xFF;

    hid_write(dev, buf, sizeof(buf));
}
