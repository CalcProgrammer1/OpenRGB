/*---------------------------------------------------------*\
| FanBusController.h                                        |
|                                                           |
|   Driver for FanBus devices                               |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      12 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "FanBusInterface.h"
#include "RGBController.h"

class FanBusController
{
public:
    FanBusController(FanBusInterface* bus_ptr, unsigned char dev_addr);
    ~FanBusController();

    std::string GetLocation();

    void        SetLEDs(std::vector<RGBColor> colors);

private:
    std::string         port_name;
    FanBusInterface*    bus;
    unsigned char       dev;
};
