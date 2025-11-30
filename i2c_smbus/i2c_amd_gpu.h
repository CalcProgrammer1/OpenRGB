/*---------------------------------------------------------*\
| i2c_amd_gpu.h                                             |
|                                                           |
|   Bits specific to AMD GPUs to reliably detect            |
|   the I2C bus that has RGB control                        |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <cstring>
#include "i2c_smbus.h"

inline constexpr const char * RECOGNIZED_I2C_BUS_NAMES[] =
{
    // Windows i2c bus
    "AMD ADL",
    // Linux i2c bus name since kernel 6.15
    "AMDGPU DM i2c OEM bus",
    // Linux i2c bus name since kernel 6.15 - older GPUs
    "AMDGPU i2c bit bus OEM 0x97",
    nullptr
};

inline bool is_amd_gpu_i2c_bus(const i2c_smbus_interface *bus)
{
    const char *name;
    size_t idx = 0;
    while((name = RECOGNIZED_I2C_BUS_NAMES[idx++]) != nullptr)
    {
        const char *pos = std::strstr(bus->device_name, name);
        if(pos == bus->device_name)
        {
            return true;
        }
    }

    return false;
}
