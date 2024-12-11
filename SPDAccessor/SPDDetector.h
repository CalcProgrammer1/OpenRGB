/*---------------------------------------------------------*\
| SPDDetector.h                                             |
|                                                           |
|   Detector for DRAM modules using SPD information         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "SPDAccessor.h"
#include "SPDCommon.h"

class SPDDetector
{
  public:
    SPDDetector(i2c_smbus_interface *bus, uint8_t address, SPDMemoryType mem_type);

    bool is_valid() const;
    SPDMemoryType memory_type() const;
    uint8_t spd_address() const;
    i2c_smbus_interface *smbus() const;

  private:
    i2c_smbus_interface *bus;
    uint8_t address;
    SPDMemoryType mem_type;
    bool valid;

    void detect_memory_type();
};
