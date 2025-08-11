/*---------------------------------------------------------*\
| HYTEMousematController_Windows_MacOS.h                    |
|                                                           |
|   Driver for HYTE mousemat (Serial implementation for     |
|   Windows and MacOS)                                      |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      18 Jul 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "RGBController.h"
#include "serial_port.h"

class HYTEMousematController
{
public:
    HYTEMousematController(char* port, std::string dev_name);
    ~HYTEMousematController();

    std::string     GetLocation();
    std::string     GetName();

    void            FirmwareAnimationControl(bool enabled);
    void            StreamingCommand(RGBColor* colors);

private:
    std::string     name;
    std::string     port_name;
    serial_port *   serialport = nullptr;
};
