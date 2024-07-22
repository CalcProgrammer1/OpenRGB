/*---------------------------------------------------------*\
| RaspberryPiController_Linux.h                             |
|                                                           |
|   Driver for Raspberry Pi GPIO ARGB                       |
|                                                           |
|   Adam Honse (CalcProgrammer1)                21 Jul 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "RGBController.h"
#include "ws2811.h"

#define RPI_WS2811_DMA  10
#define RPI_WS2811_GPIO 18

class RaspberryPiController
{
public:
    RaspberryPiController();
    ~RaspberryPiController();

    std::string GetDeviceLocation();

    bool Initialize();
    void SetColors(RGBColor * colors);
    void SetSize(unsigned int new_size);

private:
    ws2811_t        rpi_ws2811;
};
