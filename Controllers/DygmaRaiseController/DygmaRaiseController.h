/*---------------------------------------------------------*\
| DygmaRaiseController.h                                    |
|                                                           |
|   Driver for Dygma Raise keyboard                         |
|                                                           |
|   Timo Schlegel (@eispalast)                  Dec 12 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include "RGBController.h"
#include "serial_port.h"

#define DYGMA_RAISE_VID 0x1209
#define DYGMA_RAISE_PID 0x2201

#define DYGMA_RAISE_BAUD 115200
#define MAX_LEN (4*3*132+9) //max. 4 Bytes per led channel * 3 channels * 132 leds + codeword led.theme

class DygmaRaiseController
{
public:
    DygmaRaiseController();
    ~DygmaRaiseController();

    void Initialize(char* port);
    std::string GetDeviceLocation();
    void SendDirect(std::vector<RGBColor>colors, size_t led_num);
private:
    std::string                 location;
    std::string                 port_name;
    serial_port *               serialport = nullptr;
};
