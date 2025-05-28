/*---------------------------------------------------------*\
| MadCatzCyborgController.h                                 |
|                                                           |
|   Driver for MadCatz Cyborg Gaming Light                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>

class MadCatzCyborgController
{
public:
    MadCatzCyborgController(hid_device* dev_handle, const char* path);
    ~MadCatzCyborgController();

    std::string     GetDeviceLocation();
    std::string     GetSerialString();

    void            Initialize();
    void            SetLEDColor(unsigned char red, unsigned char green, unsigned char blue);
    void            SetIntensity(unsigned char intensity);

private:
    hid_device*     dev;
    std::string     location;
    
    // Protocol constants
    enum Commands
    {
        CMD_ENABLE    = 0xA1,
        CMD_COLOR     = 0xA2,
        CMD_INTENSITY = 0xA6
    };
};
