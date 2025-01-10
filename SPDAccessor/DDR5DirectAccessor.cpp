/*---------------------------------------------------------*\
| DDR5DirectAccessor.cpp                                    |
|                                                           |
|   DDR5 SPD accessor implementation using direct i2c       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "DDR5DirectAccessor.h"
#include "LogManager.h"

using namespace std::chrono_literals;

DDR5DirectAccessor::DDR5DirectAccessor(i2c_smbus_interface *bus, uint8_t spd_addr)
  : DDR5Accessor(bus, spd_addr)
{
}

DDR5DirectAccessor::~DDR5DirectAccessor()
{
}

bool DDR5DirectAccessor::isAvailable(i2c_smbus_interface *bus, uint8_t spd_addr)
{
    bool retry = true;

    while(true)
    {
        std::this_thread::sleep_for(SPD_IO_DELAY);
        int ddr5Magic = bus->i2c_smbus_read_byte_data(spd_addr, 0x00);
        std::this_thread::sleep_for(SPD_IO_DELAY);
        int ddr5Sensor = bus->i2c_smbus_read_byte_data(spd_addr, 0x01);
        std::this_thread::sleep_for(SPD_IO_DELAY);

        if(ddr5Magic < 0 || ddr5Sensor < 0)
        {
            break;
        }

        LOG_TRACE("[DDR5DirectAccessor] SPD Hub Magic: 0x%02x 0x%02x", ddr5Magic, ddr5Sensor);

        if(ddr5Magic == 0x51 && (ddr5Sensor & 0xEF) == 0x08)
        {
            return true;
        }

        int page = bus->i2c_smbus_read_byte_data(spd_addr, SPD_DDR5_MREG_VIRTUAL_PAGE);
        std::this_thread::sleep_for(SPD_IO_DELAY);

        LOG_TRACE("[DDR5DirectAccessor] SPD Page: 0x%02x", page);
        if(page < 0)
        {
            break;
        }
        else if(retry && page > 0 && page < (SPD_DDR5_EEPROM_LENGTH >> SPD_DDR5_EEPROM_PAGE_SHIFT))
        {
            // This still might be a DDR5 module, just the page is off
            bus->i2c_smbus_write_byte_data(spd_addr, SPD_DDR5_MREG_VIRTUAL_PAGE, 0);
            std::this_thread::sleep_for(SPD_IO_DELAY);
            retry = false;
        }
        else
        {
            break;
        }
    }
    return false;
}

SPDAccessor *DDR5DirectAccessor::copy()
{
    DDR5DirectAccessor *access = new DDR5DirectAccessor(bus, address);
    access->current_page = this->current_page;
    return access;
}

uint8_t DDR5DirectAccessor::at(uint16_t addr)
{
    /*-----------------------------------------------------*\
    | Ensure address is valid                               |
    \*-----------------------------------------------------*/
    if(addr >= SPD_DDR5_EEPROM_LENGTH)
    {
        return 0xFF;
    }

    /*-----------------------------------------------------*\
    | Switch to the page containing address                 |
    \*-----------------------------------------------------*/
    set_page(addr >> SPD_DDR5_EEPROM_PAGE_SHIFT);

    /*-----------------------------------------------------*\
    | Calculate offset                                      |
    \*-----------------------------------------------------*/
    uint8_t offset = (uint8_t)(addr & SPD_DDR5_EEPROM_PAGE_MASK) | 0x80;

    /*-----------------------------------------------------*\
    | Read value at address                                 |
    \*-----------------------------------------------------*/
    uint32_t value = bus->i2c_smbus_read_byte_data(address, offset);

    std::this_thread::sleep_for(SPD_IO_DELAY);

    /*-----------------------------------------------------*\
    | Return value                                          |
    \*-----------------------------------------------------*/
    return((uint8_t)value);
}

void DDR5DirectAccessor::set_page(uint8_t page)
{
    /*-----------------------------------------------------*\
    | Switch page if not already active                     |
    \*-----------------------------------------------------*/
    if(current_page != page)
    {
        bus->i2c_smbus_write_byte_data(address, SPD_DDR5_MREG_VIRTUAL_PAGE, page);
        current_page = page;
        std::this_thread::sleep_for(SPD_IO_DELAY);
    }
}
