/*---------------------------------------------------------*\
| CorsairVengeanceProControllerDetect.cpp                   |
|                                                           |
|   Detector for Corsair Vengeance Pro RGB RAM              |
|                                                           |
|   Adam Honse (CalcProgrammer1)                30 Jun 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <vector>
#include "Detector.h"
#include "CorsairVengeanceProController.h"
#include "RGBController_CorsairVengeancePro.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include "LogManager.h"

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

    if(res >= 0)
    {
        pass = true;

        res = bus->i2c_smbus_read_byte_data(address, 0x43);

        if (res != 0x1C)
        {
            pass = false;
            LOG_DEBUG("[%s] Failed: was expecting 0x1C got %02X", CORSAIR_VENGEANCE_RGB_PRO_NAME, res);
        }

        res = bus->i2c_smbus_read_byte_data(address, 0x44);

        if(!((res == 0x03) || (res == 0x04)))
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
    for(unsigned int bus = 0; bus < busses.size(); bus++)
    {
        LOG_DEBUG("[%s] Testing bus %d", CORSAIR_VENGEANCE_RGB_PRO_NAME, bus);

        IF_DRAM_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
        {
            for(unsigned char addr = 0x58; addr <= 0x5F; addr++)
            {
                if(TestForCorsairVengeanceProController(busses[bus], addr))
                {
                    CorsairVengeanceProController*     new_controller    = new CorsairVengeanceProController(busses[bus], addr);
                    RGBController_CorsairVengeancePro* new_rgbcontroller = new RGBController_CorsairVengeancePro(new_controller);

                    ResourceManager::get()->RegisterRGBController(new_rgbcontroller);
                }
            }
        }
        else
        {
            LOG_DEBUG("[%s] Bus %d is not a DRAM bus", CORSAIR_VENGEANCE_RGB_PRO_NAME, bus);
        }
    }

}   /* DetectCorsairVengeanceProControllers() */

REGISTER_I2C_DETECTOR("Corsair Vengeance Pro", DetectCorsairVengeanceProControllers);
