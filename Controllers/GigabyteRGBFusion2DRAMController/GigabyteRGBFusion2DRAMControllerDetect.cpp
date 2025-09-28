/*---------------------------------------------------------*\
| GigabyteRGBFusion2DRAMControllerDetect.cpp                |
|                                                           |
|   Detector for Gigabyte Aorus RGB Fusion 2 RAM            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                07 Jun 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <string>
#include <vector>
#include "Detector.h"
#include "LogManager.h"
#include "GigabyteRGBFusion2DRAMController.h"
#include "RGBController_GigabyteRGBFusion2DRAM.h"
#include "i2c_smbus.h"
#include "pci_ids.h"

/******************************************************************************************\
*                                                                                          *
*   TestForGigabyteRGBFusion2DRAMController                                                *
*                                                                                          *
*       Tests the given address to see if an RGB 2 Fusion DRAMcontroller exists there.     *
*       First does a quick write to test for a response                                    *
*                                                                                          *
\******************************************************************************************/

bool TestForGigabyteRGBFusion2DRAMController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    int res = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);

    if(res >= 0)
    {
        bus->i2c_smbus_write_byte_data(address, 0xE1, 0x01);

        res = bus->i2c_smbus_read_word_data(address, 0xED);

        LOG_TRACE("[Gigabyte RGB Fusion 2 DRAM] Read from 0xED: 0x%04X", res);

        if(res == 0x3282)
        {
            res = bus->i2c_smbus_read_word_data(address, 0xEB);

            LOG_TRACE("[Gigabyte RGB Fusion 2 DRAM] Read from 0xEB: 0x%04X", res);

            if(res == 0x0800)
            {
                pass = true;
            }
        }
    }

    return(pass);

}   /* TestForGigabyteRGBFusion2DRAMController() */

/***********************************************************************************************\
*                                                                                               *
*   DetectGigabyteRGBFusion2DRAMControllers                                                     *
*                                                                                               *
*       Detect Gigabyte RGB Fusion 2 controllers on the enumerated I2C buses at address 0x67.   *
*                                                                                               *
*           bus - pointer to i2c_smbus_interface where RGB Fusion device is connected           *
*           dev - I2C address of RGB Fusion device                                              *
*                                                                                               *
\***********************************************************************************************/

void DetectGigabyteRGBFusion2DRAMControllers(std::vector<i2c_smbus_interface*>& busses)
{
    for(unsigned int bus = 0; bus < busses.size(); bus++)
    {
        IF_DRAM_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
        {
            // Check for RGB Fusion 2 DRAM controller at 0x67
            if(TestForGigabyteRGBFusion2DRAMController(busses[bus], 0x67))
            {
                RGBFusion2DRAMController*     controller     = new RGBFusion2DRAMController(busses[bus], 0x67);
                RGBController_RGBFusion2DRAM* rgb_controller = new RGBController_RGBFusion2DRAM(controller);

                ResourceManager::get()->RegisterRGBController(rgb_controller);
            }
        }
    }

}   /* DetectGigabyteRGBFusion2DRAMControllers() */

REGISTER_I2C_DETECTOR("Gigabyte RGB Fusion 2 DRAM", DetectGigabyteRGBFusion2DRAMControllers);
