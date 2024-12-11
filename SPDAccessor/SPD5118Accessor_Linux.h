/*---------------------------------------------------------*\
| SPD5118Accessor_Linux.h                                   |
|                                                           |
|   DDR5 SPD accessor implementation using spd5118 driver   |
|   on Linux                                                |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "SPDAccessor.h"

class SPD5118Accessor : public DDR5Accessor
{
  public:
    SPD5118Accessor(i2c_smbus_interface *bus, uint8_t address);
    virtual ~SPD5118Accessor();

    static bool isAvailable(i2c_smbus_interface *bus, uint8_t address);

    virtual SPDAccessor *copy();
    virtual uint8_t at(uint16_t addr);

  private:
    static const char *SPD_SPD5118_PATH;

    uint8_t dump[2048];
    bool valid;

    void readEEPROM();
};
