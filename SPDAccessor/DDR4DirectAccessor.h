/*---------------------------------------------------------*\
| DDR4DirectAccessor.h                                      |
|                                                           |
|   DDR4 SPD accessor implementation using direct i2c       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "SPDAccessor.h"

class DDR4DirectAccessor : public DDR4Accessor
{
  public:
    DDR4DirectAccessor(i2c_smbus_interface *bus, uint8_t address);
    virtual ~DDR4DirectAccessor();

    static bool isAvailable(i2c_smbus_interface *bus, uint8_t address);

    virtual SPDAccessor *   copy();
    virtual uint8_t         at(uint16_t addr);

  private:
    uint8_t                 current_page                = 0xFF;
    static const uint16_t   SPD_DDR4_EEPROM_LENGTH      = 512;
    static const uint8_t    SPD_DDR4_EEPROM_PAGE_SHIFT  = 8;
    static const uint8_t    SPD_DDR4_EEPROM_PAGE_MASK   = 0xFF;

    void set_page(uint8_t page);
};
