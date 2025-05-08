/*---------------------------------------------------------*\
| GigabyteRGBFusion2BlackwellGPUController.h                |
|                                                           |
|   Driver for Gigabyte RGB Fusion 2 Blackwell GPU          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"
#include "RGBController.h"
#include "GigabyteRGBFusion2BlackwellGPUDefinitions.h"

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
    RGB_FUSION2_BLACKWELL_GPU_REG_COLOR            = 0x12,
    RGB_FUSION2_BLACKWELL_GPU_REG_MODE             = 0x16  // Used for 'Intelligent' mode
};

enum
{
    RGB_FUSION2_BLACKWELL_GPU_MODE_STATIC          = 0x01,
    RGB_FUSION2_BLACKWELL_GPU_MODE_BREATHING       = 0x02,
    RGB_FUSION2_BLACKWELL_GPU_MODE_FLASHING        = 0x03,
    RGB_FUSION2_BLACKWELL_GPU_MODE_DUAL_FLASHING   = 0x04,
    RGB_FUSION2_BLACKWELL_GPU_MODE_COLOR_CYCLE     = 0x05,
    RGB_FUSION2_BLACKWELL_GPU_MODE_WAVE            = 0x06, //not available to Eagle/Aero
    RGB_FUSION2_BLACKWELL_GPU_MODE_GRADIENT        = 0x07, //not available to Eagle/Aero
    RGB_FUSION2_BLACKWELL_GPU_MODE_COLOR_SHIFT     = 0x08, //not available to Eagle/Aero
    RGB_FUSION2_BLACKWELL_GPU_MODE_DAZZLE          = 0x0A, //not available to Eagle/Aero
};

enum
{
    RGB_FUSION2_BLACKWELL_GPU_SPEED_SLOWEST        = 0x01,
    RGB_FUSION2_BLACKWELL_GPU_SPEED_NORMAL         = 0x03,
    RGB_FUSION2_BLACKWELL_GPU_SPEED_FASTEST        = 0x06
};

enum
{
    RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MIN       = 0x01,
    RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX       = 0x0A
};

enum
{
    RGB_FUSION2_BLACKWELL_GPU_SINGLE_ZONE          = 0,
    RGB_FUSION2_BLACKWELL_GPU_GAMING_LAYOUT        = 1,
};

class RGBFusion2BlackwellGPUController
{
public:
    RGBFusion2BlackwellGPUController(i2c_smbus_interface* bus, rgb_fusion_dev_id dev);
    ~RGBFusion2BlackwellGPUController();

    RGBColor        zone_color[RGB_FUSION_2_BLACKWELL_GPU_NUMBER_OF_ZONES];

    std::string     GetDeviceLocation();
    void            SaveConfig();

    void            SetZone(uint8_t zone, uint8_t mode, fusion2_config zone_config);
    void            SetMode(uint8_t zone, uint8_t mode, fusion2_config zone_config);
private:
    i2c_smbus_interface*    bus;
    rgb_fusion_dev_id       dev;

};
