/*---------------------------------------------------------*\
| GigabyteRGBFusion2GPUController.h                         |
|                                                           |
|   Driver for Gigabyte Aorus RGB Fusion 2 GPU              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"
#include "RGBController.h"

typedef unsigned char   rgb_fusion_dev_id;

struct fusion2_config
{
    uint8_t     brightness;
    RGBColor    colors[8];
    uint8_t     numberOfColors;
    uint8_t     speed;
    uint8_t     direction;
};

enum
{
    RGB_FUSION2_GPU_REG_COLOR            = 0x40,
    RGB_FUSION2_GPU_REG_MODE             = 0x88,
    RGB_FUSION2_GPU_REG_COLOR_LEFT_MID   = 0xB0,
    RGB_FUSION2_GPU_REG_COLOR_RIGHT      = 0xB1
};

enum
{
    RGB_FUSION2_GPU_MODE_STATIC          = 0x01,
    RGB_FUSION2_GPU_MODE_BREATHING       = 0x02,
    RGB_FUSION2_GPU_MODE_COLOR_CYCLE     = 0x03,
    RGB_FUSION2_GPU_MODE_FLASHING        = 0x04,
    RGB_FUSION2_GPU_MODE_GRADIENT        = 0x05,
    RGB_FUSION2_GPU_MODE_COLOR_SHIFT     = 0x06,
    RGB_FUSION2_GPU_MODE_WAVE            = 0x07,
    RGB_FUSION2_GPU_MODE_DUAL_FLASHING   = 0x08,
    RGB_FUSION2_GPU_MODE_TRICOLOR        = 0x0B
};

enum
{
    RGB_FUSION2_GPU_SPEED_SLOWEST        = 0x00,
    RGB_FUSION2_GPU_SPEED_NORMAL         = 0x02,
    RGB_FUSION2_GPU_SPEED_FASTEST        = 0x05
};

enum
{
    RGB_FUSION2_GPU_BRIGHTNESS_MIN       = 0x00,
    RGB_FUSION2_GPU_BRIGHTNESS_MAX       = 0x63
};

class RGBFusion2GPUController
{
public:
    RGBFusion2GPUController(i2c_smbus_interface* bus, rgb_fusion_dev_id dev, std::string dev_name);
    ~RGBFusion2GPUController();

    RGBColor        zone_color[4];

    std::string     GetDeviceLocation();
    std::string     GetDeviceName();

    void            SaveConfig();

    void            SetZone(uint8_t zone, uint8_t mode, fusion2_config zone_config);
    void            SetMode(uint8_t zone, uint8_t mode, fusion2_config zone_config, uint8_t mystery_flag);

private:
    i2c_smbus_interface*    bus;
    rgb_fusion_dev_id       dev;
    std::string             name;
};
