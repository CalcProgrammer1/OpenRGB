/*---------------------------------------------------------*\
| NvidiaESAController.h                                     |
|                                                           |
|   Driver for NVIDIA ESA                                   |
|                                                           |
|   Morgan Guimard (morg)                       18 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

class NvidiaESAController
{
public:
    NvidiaESAController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~NvidiaESAController();

    std::string                 GetDeviceLocation();
    std::string                 GetNameString();
    std::string                 GetSerialString();

    void                        SetZoneColor(unsigned int zone_idx, RGBColor color);

protected:
    hid_device* dev;

private:
    std::string                 location;
    std::string                 name;
};
