/*---------------------------------------------------------*\
| CorsairVengeanceControllerDetect.cpp                      |
|                                                           |
|   Detector for original single-zone Corsair Vengeance     |
|   DDR4 RGB RAM                                            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                08 Mar 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <vector>
#include "CorsairVengeanceController.h"
#include "DetectionManager.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include "RGBController_CorsairVengeance.h"

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

}

DetectedControllers DetectCorsairVengeanceControllers(i2c_smbus_interface* bus, std::vector<SPDWrapper*> &slots, const std::string &/*name*/)
{
    DetectedControllers detected_controllers;

    for(SPDWrapper *slot : slots)
    {
        /*-------------------------------------------------*\
        | Test first address range 0x58-0x5F                |
        \*-------------------------------------------------*/
        unsigned char address = slot->address() + 8;

        if(TestForCorsairVengeanceController(bus, address))
        {
            CorsairVengeanceController*     controller     = new CorsairVengeanceController(bus, address);
            RGBController_CorsairVengeance* rgb_controller = new RGBController_CorsairVengeance(controller);

            detected_controllers.push_back(rgb_controller);
        }

        /*-------------------------------------------------*\
        | Test second address range 0x18-0x1F               |
        \*-------------------------------------------------*/
        address = slot->address() - 0x40 + 8;

        if(TestForCorsairVengeanceController(bus, address))
        {
            CorsairVengeanceController*     controller     = new CorsairVengeanceController(bus, address);
            RGBController_CorsairVengeance* rgb_controller = new RGBController_CorsairVengeance(controller);

            detected_controllers.push_back(rgb_controller);
        }
    }

    return(detected_controllers);
}

REGISTER_I2C_DRAM_DETECTOR("Corsair Vengeance RGB DRAM", DetectCorsairVengeanceControllers, JEDEC_CORSAIR, SPD_DDR4_SDRAM);
