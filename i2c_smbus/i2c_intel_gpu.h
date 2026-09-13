/*---------------------------------------------------------*\
| i2c_intel_gpu.h                                           |
|                                                           |
|   Bits specific to Intel GPUs to reliably detect          |
|   the I2C bus that has RGB control                        |
|                                                           |
|   Jan Dvořák                                  13 Sep 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <cstring>

#include "i2c_smbus.h"
#include "pci_ids.h"

inline constexpr const char * RECOGNIZED_INTEL_I2C_BUS_NAMES[] =
{
    /*-----------------------------------------------------*\
    | Linux Intel discrete GPU internal I2C controller      |
    \*-----------------------------------------------------*/
    "Synopsys DesignWare I2C adapter",
    nullptr
};

inline bool is_intel_gpu_i2c_bus(const i2c_smbus_interface *bus)
{
    if(bus->info.pci_vendor != INTEL_VEN)
    {
        return false;
    }

    const char *name;
    size_t idx = 0;

    while((name = RECOGNIZED_INTEL_I2C_BUS_NAMES[idx++]) != nullptr)
    {
        const char *pos = std::strstr(bus->info.device_name, name);

        if(pos == bus->info.device_name)
        {
            return true;
        }
    }

    return false;
}
