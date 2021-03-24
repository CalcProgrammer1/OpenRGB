#include "Detector.h"
#include "ASRockPolychromeSMBusController.h"
#include "RGBController.h"
#include "RGBController_ASRockPolychromeSMBus.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************************\
*                                                                                          *
*   TestForPolychromeSMBusController                                                       *
*                                                                                          *
*       Tests the given address to see if an ASRock Polychrome RGB controller exists there.*
*       First does a quick write to test for a response                                    *
*                                                                                          *
\******************************************************************************************/

bool TestForPolychromeSMBusController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    int res = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);

    if (res >= 0)
    {
        pass = true;
    }

    return(pass);

}   /* TestForPolychromeController() */

/******************************************************************************************\
*                                                                                          *
*   DetectPolychromeControllers                                                            *
*                                                                                          *
*       Detect ASRock Polychrome RGB SMBus controllers on the enumerated I2C busses at     *
*       address 0x6A.                                                                      *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where Polychrome device is connected      *
*           dev - I2C address of Polychrome device                                         *
*                                                                                          *
\******************************************************************************************/

void DetectPolychromeSMBusControllers(std::vector<i2c_smbus_interface*>& busses)
{
    PolychromeController* new_polychrome;
    RGBController_Polychrome* new_controller;

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        IF_MOBO_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
        {
            // Check for Polychrome controller at 0x6A
            if (TestForPolychromeSMBusController(busses[bus], 0x6A))
            {
                new_polychrome = new PolychromeController(busses[bus], 0x6A);

                if(new_polychrome->GetASRockType() != ASROCK_TYPE_UNKNOWN)
                {
                    new_controller = new RGBController_Polychrome(new_polychrome);
                    ResourceManager::get()->RegisterRGBController(new_controller);
                }
                else
                {
                    delete new_polychrome;
                }
            }
        }
    }

}   /* DetectSMBusPolychromeControllers() */

REGISTER_I2C_DETECTOR("ASRock Polychrome SMBus", DetectPolychromeSMBusControllers);
