/*---------------------------------------------------------*\
| SinowealthKeyboard10cController.h                         |
|                                                           |
|   Driver for Sinowealth Keyboards with PID 010C           |
|                                                           |
|   Rodrigo Tavares                             27 Nov 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController.h"
#include <vector>
#include <hidapi.h>

#pragma once

namespace kbd10c
{
    enum
    {
        MODE_OFF    = 0x0,
        MODE_DIRECT = 0x1,
    };
}


class SinowealthKeyboard10cController
{
public:
    SinowealthKeyboard10cController(hid_device* dev_handle, char *_path, std::string dev_name);
    ~SinowealthKeyboard10cController();

    unsigned int  GetLEDCount();
    std::string   GetLocation();
    std::string   GetName();
    std::string   GetSerialString();
    unsigned char GetCurrentMode();
    void          SetLEDsDirect(std::vector<RGBColor> colors);
private:
    hid_device*   dev;
    device_type   type;
    unsigned char current_mode;
    std::string   location;
    std::string   name;
};
