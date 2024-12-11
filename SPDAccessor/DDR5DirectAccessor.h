/*---------------------------------------------------------*\
| DDR5DirectAccessor.h                                      |
|                                                           |
|   DDR5 SPD accessor implementation using direct i2c       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "SPDAccessor.h"

class DDR5DirectAccessor : public DDR5Accessor
{
  public:
    DDR5DirectAccessor(i2c_smbus_interface *bus, uint8_t address);
    virtual ~DDR5DirectAccessor();

    static bool isAvailable(i2c_smbus_interface *bus, uint8_t address);

    virtual SPDAccessor *   copy();
    virtual uint8_t         at(uint16_t addr);

  private:
    uint8_t                 current_page                = 0xFF;
    static const uint16_t   SPD_DDR5_EEPROM_LENGTH      = 2048;
    static const uint8_t    SPD_DDR5_EEPROM_PAGE_SHIFT  = 7;
    static const uint8_t    SPD_DDR5_EEPROM_PAGE_MASK   = 0x7F;
    static const uint8_t    SPD_DDR5_MREG_VIRTUAL_PAGE  = 0x0B;

    void set_page(uint8_t page);
};
