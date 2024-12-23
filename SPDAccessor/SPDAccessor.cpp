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
#include "LogManager.h"
#include "SPDAccessor.h"

#ifdef __linux__
#include "EE1004Accessor_Linux.h"
#include "SPD5118Accessor_Linux.h"
#endif

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

SPDAccessor::SPDAccessor(i2c_smbus_interface *bus, uint8_t spd_addr)
{
    this->bus       = bus;
    this->address   = spd_addr;
}

SPDAccessor::~SPDAccessor()
{
}

SPDAccessor *SPDAccessor::for_memory_type(SPDMemoryType type, i2c_smbus_interface *bus, uint8_t spd_addr)
{
    /*-----------------------------------------------------*\
    | DDR4 can use DDR4DirectAccessor or EE1004Accessor     |
    \*-----------------------------------------------------*/
    if(type == SPD_DDR4_SDRAM)
    {
#ifdef __linux__
        if(EE1004Accessor::isAvailable(bus, spd_addr))
        {
            return(new EE1004Accessor(bus, spd_addr));
        }
#endif
        return(new DDR4DirectAccessor(bus, spd_addr));
    }

    /*-----------------------------------------------------*\
    | DDR5 can use DDR5DirectAccessor or SPD5118Accessor    |
    \*-----------------------------------------------------*/
    if(type == SPD_DDR5_SDRAM)
    {
#ifdef __linux__
        if(SPD5118Accessor::isAvailable(bus, spd_addr))
        {
            return(new SPD5118Accessor(bus, spd_addr));
        }
#endif
        return(new DDR5DirectAccessor(bus, spd_addr));
    }

    return(nullptr);
};

/*---------------------------------------------------------*\
| Internal implementation for specific memory type.         |
\*---------------------------------------------------------*/

DDR4Accessor::DDR4Accessor(i2c_smbus_interface *bus, uint8_t spd_addr)
  : SPDAccessor(bus, spd_addr)
{
}

DDR4Accessor::~DDR4Accessor()
{
}

SPDMemoryType DDR4Accessor::memory_type()
{
    return((SPDMemoryType)(this->at(0x02)));
}

uint16_t DDR4Accessor::jedec_id()
{
    return((this->at(0x140) << 8) + (this->at(0x141) & 0x7f) - 1);
}

uint8_t DDR4Accessor::manufacturer_data(uint16_t index)
{
    if(index > 28)
    {
        return 0;
    }
    return this->at(0x161 + index);
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
    return((SPDMemoryType)(this->at(0x02)));
}

uint16_t DDR5Accessor::jedec_id()
{
    return((this->at(0x200) << 8) + (this->at(0x201) & 0x7f) - 1);
}

uint8_t DDR5Accessor::manufacturer_data(uint16_t index)
{
    if(index > 84)
    {
        return 0;
    }
    return this->at(0x22B + index);
}
