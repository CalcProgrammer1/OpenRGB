/*---------------------------------------------------------*\
| RetroidAYNController_Linux.h                              |
|                                                           |
|   Driver for Retroid and AYN handheld RGB LEDs            |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      08 Nov 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <fstream>
#include <string>
#include <vector>
#include "RGBControllerInterface.h"

class RetroidAYNController
{
public:
    RetroidAYNController(std::string dev_name);
    ~RetroidAYNController();

    std::string                 GetName();
    std::string                 GetLocation();

    std::size_t                 GetLEDCount();

    void                        AddLED(std::string led_path);

    unsigned int                GetBrightness(unsigned int led_idx);
    RGBColor                    GetLEDColor(unsigned int led_idx);
    unsigned int                GetMaxBrightness(unsigned int led_idx);

    void                        SetBrightness(unsigned int led_idx, unsigned int brightness);
    void                        SetLEDColor(unsigned int led_idx, RGBColor color);

private:
    std::string                 name;

    std::vector<std::string>    led_paths;
    std::vector<std::ofstream>  led_multi_intensity;
    std::vector<std::ofstream>  led_brightness;
    std::vector<int>            led_channel_order_red;
    std::vector<int>            led_channel_order_green;
    std::vector<int>            led_channel_order_blue;

    void                        ReadChannelOrder(std::string led_path);
};
