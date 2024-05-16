/*---------------------------------------------------------*\
| ElgatoKeyLightController.h                                |
|                                                           |
|   Driver for Elgato Key Light                             |
|                                                           |
|   Monks (imtherealestmonkey@gmail.com),       11 Nov 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <cmath>
#include <string>
#include <thread>
#include <vector>
#include "RGBController.h"
#include "net_port.h"
#include "hsv.h"

class ElgatoKeyLightController
{
public:
    ElgatoKeyLightController(std::string ip);
    ~ElgatoKeyLightController();

    std::string GetLocation();
    std::string GetName();
    std::string GetVersion();
    std::string GetManufacturer();
    std::string GetUniqueID();

    void SetColor(hsv_t hsv_color);

private:
    std::string GetRequest(int brightness, int temperature);
    int HSVToK(int hue);
    std::string         location;
    net_port            port;
};
