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
    virtual uint8_t manufacturer_data(uint16_t index) = 0;

    virtual SPDAccessor *copy() = 0;

    virtual uint8_t at(uint16_t addr) = 0;

  protected:
    i2c_smbus_interface *bus;
    uint8_t address;
};

/*---------------------------------------------------------*\
| Internal implementation for specific memory type.         |
\*---------------------------------------------------------*/

class DDR4Accessor : public SPDAccessor
{
  public:
    DDR4Accessor(i2c_smbus_interface *bus, uint8_t address);
    virtual ~DDR4Accessor();
    virtual SPDMemoryType memory_type();
    virtual uint16_t jedec_id();
    virtual uint8_t manufacturer_data(uint16_t index);
};

class DDR5Accessor : public SPDAccessor
{
  public:
    DDR5Accessor(i2c_smbus_interface *bus, uint8_t address);
    virtual ~DDR5Accessor();
    virtual SPDMemoryType memory_type();
    virtual uint16_t jedec_id();
    virtual uint8_t manufacturer_data(uint16_t index);
};
