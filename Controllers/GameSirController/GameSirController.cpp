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
    unsigned char buf[65];
    memset(buf, 0x00, sizeof(buf));

    buf[0] = 0x00;
    buf[1] = 0x05;
    buf[2] = 0x08;
    buf[3] = 0x0A;
    buf[4] = 0x01;
    buf[5] = 0x03;
    buf[6] = red;
    buf[7] = green;
    buf[8] = blue;
    buf[9] = 0x00;

    unsigned int checksum = 0;
    for(int i = 1; i < 10; i++)
    {
        checksum += buf[i];
    }

    buf[10] = checksum & 0xFF;

    hid_write(dev, buf, sizeof(buf));
}
