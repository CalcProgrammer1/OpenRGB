/*---------------------------------------------------------*\
| GigabyteRGBFusionGPUController.h                          |
|                                                           |
|   Driver for Gigabyte Aorus RGB Fusion GPU                |
|                                                           |
|   Adam Honse (CalcProgrammer1)                20 Feb 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"

typedef unsigned char	rgb_fusion_dev_id;

enum
{
    RGB_FUSION_GPU_REG_COLOR            = 0x40,
    RGB_FUSION_GPU_REG_MODE             = 0x88,
    RGB_FUSION_GPU_REG_SAVE             = 0xAA,
};

enum
{
    RGB_FUSION_GPU_MODE_STATIC          = 0x01,
    RGB_FUSION_GPU_MODE_BREATHING       = 0x02,
    RGB_FUSION_GPU_MODE_FLASHING        = 0x04,
    RGB_FUSION_GPU_MODE_DUAL_FLASHING   = 0x08,
    RGB_FUSION_GPU_MODE_COLOR_CYCLE     = 0x10,
    RGB_FUSION_GPU_MODE_SPECTRUM_CYCLE  = 0x11
};

enum
{
    RGB_FUSION_GPU_SPEED_SLOWEST        = 0x00,
    RGB_FUSION_GPU_SPEED_NORMAL         = 0x05,
    RGB_FUSION_GPU_SPEED_FASTEST        = 0x09
};

enum
{
    RGB_FUSION_GPU_BRIGHTNESS_MIN       = 0x00,
    RGB_FUSION_GPU_BRIGHTNESS_MAX       = 0x63
};

class RGBFusionGPUController
{
public:
    RGBFusionGPUController(i2c_smbus_interface* bus, rgb_fusion_dev_id dev, std::string dev_name);
    ~RGBFusionGPUController();

    std::string     GetDeviceLocation();
    std::string     GetDeviceName();

    void            SetColor(unsigned char red, unsigned char green, unsigned char blue);
    void            SetMode(unsigned char mode, unsigned char speed, unsigned char brightness);
    void            Save();

private:
    i2c_smbus_interface*    bus;
    rgb_fusion_dev_id       dev;
    std::string             name;
};
