/*---------------------------------------------------------*\
| CorsairHydro2Controller.h                                 |
|                                                           |
|   Driver for Corsair H100i v2                             |
|                                                           |
|   Tim Demand (tim.dmd)                        10 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include <libusb.h>
#include "RGBController.h"

class CorsairHydro2Controller
{
public:
    CorsairHydro2Controller(libusb_device_handle* dev_handle);
    ~CorsairHydro2Controller();

    std::string GetLocation();

    void SetLED(std::vector<RGBColor>& colors);

private:
    libusb_device_handle* dev;
    std::string firmware_version;
    std::string location;

    void SendInit();
};
