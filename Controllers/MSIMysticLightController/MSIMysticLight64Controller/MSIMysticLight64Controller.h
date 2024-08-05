/*---------------------------------------------------------*\
| MSIMysticLight64Controller.h                              |
|                                                           |
|   Driver for MSI Mystic Light 64-byte motherboard         |
|                                                           |
|   T-bond                                      03 Apr 2020 |
|   Adam Honse                                  06 Mar 2021 |
|   Elchanan Haas                               23 Aug 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <cstring>
#include <limits>
#include <hidapi.h>
#include "MSIMysticLightCommon.h"
#include "RGBController.h"

enum MSI_64_MODE
{
    MSI_64_OFF             = 0,
    MSI_64_STEADY          = 1,
    MSI_64_BREATHING       = 2,
    MSI_64_PULSE           = 3,
    MSI_64_DOUBLE_PULSE    = 4,
    MSI_64_CYCLE           = 5,
    MSI_64_SMOOTH_CYCLE    = 6,
};

class MSIMysticLight64Controller
{
public:
    MSIMysticLight64Controller
        (
        hid_device*     handle,
        const char      *path
        );
    ~MSIMysticLight64Controller();

    void            SetMode
                        (
                        MSI_64_MODE        mode,
                        MSI_SPEED       speed,
                        MSI_BRIGHTNESS  brightness,
                        unsigned int num_colors,
                        Color colors[]
                        );


    std::string     GetDeviceName();
    std::string     GetDeviceLocation();
    std::string     GetFWVersion();
    std::string     GetSerial();

private:

    hid_device*                     dev;
    std::string                     location;
};
