/*---------------------------------------------------------*\
| EE1004Accessor_Linux.cpp                                  |
|                                                           |
|   SPD accessor implementation using e1004 driver on Linux |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include <fstream>
#include "EE1004Accessor_Linux.h"
#include "filesystem.h"

const char *EE1004Accessor::SPD_EE1004_PATH = "/sys/bus/i2c/drivers/ee1004/%u-%04x/eeprom";

EE1004Accessor::EE1004Accessor(i2c_smbus_interface *bus, uint8_t spd_addr)
  : DDR4Accessor(bus, spd_addr), valid(false)
{
}

EE1004Accessor::~EE1004Accessor()
{
}

bool EE1004Accessor::isAvailable(i2c_smbus_interface *bus, uint8_t spd_addr)
{
    int size = snprintf(nullptr, 0, SPD_EE1004_PATH, bus->bus_id, spd_addr);
    char *path = new char[size+1];
    snprintf(path, size+1, SPD_EE1004_PATH, bus->bus_id, spd_addr);
    bool result = std::filesystem::exists(path);
    delete[] path;
    return result;
}

SPDAccessor *EE1004Accessor::copy()
{
    EE1004Accessor *access = new EE1004Accessor(bus, address);
    memcpy(access->dump, this->dump, sizeof(this->dump));
    access->valid = this->valid;
    return access;
}

uint8_t EE1004Accessor::at(uint16_t addr)
{
    if(!valid)
    {
        readEEPROM();
    }
    return dump[addr];
}

void EE1004Accessor::readEEPROM()
{
    int size = snprintf(nullptr, 0, SPD_EE1004_PATH, bus->bus_id, address);
    char *filename = new char[size+1];
    snprintf(filename, size+1, SPD_EE1004_PATH, bus->bus_id, address);

    std::ifstream eeprom_file(filename, std::ios::in | std::ios::binary);
    if(eeprom_file)
    {
        eeprom_file.read((char*)dump, sizeof(dump));
        eeprom_file.close();
    }
    delete[] filename;
}
