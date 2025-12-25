/*---------------------------------------------------------*\
| ManliGPUController.h                                      |
|                                                           |
|   Driver for Manli GPU RGB controllers                   |
|                                                           |
|   Based on ZotacV2GPUController                           |
|   Adapted for Manli RTX 4090 Gallardo                     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"
#include "RGBController.h"

enum
{
    MANLI_GPU_REG_RGB = 0xA0,
};

enum
{
    MANLI_GPU_MODE_STATIC      = 0x00,  // Static color
    MANLI_GPU_MODE_BREATHING   = 0x01,  // Breathing effect
    MANLI_GPU_MODE_WAVE        = 0x02,  // Wave effect (no brightness - HW limitation)
    MANLI_GPU_MODE_STROBING    = 0x03,  // Strobing effect
    MANLI_GPU_MODE_RAINBOW     = 0x08,  // Rainbow effect (no brightness - HW limitation)
    MANLI_GPU_MODE_COLOR_CYCLE = 0x10,  // Color Cycle (breathing + flag 0x07)
};

struct ManliGPUConfig
{
    int numberOfZones = 0;
    bool supportsExternalLEDStrip = false;
};

struct ManliGPUZone
{
    int mode = 0;
    RGBColor color1 = ToRGBColor(0, 0, 0);
    unsigned int speed = 0;
    unsigned int brightness = 0;
};

class ManliGPUController
{
public:
    ManliGPUController(i2c_smbus_interface* bus, u8 dev, std::string dev_name);
    ~ManliGPUController();

    std::string             GetDeviceLocation();
    std::string             GetName();
    std::string             GetVersion();

    bool                    TurnOnOff(bool on);
    bool                    SetMode(ManliGPUZone zoneConfig);

private:
    i2c_smbus_interface*    bus;
    u8                      dev;
    std::string             name;
    std::string             version;

    bool                    ReadVersion();
    bool                    SendCommand(bool on, ManliGPUZone zoneConfig);
};

