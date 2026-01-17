/*---------------------------------------------------------*\
| SPDAccessor.cpp                                           |
|                                                           |
|   Access to SPD information on various DIMMs              |
|                                                           |
|   Milan Cermak (krysmanta)                    09 Nov 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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


// Sources for define values:
// - https://en.wikipedia.org/wiki/Serial_presence_detect
// - JEDEC DDR5 Serial Presence Detect (SPD): Table of contents

#define BASIC_MEMORY_TYPE_ADDR      (0x02)

#define DDR4_JEDEC_ID_ADDR          (0x140)
#define DDR4_PART_NR_START          (0x149)
#define DDR4_PART_NR_END            (0x15C)
#define DDR4_PART_NR_LEN            (DDR4_PART_NR_END - DDR4_PART_NR_START + 1)
#define DDR4_MANUF_SPECIFIC_START   (0x161)
#define DDR4_MANUF_SPECIFIC_END     (0x17D)
#define DDR4_MANUF_SPECIFIC_LEN     (DDR4_MANUF_SPECIFIC_END - DDR4_MANUF_SPECIFIC_START + 1)

#define DDR5_JEDEC_ID_ADDR          (0x200)
#define DDR5_PART_NR_START          (0x209)
#define DDR5_PART_NR_END            (0x226)
#define DDR5_PART_NR_LEN            (DDR5_PART_NR_END - DDR5_PART_NR_START + 1)
#define DDR5_MANUF_SPECIFIC_START   (0x22B)
#define DDR5_MANUF_SPECIFIC_END     (0x27F)
#define DDR5_MANUF_SPECIFIC_LEN     (DDR5_MANUF_SPECIFIC_END - DDR5_MANUF_SPECIFIC_START + 1)

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

std::string SPDAccessor::read_part_nr_at(uint16_t address, std::size_t len)
{
    std::string part_number;

    for(std::size_t i = 0; i < len; i++)
    {
        std::size_t spd_addr = address + i;
        part_number += (char)this->at(spd_addr);
    }

    // Find the true end of string & truncate it to that point.
    // Part number should be padded with 0x20 (space) for DDR4 (Source: Wikipedia)
    // It may be padded with 0x00 (Source: real-life tests on DDR5 memory)
    // Note: To prevent infinite loop, end_of_string_idx MUST be signed.
    int end_of_string_idx = part_number.length()-1;
    for(; end_of_string_idx >= 0; end_of_string_idx--)
    {
        if(
            part_number[end_of_string_idx] != '\0' &&
            part_number[end_of_string_idx] != ' '
        )
        {
            break;
        }
    }
    part_number = part_number.substr(0, end_of_string_idx + 1);

    return part_number;
}

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
    return((SPDMemoryType)(this->at(BASIC_MEMORY_TYPE_ADDR)));
}

uint16_t DDR4Accessor::jedec_id()
{
    return((this->at(DDR4_JEDEC_ID_ADDR) << 8) + (this->at(DDR4_JEDEC_ID_ADDR+1) & 0x7f) - 1);
}

std::string DDR4Accessor::part_number()
{
    return this->read_part_nr_at(DDR4_PART_NR_START, DDR4_PART_NR_LEN);
}

uint8_t DDR4Accessor::manufacturer_data(uint16_t index)
{
    if(index > DDR4_MANUF_SPECIFIC_LEN-1)
    {
        return 0;
    }
    return this->at(DDR4_MANUF_SPECIFIC_START + index);
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
    return((SPDMemoryType)(this->at(BASIC_MEMORY_TYPE_ADDR)));
}

uint16_t DDR5Accessor::jedec_id()
{
    return((this->at(DDR5_JEDEC_ID_ADDR) << 8) + (this->at(DDR5_JEDEC_ID_ADDR+1) & 0x7f) - 1);
}

std::string DDR5Accessor::part_number()
{
    return this->read_part_nr_at(DDR5_PART_NR_START, DDR5_PART_NR_LEN);
}

uint8_t DDR5Accessor::manufacturer_data(uint16_t index)
{
    if(index > DDR5_MANUF_SPECIFIC_LEN-1)
    {
        return 0;
    }
    return this->at(DDR5_MANUF_SPECIFIC_START + index);
}
