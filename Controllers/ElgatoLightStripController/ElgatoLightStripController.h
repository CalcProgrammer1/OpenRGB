/*---------------------------------------------------------*\
| ElgatoLightStripController.h                              |
|                                                           |
|   Driver for Elgato Light Strip                           |
|                                                           |
|   Monks (@iamtherealestmonkey)                03 Nov 2021 |
|   DomePlaysHD                                 12 Mar 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "net_port.h"
#include "hsv.h"

class ElgatoLightStripController
{
    public:
        ElgatoLightStripController(std::string ip);
        ~ElgatoLightStripController();

        std::string GetLocation();
        std::string GetName();
        std::string GetVersion();
        std::string GetManufacturer();
        std::string GetUniqueID();

        void SetColor(hsv_t hsv_color);
        int GetBrightness();
        void SetBrightness(int brightness);

    private:
        std::string GetRequest(int hue, int saturation, int brightness);
        std::string location;
        std::string firmware_version;
        std::string serialnumber;
        std::string displayname;
        net_port port;
        int device_brightness;
};
