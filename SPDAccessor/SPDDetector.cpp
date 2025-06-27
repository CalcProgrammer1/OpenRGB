/*---------------------------------------------------------*\
| SPDDetector.cpp                                           |
|                                                           |
|   Detector for DRAM modules using SPD information         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "DDR4DirectAccessor.h"
#include "DDR5DirectAccessor.h"
#include "LogManager.h"
#include "SPDDetector.h"

#ifdef __linux__
#include "EE1004Accessor_Linux.h"
#include "SPD5118Accessor_Linux.h"
#endif

SPDDetector::SPDDetector(i2c_smbus_interface *bus, uint8_t address, SPDMemoryType mem_type = SPD_RESERVED)
  : bus(bus), address(address), mem_type(mem_type), valid(false)
{
    detect_memory_type();
}

bool SPDDetector::is_valid() const
{
    return(valid);
}

SPDMemoryType SPDDetector::memory_type() const
{
    return(mem_type);
}

void SPDDetector::detect_memory_type()
{
    SPDAccessor *accessor;

    /*---------------------------------------------------------*\
    | On Linux, attempt to use the ee1004 or spd5118 drivers to |
    | access SPD on DDR4 and DDR5, respectively                 |
    \*---------------------------------------------------------*/
#ifdef __linux__
    if(EE1004Accessor::isAvailable(bus, address))
    {
        LOG_DEBUG("[SPDDetector] Probing DRAM using EE1004 Accessor on bus %d, address 0x%02x", bus->bus_id, address);
        accessor = new EE1004Accessor(bus, address);
    }
    else if(SPD5118Accessor::isAvailable(bus, address))
    {
        LOG_DEBUG("[SPDDetector] Probing DRAM using SPD5118 Accessor on bus %d, address 0x%02x", bus->bus_id, address);
        accessor = new SPD5118Accessor(bus, address);
    }
    else
#endif
    /*---------------------------------------------------------*\
    | Otherwise, access the SPD using a direct accessor using   |
    | i2c for DDR4 and DDR5                                     |
    \*---------------------------------------------------------*/
    if((mem_type == SPD_RESERVED
     || mem_type == SPD_DDR4_SDRAM
     || mem_type == SPD_DDR4E_SDRAM
     || mem_type == SPD_LPDDR4_SDRAM
     || mem_type == SPD_LPDDR4X_SDRAM)
    &&  DDR4DirectAccessor::isAvailable(bus, address))
    {
        LOG_DEBUG("[SPDDetector] Probing DRAM using DDR4 Direct Accessor on bus %d, address 0x%02x", bus->bus_id, address);
        accessor = new DDR4DirectAccessor(bus, address);
    }
    else if((mem_type == SPD_RESERVED
          || mem_type == SPD_DDR5_SDRAM
          || mem_type == SPD_LPDDR5_SDRAM)
         &&  DDR5DirectAccessor::isAvailable(bus, address))
    {
        LOG_DEBUG("[SPDDetector] Probing DRAM using DDR5 Direct Accessor on bus %d, address 0x%02x", bus->bus_id, address);
        accessor = new DDR5DirectAccessor(bus, address);
    }
    /*---------------------------------------------------------*\
    | Otherwise, probe the SPD directly using i2c, probably an  |
    | older system than DDR4                                    |
    \*---------------------------------------------------------*/
    else if(mem_type == SPD_RESERVED)
    {
        LOG_DEBUG("[SPDDetector] Probing DRAM older than DDR4 on bus %d, address 0x%02x", bus->bus_id, address);

        int value = bus->i2c_smbus_read_byte_data(address, 0x02);

        if(value < 0)
        {
            valid = false;
        }
        else
        {
            mem_type = (SPDMemoryType)value;

            /*-------------------------------------------------*\
            | We are only interested in DDR4 and DDR5 systems   |
            \*-------------------------------------------------*/
            valid = (mem_type == SPD_DDR4_SDRAM
                  || mem_type == SPD_DDR4E_SDRAM
                  || mem_type == SPD_LPDDR4_SDRAM
                  || mem_type == SPD_LPDDR4X_SDRAM
                  || mem_type == SPD_DDR5_SDRAM
                  || mem_type == SPD_LPDDR5_SDRAM);
        }

        return;
    }
    /*---------------------------------------------------------*\
    | If memory type could not be determined, detection failed  |
    \*---------------------------------------------------------*/
    else
    {
        LOG_DEBUG("[SPDDetector] Memory type could not be determined for bus %d, address 0x%02x", bus->bus_id, address);
        valid = false;
        return;
    }

    /*---------------------------------------------------------*\
    | If an accessor was created, save the memory type          |
    \*---------------------------------------------------------*/
    valid = true;
    mem_type = accessor->memory_type();

    /*---------------------------------------------------------*\
    | Delete the accessor                                       |
    \*---------------------------------------------------------*/
    delete accessor;
}

uint8_t SPDDetector::spd_address() const
{
    return(this->address);
}

i2c_smbus_interface *SPDDetector::smbus() const
{
    return(this->bus);
}
