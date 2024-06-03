/*---------------------------------------------------------*\
| PhilipsHueController.h                                    |
|                                                           |
|   Driver for Philips Hue                                  |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      15 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include "HueDeviceTypes.h"

class PhilipsHueController
{
public:
    PhilipsHueController(hueplusplus::Light light_ptr, std::string bridge_ip);
    ~PhilipsHueController();

    std::string GetLocation();
    std::string GetName();
    std::string GetVersion();
    std::string GetManufacturer();
    std::string GetUniqueID();

    void SetColor(unsigned char red, unsigned char green, unsigned char blue);

private:
    hueplusplus::Light  light;
    std::string         location;
    bool                dark;
};
