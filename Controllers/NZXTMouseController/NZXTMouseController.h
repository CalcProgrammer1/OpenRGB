/*---------------------------------------------------------*\
| NZXTMouseController.h                                     |
|                                                           |
|   Driver for NZXT Mouse                                   |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      13 Dec 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <hidapi.h>
#include "RGBController.h"

class NZXTMouseController
{
public:
    NZXTMouseController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~NZXTMouseController();

    std::string     GetFirmwareVersion();
    std::string     GetLocation();
    std::string     GetName();
    std::string     GetSerialString();

    void            SetLEDs
                        (
                        RGBColor *      colors
                        );

private:
    hid_device*     dev;

    char            firmware_version[16];
    std::string     location;
    std::string     name;

    void            SendFirmwareRequest();
};
