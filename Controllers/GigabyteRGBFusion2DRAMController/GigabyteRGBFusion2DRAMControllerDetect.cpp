#include "Detector.h"
#include "GigabyteRGBFusion2DRAMController.h"
#include "RGBController.h"
#include "RGBController_GigabyteRGBFusion2DRAM.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string>

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
    if (res >= 0)
    {
        pass = true;

        // res = bus->i2c_smbus_read_byte_data(address, 0xF2);

        // if (res != 0xC4)
        // {
        //     pass = false;
        // }
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
    RGBFusion2DRAMController* new_rgb_fusion;
    RGBController_RGBFusion2DRAM* new_controller;

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        IF_DRAM_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
        {
            // Check for RGB Fusion 2 DRAM controller at 0x67
            if (TestForGigabyteRGBFusion2DRAMController(busses[bus], 0x67))
            {
                new_rgb_fusion = new RGBFusion2DRAMController(busses[bus], 0x67);
                new_controller = new RGBController_RGBFusion2DRAM(new_rgb_fusion);
                ResourceManager::get()->RegisterRGBController(new_controller);
            }
        }
    }

}   /* DetectGigabyteRGBFusion2DRAMControllers() */

// This detector is disabled as proper detection isn't implemented
//REGISTER_I2C_DETECTOR("Gigabyte RGB Fusion 2 DRAM", DetectGigabyteRGBFusion2DRAMControllers);
