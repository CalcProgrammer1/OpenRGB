/*---------------------------------------------------------*\
| ColorfulGPUController.h                                   |
|                                                           |
|   Driver for Colorful GPU                                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"
#include "RGBController.h"

typedef unsigned char colorful_gpu_dev_id;

#define COLORFUL_PACKET_LENGTH_V1 11
#define COLORFUL_PACKET_LENGTH_V2 122

class ColorfulGPUController
{
public:
    ColorfulGPUController(i2c_smbus_interface* bus, colorful_gpu_dev_id dev);
    ~ColorfulGPUController();

    std::string GetDeviceLocation();
    void        SetDirect(RGBColor color);

private:
    i2c_smbus_interface *   bus;
    colorful_gpu_dev_id     dev;
};
