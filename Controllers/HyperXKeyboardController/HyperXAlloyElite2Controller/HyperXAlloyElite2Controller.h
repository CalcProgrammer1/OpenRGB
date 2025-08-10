/*---------------------------------------------------------*\
| HyperXAlloyElite2Controller.h                             |
|                                                           |
|   Driver for HyperX Alloy Elite 2 keyboard                |
|                                                           |
|   KundaPanda (vojdo)                          02 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

class HyperXAlloyElite2Controller
{
public:
    HyperXAlloyElite2Controller(hid_device* dev_handle, const char* path, std::string dev_name);
    ~HyperXAlloyElite2Controller();

    std::string                         GetDeviceLocation();
    std::string                         GetNameString();
    std::string                         GetSerialString();

    void                                SetLEDsDirect(const std::vector<RGBColor>& colors);

private:
    hid_device*                         dev;
    std::string                         location;
    std::string                         name;

    void                                SendDirectInitialization();
};
