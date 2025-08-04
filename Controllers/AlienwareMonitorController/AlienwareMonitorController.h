/*---------------------------------------------------------*\
| AlienwareMonitorController.h                              |
|                                                           |
|   Detector for Alienware monitors                         |
|                                                           |
|   Adam Honse (CalcProgrammer1)                08 May 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <hidapi.h>
#include <vector>
#include <string>

class AlienwareMonitorController
{
public:
AlienwareMonitorController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~AlienwareMonitorController();

    std::string GetLocation();
    std::string GetName();
    std::string GetSerialString();
    void SendColor(unsigned char led_id, unsigned char r, unsigned char g, unsigned char b);

private:
    hid_device* dev;
    std::string location;
    std::string name;

    void Initialize();
};
