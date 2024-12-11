/*---------------------------------------------------------*\
| EE1004Accessor_Linux.h                                    |
|                                                           |
|   SPD accessor implementation using e1004 driver on Linux |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "SPDAccessor.h"

class EE1004Accessor : public DDR4Accessor
{
  public:
    EE1004Accessor(i2c_smbus_interface *bus, uint8_t address);
    virtual ~EE1004Accessor();

    static bool isAvailable(i2c_smbus_interface *bus, uint8_t address);

    virtual SPDAccessor *copy();
    virtual uint8_t at(uint16_t addr);

  private:
    static const char *SPD_EE1004_PATH;

    uint8_t dump[512];
    bool valid;

    void readEEPROM();
};
