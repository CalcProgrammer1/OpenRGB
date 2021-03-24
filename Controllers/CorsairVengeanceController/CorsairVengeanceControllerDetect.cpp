#include "Detector.h"
#include "CorsairVengeanceController.h"
#include "RGBController.h"
#include "RGBController_CorsairVengeance.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************************\
*                                                                                          *
*   TestForCorsairVengeanceController                                                      *
*                                                                                          *
*       Tests the given address to see if a Corsair controller exists there.               *
*                                                                                          *
\******************************************************************************************/

bool TestForCorsairVengeanceController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    int res = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);

    if (res >= 0)
    {
        pass = true;

        for (int i = 0xA0; i < 0xB0; i++)
        {
            res = bus->i2c_smbus_read_byte_data(address, i);

            if (res != 0xBA)
            {
                pass = false;
            }
        }
    }

    return(pass);

}   /* TestForCorsairVengeanceController() */

/******************************************************************************************\
*                                                                                          *
*   DetectCorsairVengeanceControllers                                                      *
*                                                                                          *
*       Detect Corsair controllers on the enumerated I2C busses.                           *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where Aura device is connected            *
*           dev - I2C address of Aura device                                               *
*                                                                                          *
\******************************************************************************************/

void DetectCorsairVengeanceControllers(std::vector<i2c_smbus_interface*> &busses)
{
    CorsairVengeanceController* new_corsair;
    RGBController_CorsairVengeance* new_controller;

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        IF_DRAM_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
        {
            // Check for Corsair controller at 0x58
            if (TestForCorsairVengeanceController(busses[bus], 0x58))
            {
                new_corsair = new CorsairVengeanceController(busses[bus], 0x58);
                new_controller = new RGBController_CorsairVengeance(new_corsair);
                ResourceManager::get()->RegisterRGBController(new_controller);
            }

            // Check for Corsair controller at 0x59
            if (TestForCorsairVengeanceController(busses[bus], 0x59))
            {
                new_corsair = new CorsairVengeanceController(busses[bus], 0x59);
                new_controller = new RGBController_CorsairVengeance(new_corsair);
                ResourceManager::get()->RegisterRGBController(new_controller);
            }

            // Check for Corsair controller at 0x5A
            if (TestForCorsairVengeanceController(busses[bus], 0x5A))
            {
                new_corsair = new CorsairVengeanceController(busses[bus], 0x5A);
                new_controller = new RGBController_CorsairVengeance(new_corsair);
                ResourceManager::get()->RegisterRGBController(new_controller);
            }

            // Check for Corsair controller at 0x5B
            if (TestForCorsairVengeanceController(busses[bus], 0x5B))
            {
                new_corsair = new CorsairVengeanceController(busses[bus], 0x5B);
                new_controller = new RGBController_CorsairVengeance(new_corsair);
                ResourceManager::get()->RegisterRGBController(new_controller);
            }

            // Check for Corsair controller at 0x5C
            if (TestForCorsairVengeanceController(busses[bus], 0x5C))
            {
                new_corsair = new CorsairVengeanceController(busses[bus], 0x5C);
                new_controller = new RGBController_CorsairVengeance(new_corsair);
                ResourceManager::get()->RegisterRGBController(new_controller);
            }

            // Check for Corsair controller at 0x5D
            if (TestForCorsairVengeanceController(busses[bus], 0x5D))
            {
                new_corsair = new CorsairVengeanceController(busses[bus], 0x5D);
                new_controller = new RGBController_CorsairVengeance(new_corsair);
                ResourceManager::get()->RegisterRGBController(new_controller);
            }

            // Check for Corsair controller at 0x5E
            if (TestForCorsairVengeanceController(busses[bus], 0x5E))
            {
                new_corsair = new CorsairVengeanceController(busses[bus], 0x5E);
                new_controller = new RGBController_CorsairVengeance(new_corsair);
                ResourceManager::get()->RegisterRGBController(new_controller);
            }

            // Check for Corsair controller at 0x5F
            if (TestForCorsairVengeanceController(busses[bus], 0x5F))
            {
                new_corsair = new CorsairVengeanceController(busses[bus], 0x5F);
                new_controller = new RGBController_CorsairVengeance(new_corsair);
                ResourceManager::get()->RegisterRGBController(new_controller);
            }
        }
    }

}   /* DetectCorsairVengeanceControllers() */

REGISTER_I2C_DETECTOR("Corsair Vengeance", DetectCorsairVengeanceControllers);
