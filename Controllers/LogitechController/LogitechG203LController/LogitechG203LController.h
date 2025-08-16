/*---------------------------------------------------------*\
| LogitechG203LController.h                                 |
|                                                           |
|   Driver for Logitech G203L                               |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

enum
{
    LOGITECH_G203L_MODE_DIRECT      = 0x07,
    LOGITECH_G203L_MODE_OFF         = 0x00,
    LOGITECH_G203L_MODE_STATIC      = 0x01,
    LOGITECH_G203L_MODE_CYCLE       = 0x02,
    LOGITECH_G203L_MODE_WAVE        = 0x03,
    LOGITECH_G203L_MODE_BREATHING   = 0x04,
    LOGITECH_G203L_MODE_COLORMIXING = 0x06,
};

class LogitechG203LController
{
public:
    LogitechG203LController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~LogitechG203LController();

    std::string GetDeviceLocation();
    std::string GetNameString();
    std::string GetSerialString();

    void        SetSingleLED(int led, unsigned char red, unsigned char green, unsigned char blue);
    void        SetMode(int mode, int speed, unsigned char brightness, unsigned char dir, unsigned char red, unsigned char green, unsigned char blue);
    void        SetDevice(std::vector<RGBColor> colors);

private:
    hid_device* dev;
    std::string location;
    std::string name;

    void        SendApply();
    void        SendPacket(unsigned char* buffer);
};
