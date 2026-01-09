/*---------------------------------------------------------*\
| CorsairDRAMControllerDetect.cpp                           |
|                                                           |
|   Detector for Corsair DRAM RGB controllers               |
|                                                           |
|   Adam Honse (CalcProgrammer1)                30 Jun 2019 |
|   Erik Gilling (konkers)                      25 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <vector>
#include "CorsairDRAMController.h"
#include "DetectionManager.h"
#include "i2c_smbus.h"
#include "LogManager.h"
#include "pci_ids.h"
#include "RGBController_CorsairDRAM.h"

using namespace std::chrono_literals;

#define CORSAIR_DRAM_NAME "Corsair DRAM"

bool TestForCorsairDRAMController(i2c_smbus_interface *bus, unsigned char address)
{
    int res = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);

    LOG_DEBUG("[%s] Trying address %02X", CORSAIR_DRAM_NAME, address);

    if(res < 0)
    {
        LOG_DEBUG("[%s] Failed: res was %04X", CORSAIR_DRAM_NAME, res);
        return false;
    }

    res = bus->i2c_smbus_read_byte_data(address, 0x43);

    if(!(res == 0x1A || res == 0x1B || res == 0x1C))
    {
        LOG_DEBUG("[%s] Failed: expected 0x1A, 0x1B, or 0x1C, got %04X", CORSAIR_DRAM_NAME, res);
        return false;
    }

    res = bus->i2c_smbus_read_byte_data(address, 0x44);

    if(!(res == 0x03 || res == 0x04))
    {
        LOG_DEBUG("[%s] Failed: expected 0x03 or 0x04, got %04X", CORSAIR_DRAM_NAME, res);
        return false;
    }

    return true;
}

void DetectCorsairDRAMControllers(std::vector<i2c_smbus_interface *> &busses)
{
    for(unsigned int bus = 0; bus < busses.size(); bus++)
    {
        IF_DRAM_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
        {
            LOG_DEBUG("[%s] Testing bus %d", CORSAIR_DRAM_NAME, bus);

            std::vector<unsigned char> addresses;

            for(unsigned char addr = 0x58; addr <= 0x5F; addr++)
            {
                addresses.push_back(addr);
            }

            for(unsigned char addr = 0x18; addr <= 0x1F; addr++)
            {
                addresses.push_back(addr);
            }

            for(unsigned char addr : addresses)
            {
                if(TestForCorsairDRAMController(busses[bus], addr))
                {
                    CorsairDRAMController*     controller     = new CorsairDRAMController(busses[bus], addr);
                    RGBController_CorsairDRAM* rgb_controller = new RGBController_CorsairDRAM(controller);

                    DetectionManager::get()->RegisterRGBController(rgb_controller);
                }

                std::this_thread::sleep_for(10ms);
            }
        }
    }
}

REGISTER_I2C_DETECTOR(CORSAIR_DRAM_NAME, DetectCorsairDRAMControllers);
