/*---------------------------------------------------------*\
| LogitechG522Controller.h                                  |
|                                                           |
|   Driver for Logitech G522 headset                        |
|                                                           |
|   fawmdev                                     03 Dec 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

/*-----------------------------------------------------*\
| Logitech G522 Protocol Constants                      |
\*-----------------------------------------------------*/
#define LOGITECH_G522_REPORT_ID             0x50
#define LOGITECH_G522_PACKET_SIZE           65

enum
{
    LOGITECH_G522_MODE_OFF                  = 0x00,
    LOGITECH_G522_MODE_DIRECT               = 0x01,
};

class LogitechG522Controller
{
public:
    LogitechG522Controller(hid_device* dev_handle, const char* path, std::string dev_name);
    ~LogitechG522Controller();

    std::string GetDeviceLocation();
    std::string GetDeviceName();

    void        SetColor(unsigned char red, unsigned char green, unsigned char blue);
    void        SetOff();

private:
    hid_device* dev;
    std::string location;
    std::string name;

    void        SendColorPacket(unsigned char red, unsigned char green, unsigned char blue);
};
