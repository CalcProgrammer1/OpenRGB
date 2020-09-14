#include "Detector.h"
#include "CrucialController.h"
#include "RGBController.h"
#include "RGBController_Crucial.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

/*----------------------------------------------------------------------*\
| This list contains the available SMBus addresses for Crucial RAM       |
\*----------------------------------------------------------------------*/
#define CRUCIAL_ADDRESS_COUNT  4

static const unsigned char crucial_addresses[] =
{
    0x20,
    0x21,
    0x22,
    0x23
};


/******************************************************************************************\
*                                                                                          *
*   TestForCrucialController                                                               *
*                                                                                          *
*       Tests the given address to see if an Crucial controller exists there.  First does a*
*       quick write to test for a response, and if so does a simple read at 0xA0 to test   *
*       for incrementing values 0...F which was observed at this location during data dump *
*                                                                                          *
\******************************************************************************************/

bool TestForCrucialController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    int res = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);

    if (res >= 0)
    {
        pass = true;

        for (int i = 0xA0; i < 0xB0; i++)
        {
            res = bus->i2c_smbus_read_byte_data(address, i);

            if (res != (i - 0xA0))
            {
                pass = false;
            }
        }
    }

    return(pass);

}   /* TestForCrucialController() */

/******************************************************************************************\
*                                                                                          *
*   DetectCrucialControllers                                                               *
*                                                                                          *
*       Detect Crucial controllers on the enumerated I2C busses.                           *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where Aura device is connected            *
*           dev - I2C address of Aura device                                               *
*                                                                                          *
\******************************************************************************************/

void DetectCrucialControllers(std::vector<i2c_smbus_interface*> &busses, std::vector<RGBController*> &rgb_controllers)
{
    CrucialController* new_crucial;
    RGBController_Crucial* new_controller;

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        IF_DRAM_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
        {
            // Add Crucial controllers
            for (unsigned int address_list_idx = 0; address_list_idx < CRUCIAL_ADDRESS_COUNT; address_list_idx++)
            {
                if (TestForCrucialController(busses[bus], crucial_addresses[address_list_idx]))
                {
                    new_crucial = new CrucialController(busses[bus], crucial_addresses[address_list_idx]);
                    new_controller = new RGBController_Crucial(new_crucial);
                    rgb_controllers.push_back(new_controller);
                }
            }
        }
    }

}   /* DetectCrucialControllers() */

REGISTER_I2C_DETECTOR("Crucial", DetectCrucialControllers);
