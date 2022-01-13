#include "Detector.h"
#include "CorsairVengeanceProController.h"
#include "RGBController.h"
#include "RGBController_CorsairVengeancePro.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include "LogManager.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std::chrono_literals;

/******************************************************************************************\
*                                                                                          *
*   TestForCorsairVengeanceProController                                                   *
*                                                                                          *
*       Tests the given address to see if a Corsair Pro controller exists there.           *
*                                                                                          *
\******************************************************************************************/

bool TestForCorsairVengeanceProController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    int res = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);

    LOG_DEBUG("[%s] Trying address %02X", CORSAIR_VENGEANCE_RGB_PRO_NAME, address);

    if (res >= 0)
    {
        pass = true;

        res = bus->i2c_smbus_read_byte_data(address, 0x43);

        if (res != 0x1C)
        {
            pass = false;
            LOG_DEBUG("[%s] Failed: was expecting 0x1C got %02X", CORSAIR_VENGEANCE_RGB_PRO_NAME, res);
        }

        res = bus->i2c_smbus_read_byte_data(address, 0x44);

        if (!((res == 0x03) || (res == 0x04)))
        {
            pass = false;
            LOG_DEBUG("[%s] Failed: was expecting 0x03 or 0x04 got %02X", CORSAIR_VENGEANCE_RGB_PRO_NAME, res);
        }
    }
    else
    {
        LOG_DEBUG("[%s] Failed: res was %04X", CORSAIR_VENGEANCE_RGB_PRO_NAME, res);
    }

    std::this_thread::sleep_for(10ms);
    
    return(pass);

}   /* TestForCorsairVengeanceProController() */

/******************************************************************************************\
*                                                                                          *
*   DetectCorsairVengeanceProControllers                                                   *
*                                                                                          *
*       Detect Corsair Vengeance Pro controllers on the enumerated I2C busses.             *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where Aura device is connected            *
*           dev - I2C address of Aura device                                               *
*                                                                                          *
\******************************************************************************************/

void DetectCorsairVengeanceProControllers(std::vector<i2c_smbus_interface*> &busses)
{
    CorsairVengeanceProController* new_corsair_pro;
    RGBController_CorsairVengeancePro* new_controller;

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        LOG_DEBUG("[%s] Testing bus %d", CORSAIR_VENGEANCE_RGB_PRO_NAME, bus);

        IF_DRAM_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
        {
            // Check for Corsair controller at 0x58
            if (TestForCorsairVengeanceProController(busses[bus], 0x58))
            {
                new_corsair_pro = new CorsairVengeanceProController(busses[bus], 0x58);
                new_controller = new RGBController_CorsairVengeancePro(new_corsair_pro);
                ResourceManager::get()->RegisterRGBController(new_controller);
            }

            // Check for Corsair controller at 0x59
            if (TestForCorsairVengeanceProController(busses[bus], 0x59))
            {
                new_corsair_pro = new CorsairVengeanceProController(busses[bus], 0x59);
                new_controller = new RGBController_CorsairVengeancePro(new_corsair_pro);
                ResourceManager::get()->RegisterRGBController(new_controller);
            }

            // Check for Corsair controller at 0x5A
            if (TestForCorsairVengeanceProController(busses[bus], 0x5A))
            {
                new_corsair_pro = new CorsairVengeanceProController(busses[bus], 0x5A);
                new_controller = new RGBController_CorsairVengeancePro(new_corsair_pro);
                ResourceManager::get()->RegisterRGBController(new_controller);
            }

            // Check for Corsair controller at 0x5B
            if (TestForCorsairVengeanceProController(busses[bus], 0x5B))
            {
                new_corsair_pro = new CorsairVengeanceProController(busses[bus], 0x5B);
                new_controller = new RGBController_CorsairVengeancePro(new_corsair_pro);
                ResourceManager::get()->RegisterRGBController(new_controller);
            }

            // Check for Corsair controller at 0x5C
            if (TestForCorsairVengeanceProController(busses[bus], 0x5C))
            {
                new_corsair_pro = new CorsairVengeanceProController(busses[bus], 0x5C);
                new_controller = new RGBController_CorsairVengeancePro(new_corsair_pro);
                ResourceManager::get()->RegisterRGBController(new_controller);
            }

            // Check for Corsair controller at 0x5D
            if (TestForCorsairVengeanceProController(busses[bus], 0x5D))
            {
                new_corsair_pro = new CorsairVengeanceProController(busses[bus], 0x5D);
                new_controller = new RGBController_CorsairVengeancePro(new_corsair_pro);
                ResourceManager::get()->RegisterRGBController(new_controller);
            }

            // Check for Corsair controller at 0x5E
            if (TestForCorsairVengeanceProController(busses[bus], 0x5E))
            {
                new_corsair_pro = new CorsairVengeanceProController(busses[bus], 0x5E);
                new_controller = new RGBController_CorsairVengeancePro(new_corsair_pro);
                ResourceManager::get()->RegisterRGBController(new_controller);
            }

            // Check for Corsair controller at 0x5F
            if (TestForCorsairVengeanceProController(busses[bus], 0x5F))
            {
                new_corsair_pro = new CorsairVengeanceProController(busses[bus], 0x5F);
                new_controller = new RGBController_CorsairVengeancePro(new_corsair_pro);
                ResourceManager::get()->RegisterRGBController(new_controller);
            }
        }
        else
        {
            LOG_DEBUG("[%s] Bus %d is not a DRAM bus", CORSAIR_VENGEANCE_RGB_PRO_NAME, bus);
        }
    }

}   /* DetectCorsairVengeanceProControllers() */

REGISTER_I2C_DETECTOR("Corsair Vengeance Pro", DetectCorsairVengeanceProControllers);
