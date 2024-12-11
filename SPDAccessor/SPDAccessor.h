/*---------------------------------------------------------*\
| SPDAccessor.h                                             |
|                                                           |
|   Access to SPD information on various DIMMs              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "SPDCommon.h"

class SPDAccessor
{
  public:
    SPDAccessor(i2c_smbus_interface *bus, uint8_t address);
    virtual ~SPDAccessor();

    static SPDAccessor *for_memory_type(SPDMemoryType type, i2c_smbus_interface *bus, uint8_t address);

    virtual SPDMemoryType memory_type() = 0;
    virtual uint16_t jedec_id() = 0;

    virtual SPDAccessor *copy() = 0;

    virtual uint8_t at(uint16_t addr) = 0;

  protected:
    i2c_smbus_interface *bus;
    uint8_t address;
};

/*-------------------------------------------------------------------------*\
| Internal implementation for specific memory type.                         |
\*-------------------------------------------------------------------------*/

class DDR4Accessor : public SPDAccessor
{
  public:
    DDR4Accessor(i2c_smbus_interface *bus, uint8_t address);
    virtual ~DDR4Accessor();
    virtual SPDMemoryType memory_type();
    virtual uint16_t jedec_id();
};



class DDR5Accessor : public SPDAccessor
{
  public:
    DDR5Accessor(i2c_smbus_interface *bus, uint8_t address);
    virtual ~DDR5Accessor();
    virtual SPDMemoryType memory_type();
    virtual uint16_t jedec_id();
};

class DDR5DirectAccessor : public DDR5Accessor
{
  public:
    DDR5DirectAccessor(i2c_smbus_interface *bus, uint8_t address);
    virtual ~DDR5DirectAccessor();

    static bool isAvailable(i2c_smbus_interface *bus, uint8_t address);

    virtual SPDAccessor *copy();
    virtual uint8_t at(uint16_t addr);

  private:
    uint8_t current_page = 0xFF;
    static const uint16_t SPD_DDR5_EEPROM_LENGTH = 2048;
    static const uint8_t SPD_DDR5_EEPROM_PAGE_SHIFT = 7;
    static const uint8_t SPD_DDR5_EEPROM_PAGE_MASK = 0x7F;
    static const uint8_t SPD_DDR5_MREG_VIRTUAL_PAGE = 0x0B;

    void set_page(uint8_t page);
};

#ifdef __linux__
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

    void readEeprom();
};
#endif
