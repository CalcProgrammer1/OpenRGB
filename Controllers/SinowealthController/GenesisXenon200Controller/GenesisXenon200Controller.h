/*---------------------------------------------------------*\
| GenesisXenon200Controller.h                               |
|                                                           |
|   Driver for Genesis Xenon 200 mouse                      |
|                                                           |
|   chrabonszcz                                    Jul 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <hidapi.h>
#include "RGBController.h"

class GenesisXenon200Controller
{
public:
    GenesisXenon200Controller(hid_device* dev_handle, hid_device* cmd_dev_handle, const char* path, std::string dev_name);
    ~GenesisXenon200Controller();

    std::string GetLocationString();
    std::string GetNameString();

    void        SaveMode(unsigned char mode, unsigned char value, RGBColor color);

private:
    hid_device* dev;
    hid_device* cmd_dev;
    std::string location;
    std::string name;
};
