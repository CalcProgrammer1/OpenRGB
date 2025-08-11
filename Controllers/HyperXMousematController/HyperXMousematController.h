/*---------------------------------------------------------*\
| HyperXMousematController.h                                |
|                                                           |
|   Driver for HyperX mousemat                              |
|                                                           |
|   Adam Honse (CalcProgrammer1)                25 Oct 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "RGBController.h"
#include "hidapi_wrapper.h"

class HyperXMousematController
{
public:
    HyperXMousematController(hidapi_wrapper hid_wrapper, hid_device* dev_handle, const char* path, std::string dev_name);
    ~HyperXMousematController();

    std::string GetDeviceLocation();
    std::string GetNameString();
    std::string GetSerialString();

    void SendDirect
            (
            RGBColor*       color_data
            );

private:
    hidapi_wrapper          wrapper;
    hid_device*             dev;
    std::string             location;
    std::string             name;
};
