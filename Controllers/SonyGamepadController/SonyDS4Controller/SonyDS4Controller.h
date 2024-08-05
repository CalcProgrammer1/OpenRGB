/*---------------------------------------------------------*\
| SonyDS4Controller.h                                       |
|                                                           |
|   Driver for Sony Dualshock 4                             |
|                                                           |
|   Pol Rius (alpemwarrior)                     24 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>

class SonyDS4Controller
{
public:
    SonyDS4Controller(hid_device * device_handle, const char * device_path);
    ~SonyDS4Controller();

    std::string GetLocation();
    std::string GetSerialString();

    void SetColors(unsigned char red, unsigned char green, unsigned char blue);

private:
    hid_device*     dev;
    bool            is_bluetooth = false;
    std::string     location;

    void sendReportUSB(unsigned char red, unsigned char green, unsigned char blue);
    void sendReportBT(unsigned char red, unsigned char green, unsigned char blue);
};
