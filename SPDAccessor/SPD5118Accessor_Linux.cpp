/*---------------------------------------------------------*\
| SPD5118Accessor_Linux.cpp                                 |
|                                                           |
|   DDR5 SPD accessor implementation using spd5118 driver   |
|   on Linux                                                |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include <fstream>
#include "SPD5118Accessor_Linux.h"
#include "filesystem.h"

const char *SPD5118Accessor::SPD_SPD5118_PATH = "/sys/bus/i2c/drivers/spd5118/%u-%04x/eeprom";

SPD5118Accessor::SPD5118Accessor(i2c_smbus_interface *bus, uint8_t spd_addr)
  : DDR5Accessor(bus, spd_addr), valid(false)
{
}

SPD5118Accessor::~SPD5118Accessor()
{
}

bool SPD5118Accessor::isAvailable(i2c_smbus_interface *bus, uint8_t spd_addr)
{
    int size = snprintf(nullptr, 0, SPD_SPD5118_PATH, bus->bus_id, spd_addr);
    char *path = new char[size+1];
    snprintf(path, size+1, SPD_SPD5118_PATH, bus->bus_id, spd_addr);
    bool result = std::filesystem::exists(path);
    delete[] path;
    return result;
}

SPDAccessor *SPD5118Accessor::copy()
{
    SPD5118Accessor *access = new SPD5118Accessor(bus, address);
    memcpy(access->dump, this->dump, sizeof(this->dump));
    access->valid = this->valid;
    return access;
}

uint8_t SPD5118Accessor::at(uint16_t addr)
{
    if(!valid)
    {
        readEEPROM();
    }
    return dump[addr];
}

void SPD5118Accessor::readEEPROM()
{
    int size = snprintf(nullptr, 0, SPD_SPD5118_PATH, bus->bus_id, address);
    char *filename = new char[size+1];
    snprintf(filename, size+1, SPD_SPD5118_PATH, bus->bus_id, address);

    std::ifstream eeprom_file(filename, std::ios::in | std::ios::binary);
    if(eeprom_file)
    {
        eeprom_file.read((char*)dump, sizeof(dump));
        eeprom_file.close();
    }
    delete[] filename;
}
