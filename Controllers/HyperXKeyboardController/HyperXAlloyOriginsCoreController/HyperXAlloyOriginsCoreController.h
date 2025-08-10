/*---------------------------------------------------------*\
| HyperXAlloyOriginsCoreController.h                        |
|                                                           |
|   Driver for HyperX Alloy Origins Core keyboard           |
|                                                           |
|   Volodymyr Nazarchuk (Vavooon)               28 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

class HyperXAlloyOriginsCoreController
{
public:
    HyperXAlloyOriginsCoreController(hid_device* dev_handle, hid_device_info* dev_info, std::string dev_name);
    ~HyperXAlloyOriginsCoreController();

    std::string     GetDeviceLocation();
    std::string     GetNameString();
    std::string     GetSerialString();
    std::string     GetFirmwareVersion();
    unsigned int    GetVariant();

    void SetLEDsDirect(std::vector<RGBColor> colors);
    void SetBrightness(unsigned int brightness);

private:
    hid_device*     dev;
    std::string     location;
    std::string     firmware_version;
    std::string     name;
};
