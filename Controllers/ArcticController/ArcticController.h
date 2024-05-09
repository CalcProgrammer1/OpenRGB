/*---------------------------------------------------------*\
| ArcticController.h                                        |
|                                                           |
|   Driver for Arctic devices                               |
|                                                           |
|   Armin Wolf (Wer-Wolf)                       09 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <algorithm>
#include "RGBController.h"
#include "serial_port.h"

class ArcticController
{
public:
    ArcticController(const std::string &portname);
    ~ArcticController();

    void SetChannels(std::vector<RGBColor> colors);
    bool IsPresent();

    std::string GetLocation();

private:
    std::string port_name;
    serial_port serialport;
};
