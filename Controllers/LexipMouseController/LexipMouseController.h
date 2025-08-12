/*---------------------------------------------------------*\
| LexipMouseController.h                                    |
|                                                           |
|   Driver for Lexip mouse                                  |
|                                                           |
|   Morgan Guimard (morg)                       21 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define PACKET_DATA_LENGTH 64

class LexipMouseController
{
public:
    LexipMouseController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~LexipMouseController();

    std::string                 GetDeviceLocation();
    std::string                 GetNameString();
    std::string                 GetSerialString();

    void                        SetDirect(RGBColor color);
protected:
    hid_device* dev;

private:
    std::string                 location;
    std::string                 name;
    std::string                 version;
};
