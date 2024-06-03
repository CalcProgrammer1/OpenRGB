/*---------------------------------------------------------*\
| PhilipsHueEntertainmentController.h                       |
|                                                           |
|   Detector for Philips Hue Entertainment Mode             |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      06 Nov 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include "Bridge.h"
#include "EntertainmentMode.h"
#include "Group.h"
#include "RGBController.h"

#define HUE_ENTERTAINMENT_HEADER_SIZE   16
#define HUE_ENTERTAINMENT_LIGHT_SIZE    9

class PhilipsHueEntertainmentController
{
public:
    PhilipsHueEntertainmentController(hueplusplus::Bridge& bridge_ptr, hueplusplus::Group group_ptr);
    ~PhilipsHueEntertainmentController();

    std::string GetLocation();
    std::string GetName();
    std::string GetVersion();
    std::string GetManufacturer();
    std::string GetUniqueID();
    unsigned int GetNumLEDs();

    void SetColor(RGBColor* colors);

    void Connect();
    void Disconnect();

private:
    hueplusplus::Bridge&            bridge;
    hueplusplus::Group              group;
    hueplusplus::EntertainmentMode* entertainment;

    std::string                     location;
    unsigned int                    num_leds;
    bool                            connected;
};
