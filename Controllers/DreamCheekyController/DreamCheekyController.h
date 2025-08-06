/*---------------------------------------------------------*\
| DreamCheekyController.h                                   |
|                                                           |
|   Driver for Dream Cheeky devices                         |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      06 Sep 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>

class DreamCheekyController
{
public:
    DreamCheekyController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~DreamCheekyController();

    std::string GetDeviceLocation();
    std::string GetNameString();
    std::string GetSerialString();

    void SetColor(unsigned char red, unsigned char grn, unsigned char blu);

private:
    hid_device* dev;
    std::string location;
    std::string name;
};
