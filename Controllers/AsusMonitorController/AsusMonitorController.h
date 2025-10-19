/*---------------------------------------------------------*\
| AsusMonitorController.h                                   |
|                                                           |
|   Driver for Asus monitors                                |
|                                                           |
|   Morgan Guimard (morg)                       19 oct 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define ASUS_MONITOR_REPORT_SIZE 65

class AsusMonitorController
{
public:
    AsusMonitorController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~AsusMonitorController();

    std::string                 GetDeviceLocation();
    std::string                 GetNameString();
    std::string                 GetSerialString();
    unsigned int                GetNumberOfLEDs();
    void                        SetDirect(std::vector<RGBColor> colors);    
    void                        SendInit();

protected:
    hid_device* dev;

private:
    std::string                 location;
    std::string                 name;
};
