/*---------------------------------------------------------*\
| CorsairVengeanceControllerDetect.cpp                      |
|                                                           |
|   Detector for original single-zone Corsair Vengeance     |
|   DDR4 RGB RAM                                            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                08 Mar 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <vector>
#include "Detector.h"
#include "CorsairVengeanceController.h"
#include "RGBController_CorsairVengeance.h"
#include "i2c_smbus.h"
#include "pci_ids.h"

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
    for(unsigned int bus = 0; bus < busses.size(); bus++)
    {
        IF_DRAM_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
        {
            for(unsigned char addr = 0x58; addr <= 0x5F; addr++)
            {
                if(TestForCorsairVengeanceController(busses[bus], addr))
                {
                    CorsairVengeanceController*     new_controller    = new CorsairVengeanceController(busses[bus], addr);
                    RGBController_CorsairVengeance* new_rgbcontroller = new RGBController_CorsairVengeance(new_controller);

                    ResourceManager::get()->RegisterRGBController(new_rgbcontroller);
                }
            }
            for(unsigned char addr = 0x18; addr <= 0x1F; addr++)
            {
                if(TestForCorsairVengeanceController(busses[bus], addr))
                {
                    CorsairVengeanceController*     new_controller    = new CorsairVengeanceController(busses[bus], addr);
                    RGBController_CorsairVengeance* new_rgbcontroller = new RGBController_CorsairVengeance(new_controller);

                    ResourceManager::get()->RegisterRGBController(new_rgbcontroller);
                }
            }
        }
    }

}   /* DetectCorsairVengeanceControllers() */

REGISTER_I2C_DETECTOR("Corsair Vengeance", DetectCorsairVengeanceControllers);
