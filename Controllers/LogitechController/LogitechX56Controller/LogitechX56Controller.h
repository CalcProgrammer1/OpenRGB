/*---------------------------------------------------------*\
| LogitechX56Controller.h                                   |
|                                                           |
|   Driver for Logitech X56                                 |
|                                                           |
|   Edbgon                                      11 Jun 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define X56_CONTROLLER_PACKET_SIZE          64

class LogitechX56Controller
{
public:
    LogitechX56Controller(hid_device* dev_handle, const char* path);

    ~LogitechX56Controller();

    std::string GetDeviceLocation();
    char*       GetDeviceName();
    std::string GetSerialString();

    void SetColor(RGBColor colors, uint8_t brightness);
    void Save();

private:
    char                    device_name[32];
    hid_device*             dev;
    std::string             location;
};
