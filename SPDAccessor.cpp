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

#include "LogManager.h"
#include "SPDAccessor.h"
#include "filesystem.h"

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

SPDDetector::SPDDetector(i2c_smbus_interface *bus, uint8_t address, SPDMemoryType mem_type = SPD_RESERVED)
  : bus(bus), address(address), mem_type(mem_type), valid(false)
{
    detect_memory_type();
}

bool SPDDetector::is_valid() const
{
    return valid;
}

SPDMemoryType SPDDetector::memory_type() const
{
    return mem_type;
}

void SPDDetector::detect_memory_type()
{
    SPDAccessor *accessor;

    LOG_DEBUG("Probing DRAM on address 0x%02x", address);

#ifdef __linux__
    if(EE1004Accessor::isAvailable(bus, address))
    {
        accessor = new EE1004Accessor(bus, address);
    }
    else if(SPD5118Accessor::isAvailable(bus, address))
    {
        accessor = new SPD5118Accessor(bus, address);
    }
    else
#endif
    if((mem_type == SPD_RESERVED || mem_type == SPD_DDR4_SDRAM || mem_type == SPD_DDR4E_SDRAM ||
        mem_type == SPD_LPDDR4_SDRAM || mem_type == SPD_LPDDR4X_SDRAM) &&
       DDR4DirectAccessor::isAvailable(bus, address))
    {
        accessor = new DDR4DirectAccessor(bus, address);
    }
    else if((mem_type == SPD_RESERVED || mem_type == SPD_DDR5_SDRAM || mem_type == SPD_LPDDR5_SDRAM) &&
            DDR5DirectAccessor::isAvailable(bus, address))
    {
        accessor = new DDR5DirectAccessor(bus, address);
    }
    else if(mem_type == SPD_RESERVED)
    {
        // Probe the SPD directly - probably an older system than DDR4
        LOG_TRACE("Probing memory type older than DDR4");
        int value = bus->i2c_smbus_read_byte_data(address, 0x02);
        if(value < 0)
        {
            valid = false;
        }
        else
        {
            mem_type = (SPDMemoryType) value;
            valid = true;
        }
        return;
    }
    else
    {
        valid = false;
        return;
    }

    valid = true;
    mem_type = accessor->memory_type();
    delete accessor;
}

uint8_t SPDDetector::spd_address() const
{
    return this->address;
}

i2c_smbus_interface *SPDDetector::smbus() const
{
    return this->bus;
}

SPDWrapper::SPDWrapper(const SPDWrapper &wrapper)
{
    this->accessor = wrapper.accessor->copy();
    this->address = wrapper.address;
    this->mem_type = wrapper.mem_type;
}

SPDWrapper::SPDWrapper(const SPDDetector &detector)
{
    this->address = detector.spd_address();
    this->mem_type = detector.memory_type();

    // Allocate a new accessor
    this->accessor = SPDAccessor::for_memory_type(this->mem_type, detector.smbus(), this->address);
}

SPDWrapper::~SPDWrapper()
{
    delete accessor;
}

SPDMemoryType SPDWrapper::memory_type()
{
    return mem_type;
}

int SPDWrapper::index()
{
    return this->address - 0x50;
}

uint16_t SPDWrapper::jedec_id()
{
    if(accessor == nullptr)
    {
        return 0x0000;
    }
    return accessor->jedec_id();
}

/*-------------------------------------------------------------------------*\
| Helper functions for easier collection handling.                          |
\*-------------------------------------------------------------------------*/

bool is_jedec_in_slots(std::vector<SPDWrapper> &slots, uint16_t jedec_id)
{
    for(SPDWrapper &slot : slots)
    {
        if(slot.jedec_id() == jedec_id)
        {
            return true;
        }
    }
    return false;
}

std::vector<SPDWrapper*> slots_with_jedec(std::vector<SPDWrapper> &slots, uint16_t jedec_id)
{
    std::vector<SPDWrapper*> matching_slots;

    for(SPDWrapper &slot : slots)
    {
        if(slot.jedec_id() == jedec_id)
        {
            matching_slots.push_back(&slot);
        }
    }

    return matching_slots;
}

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

DDR4DirectAccessor::DDR4DirectAccessor(i2c_smbus_interface *bus, uint8_t spd_addr)
  : DDR4Accessor(bus, spd_addr)
{
}

DDR4DirectAccessor::~DDR4DirectAccessor()
{
}

bool DDR4DirectAccessor::isAvailable(i2c_smbus_interface *bus, uint8_t spd_addr)
{
    int value = bus->i2c_smbus_write_quick(0x36, 0x00);
    if(value < 0)
    {
        return false;
    }

    // Do page switch
    bus->i2c_smbus_write_byte_data(0x36, 0x00, 0xFF);
    std::this_thread::sleep_for(SPD_IO_DELAY);

    value = bus->i2c_smbus_read_byte_data(spd_addr, 0x00);
    return (value == 0x23);
}

SPDAccessor *DDR4DirectAccessor::copy()
{
    return new DDR4DirectAccessor(bus, address);
}

uint8_t DDR4DirectAccessor::at(uint16_t addr)
{
    if(addr >= SPD_DDR4_EEPROM_LENGTH)
    {
        //throw OutOfBoundsError(addr);
        return 0xFF;
    }
    set_page(addr >> SPD_DDR4_EEPROM_PAGE_SHIFT);
    uint8_t offset = (uint8_t)(addr & SPD_DDR4_EEPROM_PAGE_MASK);
    uint32_t value = bus->i2c_smbus_read_byte_data(address, offset);
    std::this_thread::sleep_for(SPD_IO_DELAY);
    return (uint8_t)value;
}

void DDR4DirectAccessor::set_page(uint8_t page)
{
    if(current_page != page)
    {
        bus->i2c_smbus_write_byte_data(0x36 + page, 0x00, 0xFF);
        current_page = page;
        std::this_thread::sleep_for(SPD_IO_DELAY);
    }
}

#ifdef __linux__
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
    int size = snprintf(nullptr, 0, SPD_EE1004_PATH, bus->port_id, spd_addr);
    char *path = new char[size+1];
    snprintf(path, size+1, SPD_EE1004_PATH, bus->port_id, spd_addr);
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
        readEeprom();
    }
    return dump[addr];
}

void EE1004Accessor::readEeprom()
{
    int size = snprintf(nullptr, 0, SPD_EE1004_PATH, bus->port_id, address);
    char *filename = new char[size+1];
    snprintf(filename, size+1, SPD_EE1004_PATH, bus->port_id, address);

    std::ifstream eeprom_file(filename, std::ios::in | std::ios::binary);
    if(eeprom_file)
    {
        eeprom_file.read((char*)dump, sizeof(dump));
        eeprom_file.close();
    }
    delete[] filename;
}
#endif

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
        int ddr5Magic = bus->i2c_smbus_read_byte_data(spd_addr, 0x00);
        int ddr5Sensor = bus->i2c_smbus_read_byte_data(spd_addr, 0x01);
        std::this_thread::sleep_for(SPD_IO_DELAY);

        if(ddr5Magic < 0 || ddr5Sensor < 0)
        {
            break;
        }

        LOG_TRACE("SPD Hub Magic: 0x%02x 0x%02x", ddr5Magic, ddr5Sensor);

        if(ddr5Magic == 0x51 && (ddr5Sensor & 0xEF) == 0x08)
        {
            return true;
        }

        int page = bus->i2c_smbus_read_byte_data(spd_addr, SPD_DDR5_MREG_VIRTUAL_PAGE);
        std::this_thread::sleep_for(SPD_IO_DELAY);

        LOG_TRACE("SPD Page: 0x%02x", page);
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
    if(addr >= SPD_DDR5_EEPROM_LENGTH)
    {
        //throw OutOfBoundsError(addr);
        return 0xFF;
    }
    set_page(addr >> SPD_DDR5_EEPROM_PAGE_SHIFT);
    uint8_t offset = (uint8_t)(addr & SPD_DDR5_EEPROM_PAGE_MASK) | 0x80;
    uint32_t value = bus->i2c_smbus_read_byte_data(address, offset);
    std::this_thread::sleep_for(SPD_IO_DELAY);
    return (uint8_t)value;
}

void DDR5DirectAccessor::set_page(uint8_t page)
{
    if(current_page != page)
    {
        bus->i2c_smbus_write_byte_data(address, SPD_DDR5_MREG_VIRTUAL_PAGE, page);
        current_page = page;
        std::this_thread::sleep_for(SPD_IO_DELAY);
    }
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
