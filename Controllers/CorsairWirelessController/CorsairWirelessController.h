/*---------------------------------------------------------*\
| CorsairWirelessController.h                               |
|                                                           |
|   Driver for Corsair wireless keyboard                    |
|                                                           |
|   Adam Honse (CalcProgrammer1)                08 May 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

class CorsairWirelessController
{
public:
    CorsairWirelessController(hid_device* dev_handle, const char* path);
    ~CorsairWirelessController();

    device_type     GetDeviceType();
    std::string     GetDeviceLocation();
    std::string     GetFirmwareString();
    std::string     GetName();
    std::string     GetSerialString();

    void            SetLEDs(std::vector<RGBColor> colors);
    void            SetName(std::string device_name);

private:
    hid_device*             dev;

    std::string             firmware_version;
    std::string             location;
    std::string             name;
    device_type             type;

    void            EnterDirectMode();
    void            ExitDirectMode();
    void            StartDirectMode();
    void            SendDirectFrame(bool first_frame, unsigned char* data);
};
