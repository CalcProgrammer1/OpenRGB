/*---------------------------------------------------------*\
| SPDAccessor.cpp                                           |
|                                                           |
|   Access to SPD information on various DIMMs              |
|                                                           |
|   Milan Cermak (krysmanta)                    09 Nov 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "DDR4DirectAccessor.h"
#include "DDR5DirectAccessor.h"
#include "EE1004Accessor_Linux.h"
#include "LogManager.h"
#include "SPDAccessor.h"

using namespace std::chrono_literals;

const char *spd_memory_type_name[] =
{
    "Reserved",
    "FPM",
    "EDO",
    "Nibble",
    "SDR",
    "Multiplex ROM",
    "DDR",
    "DDR",
    "DDR2",
    "FB",
    "FB Probe",
    "DDR3",
    "DDR4",
    "Reserved",
    "DDR4e",
    "LPDDR3",
    "LPDDR4",
    "LPDDR4X",
    "DDR5",
    "LPDDR5"
};

/*-------------------------------------------------------------------------*\
| Internal implementation for specific memory type.                         |
\*-------------------------------------------------------------------------*/

SPDAccessor::SPDAccessor(i2c_smbus_interface *bus, uint8_t spd_addr)
{
    this->bus = bus;
    this->address = spd_addr;
}

SPDAccessor::~SPDAccessor()
{
}

SPDAccessor *SPDAccessor::for_memory_type(SPDMemoryType type, i2c_smbus_interface *bus, uint8_t spd_addr)
{
    if(type == SPD_DDR4_SDRAM)
    {
#ifdef __linux__
        if(EE1004Accessor::isAvailable(bus, spd_addr))
        {
            return new EE1004Accessor(bus, spd_addr);
        }
#endif
        return new DDR4DirectAccessor(bus, spd_addr);
    }
    if(type == SPD_DDR5_SDRAM)
    {
#ifdef __linux__
        if(SPD5118Accessor::isAvailable(bus, spd_addr))
        {
            return new SPD5118Accessor(bus, spd_addr);
        }
#endif
        return new DDR5DirectAccessor(bus, spd_addr);
    }

    return nullptr;
};

DDR4Accessor::DDR4Accessor(i2c_smbus_interface *bus, uint8_t spd_addr)
  : SPDAccessor(bus, spd_addr)
{
}

DDR4Accessor::~DDR4Accessor()
{
}

SPDMemoryType DDR4Accessor::memory_type()
{
    return (SPDMemoryType)(this->at(0x02));
}

uint16_t DDR4Accessor::jedec_id()
{
    return (this->at(0x140) << 8) + (this->at(0x141) & 0x7f) - 1;
}

DDR5Accessor::DDR5Accessor(i2c_smbus_interface *bus, uint8_t spd_addr)
  : SPDAccessor(bus, spd_addr)
{
}

DDR5Accessor::~DDR5Accessor()
{
}

SPDMemoryType DDR5Accessor::memory_type()
{
    return (SPDMemoryType)(this->at(0x02));
}

uint16_t DDR5Accessor::jedec_id()
{
    return (this->at(0x200) << 8) + (this->at(0x201) & 0x7f) - 1;
}


#ifdef __linux__
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
    int size = snprintf(nullptr, 0, SPD_SPD5118_PATH, bus->port_id, spd_addr);
    char *path = new char[size+1];
    snprintf(path, size+1, SPD_SPD5118_PATH, bus->port_id, spd_addr);
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
        readEeprom();
    }
    return dump[addr];
}

void SPD5118Accessor::readEeprom()
{
    int size = snprintf(nullptr, 0, SPD_SPD5118_PATH, bus->port_id, address);
    char *filename = new char[size+1];
    snprintf(filename, size+1, SPD_SPD5118_PATH, bus->port_id, address);

    std::ifstream eeprom_file(filename, std::ios::in | std::ios::binary);
    if(eeprom_file)
    {
        eeprom_file.read((char*)dump, sizeof(dump));
        eeprom_file.close();
    }
    delete[] filename;
}
#endif
