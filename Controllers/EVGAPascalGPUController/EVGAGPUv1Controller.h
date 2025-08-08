/*---------------------------------------------------------*\
| EVGAGPUv1Controller.h                                     |
|                                                           |
|   Driver for EVGA V1 (Pascal) GPU                         |
|                                                           |
|   Adam Honse (CalcProgrammer1)                11 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"

typedef unsigned char	evga_dev_id;

#define EVGAGPUV1_CONTROLLER_NAME   "EVGAv1"

enum
{
    EVGA_GPU_V1_REG_MODE                = 0x0C,
    EVGA_GPU_V1_REG_RED                 = 0x09,
    EVGA_GPU_V1_REG_GREEN               = 0x0A,
    EVGA_GPU_V1_REG_BLUE                = 0x0B,
};

enum
{
    EVGA_GPU_V1_MODE_OFF                = 0x00,
    EVGA_GPU_V1_MODE_CUSTOM             = 0x01,
    EVGA_GPU_V1_MODE_RAINBOW            = 0x02,
    EVGA_GPU_V1_MODE_BREATHING          = 0x05,
};

class EVGAGPUv1Controller
{
public:
    EVGAGPUv1Controller(i2c_smbus_interface* bus, evga_dev_id dev, std::string dev_name);
    ~EVGAGPUv1Controller();

    std::string     GetDeviceLocation();
    std::string     GetDeviceName();

    unsigned char   GetMode();
    unsigned char   GetRed();
    unsigned char   GetGreen();
    unsigned char   GetBlue();

    void            SetColor(unsigned char red, unsigned char green, unsigned char blue);
    void            SetMode(unsigned char mode);
    void            SaveSettings();

private:
    i2c_smbus_interface*    bus;
    evga_dev_id             dev;
    std::string             name;
};
