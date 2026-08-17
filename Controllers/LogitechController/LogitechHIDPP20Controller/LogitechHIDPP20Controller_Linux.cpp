/*---------------------------------------------------------*\
| LogitechHIDPP20Controller_Linux.cpp                       |
|                                                           |
|   Linux-specific path-migration and device-name lookup    |
|   for the unified Logitech HID++ 2.0 controller.          |
|                                                           |
|   Uses sysfs (/sys/class/hidraw) to find the same         |
|   physical device on a new hidraw path after a USB <->    |
|   wireless transition, and to read Centurion sub-device   |
|   friendly names from HID_NAME uevent fields.             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include <cctype>
#include <cstdio>
#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <dirent.h>
#include "LogitechHIDPP20Controller.h"
#include "LogManager.h"

#define LOG_TAG log_tag.c_str()

std::string LogitechHIDPP20Controller::GetCenturionSubDeviceName(const std::string& path)
{
    /*-----------------------------------------------------*\
    | Prefer the USB product string (Solaar's source)       |
    | over HID_NAME's "Logitech " prefix: read              |
    | /sys/class/hidraw/<N>/device/../../product, with      |
    | HID_NAME from the hidraw uevent as the fallback.      |
    \*-----------------------------------------------------*/
    std::string name;
    size_t pos = path.rfind("hidraw");

    if(pos == std::string::npos)
    {
        return name;
    }

    std::string hidraw = path.substr(pos);

    std::ifstream pf("/sys/class/hidraw/" + hidraw + "/device/../../product");

    if(pf)
    {
        std::getline(pf, name);

        while(!name.empty() && (name.back() == '\n' || name.back() == '\r'))
        {
            name.pop_back();
        }
    }

    if(!name.empty())
    {
        return name;
    }

    std::string uevent_path = "/sys/class/hidraw/" + hidraw + "/device/uevent";
    FILE* f = fopen(uevent_path.c_str(), "r");

    if(!f)
    {
        return name;
    }

    char line[256];

    while(fgets(line, sizeof(line), f))
    {
        if(strncmp(line, "HID_NAME=", 9) == 0)
        {
            name = line + 9;

            while(!name.empty() && (name.back() == '\n' || name.back() == '\r'))
            {
                name.pop_back();
            }

            break;
        }
    }

    fclose(f);
    return name;
}
