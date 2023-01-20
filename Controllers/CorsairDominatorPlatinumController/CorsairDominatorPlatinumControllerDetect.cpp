#include "Detector.h"
#include "CorsairDominatorPlatinumController.h"
#include "RGBController.h"
#include "RGBController_CorsairDominatorPlatinum.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include "LogManager.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std::chrono_literals;

bool TestForCorsairDominatorPlatinumController(i2c_smbus_interface *bus, unsigned char address)
{
    int res = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);

    LOG_DEBUG("[%s] Trying address %02X", CORSAIR_DOMINATOR_PLATINUM_NAME, address);

    if(res < 0)
    {
        LOG_DEBUG("[%s] Failed: res was %04X", CORSAIR_DOMINATOR_PLATINUM_NAME, res);
        return false;
    }

    res = bus->i2c_smbus_read_byte_data(address, 0x43);

    if(res != 0x1b)
    {
        LOG_DEBUG("[%s] Failed: expected 0x1b, got %04X", CORSAIR_DOMINATOR_PLATINUM_NAME, res);
        return false;
    }

    res = bus->i2c_smbus_read_byte_data(address, 0x44);

    if(res != 0x04)
    {
        LOG_DEBUG("[%s] Failed: expected 0x04, got %04X", CORSAIR_DOMINATOR_PLATINUM_NAME, res);
        return false;
    }

    return true;
}

/******************************************************************************************\
*                                                                                          *
*   DetectCorsairDominatorPlatinumControllers                                              *
*                                                                                          *
*       Detect Corsair Dominator Platinum controllers on the enumerated I2C busses.        *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where Aura device is connected            *
*           dev - I2C address of Aura device                                               *
*                                                                                          *
\******************************************************************************************/

void DetectCorsairDominatorPlatinumControllers(std::vector<i2c_smbus_interface *> &busses)
{
    for(unsigned int bus = 0; bus < busses.size(); bus++)
    {
        IF_DRAM_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
        {
            LOG_DEBUG("[%s] Testing bus %d", CORSAIR_DOMINATOR_PLATINUM_NAME, bus);

            for(unsigned char addr = 0x58; addr <= 0x5F; addr++)
            {
                if(TestForCorsairDominatorPlatinumController(busses[bus], addr))
                {
                    CorsairDominatorPlatinumController*     new_controller    = new CorsairDominatorPlatinumController(busses[bus], addr);
                    RGBController_CorsairDominatorPlatinum* new_rgbcontroller = new RGBController_CorsairDominatorPlatinum(new_controller);
                    
                    ResourceManager::get()->RegisterRGBController(new_rgbcontroller);
                }
                std::this_thread::sleep_for(10ms);
            }
            for(unsigned char addr = 0x18; addr <= 0x1F; addr++)
            {
                if(TestForCorsairDominatorPlatinumController(busses[bus], addr))
                {
                    CorsairDominatorPlatinumController*     new_controller    = new CorsairDominatorPlatinumController(busses[bus], addr);
                    RGBController_CorsairDominatorPlatinum* new_rgbcontroller = new RGBController_CorsairDominatorPlatinum(new_controller);
                    
                    ResourceManager::get()->RegisterRGBController(new_rgbcontroller);
                }
                std::this_thread::sleep_for(10ms);
            }
        }
        else
        {
            LOG_DEBUG("[%s] Bus %d is not a DRAM bus", CORSAIR_DOMINATOR_PLATINUM_NAME, bus);
        }
    }
}   /* DetectCorsairDominatorPlatinumControllers() */

REGISTER_I2C_DETECTOR("Corsair Dominator Platinum", DetectCorsairDominatorPlatinumControllers);
