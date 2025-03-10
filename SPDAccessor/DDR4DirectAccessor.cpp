/*---------------------------------------------------------*\
| DDR4DirectAccessor.cpp                                    |
|                                                           |
|   DDR4 SPD accessor implementation using direct i2c       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <chrono>
#include "DDR4DirectAccessor.h"

using namespace std::chrono_literals;

DDR4DirectAccessor::DDR4DirectAccessor(i2c_smbus_interface *bus, uint8_t spd_addr)
  : DDR4Accessor(bus, spd_addr)
{
}

DDR4DirectAccessor::~DDR4DirectAccessor()
{
}

bool DDR4DirectAccessor::isAvailable(i2c_smbus_interface *bus, uint8_t spd_addr)
{
    /*-----------------------------------------------------*\
    | Perform i2c quick transfer to test if i2c address     |
    | responds                                              |
    \*-----------------------------------------------------*/
    int value = bus->i2c_smbus_write_quick(0x36, 0x00);
    if(value < 0)
    {
        return false;
    }

    /*-----------------------------------------------------*\
    | Select low page                                       |
    \*-----------------------------------------------------*/
    bus->i2c_smbus_write_byte_data(0x36, 0x00, 0xFF);

    std::this_thread::sleep_for(SPD_IO_DELAY);

    /*-----------------------------------------------------*\
    | Read value at address 0 in SPD device                 |
    \*-----------------------------------------------------*/
    value = bus->i2c_smbus_read_byte_data(spd_addr, 0x00);

    /*-----------------------------------------------------*\
    | DDR4 is available if value is 0x23                    |
    \*-----------------------------------------------------*/
    return(value == 0x23);
}

SPDAccessor *DDR4DirectAccessor::copy()
{
    return new DDR4DirectAccessor(bus, address);
}

uint8_t DDR4DirectAccessor::at(uint16_t addr)
{
    /*-----------------------------------------------------*\
    | Ensure address is valid                               |
    \*-----------------------------------------------------*/
    if(addr >= SPD_DDR4_EEPROM_LENGTH)
    {
        return 0xFF;
    }

    /*-----------------------------------------------------*\
    | Switch to the page containing address                 |
    \*-----------------------------------------------------*/
    set_page(addr >> SPD_DDR4_EEPROM_PAGE_SHIFT);

    /*-----------------------------------------------------*\
    | Calculate offset                                      |
    \*-----------------------------------------------------*/
    uint8_t offset = (uint8_t)(addr & SPD_DDR4_EEPROM_PAGE_MASK);

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

void DDR4DirectAccessor::set_page(uint8_t page)
{
    /*-----------------------------------------------------*\
    | Switch page if not already active                     |
    \*-----------------------------------------------------*/
    if(current_page != page)
    {
        bus->i2c_smbus_write_byte_data(0x36 + page, 0x00, 0xFF);
        current_page = page;

        std::this_thread::sleep_for(SPD_IO_DELAY);
    }
}
