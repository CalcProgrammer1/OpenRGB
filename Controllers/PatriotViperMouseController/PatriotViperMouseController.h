/*---------------------------------------------------------*\
| PatriotViperMouseController.h                             |
|                                                           |
|   Detector for Patriot Viper Mouse                        |
|                                                           |
|   mi4code                                     23 May 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <hidapi.h>
#include "StringUtils.h"
#include <cstring>
#include "RGBController.h"

class PatriotViperMouseController
{
public:
    PatriotViperMouseController(hid_device* dev_handle, const char* path);
    ~PatriotViperMouseController();

    std::string GetLocation();
    std::string GetSerial();
    void SetRGB(std::vector<RGBColor> colors);

private:
    hid_device*             _dev;
    std::string             _location;
};