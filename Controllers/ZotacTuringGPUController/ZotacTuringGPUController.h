/*---------------------------------------------------------*\
| ZotacTuringGPUController.h                                |
|                                                           |
|   Driver for Zotac Turing GPU                             |
|                                                           |
|   David Henry                                 07 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"
#include "RGBController.h"

enum
{
    ZOTAC_TURING_GPU_REG_COLOR_AND_MODE = 0xA0,
};

enum
{
    ZOTAC_GPU_MODE_STATIC               = 0x00,
    ZOTAC_GPU_MODE_STROBE               = 0x01,
    ZOTAC_GPU_MODE_WAVE                 = 0x02,
    ZOTAC_GPU_MODE_BREATHING            = 0x03,
    ZOTAC_GPU_MODE_COLOR_CYCLE          = 0x04,
};

enum
{
    ZOTAC_GPU_SPEED_SLOWEST             = 0x09,
    ZOTAC_GPU_SPEED_NORMAL              = 0x04,
    ZOTAC_GPU_SPEED_FASTEST             = 0x00
};

class ZotacTuringGPUController
{
public:
    ZotacTuringGPUController(i2c_smbus_interface* bus, u8 dev, std::string dev_name);
    ~ZotacTuringGPUController();

    std::string             GetDeviceLocation();
    std::string             GetDeviceName();

    void                    GetMode(RGBColor& color, int& mode, unsigned int& speed);
    void                    SetMode(RGBColor color, int mode, unsigned int speed);

private:
    i2c_smbus_interface*    bus;
    u8                      dev;
    std::string             name;

};
