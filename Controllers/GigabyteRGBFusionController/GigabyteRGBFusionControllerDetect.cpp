/*---------------------------------------------------------*\
| GigabyteRGBFusionControllerDetect.cpp                     |
|                                                           |
|   Detector for Gigabyte Aorus RGB Fusion SMBus            |
|   motherboard                                             |
|                                                           |
|   Adam Honse (CalcProgrammer1)                10 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <vector>
#include "DetectionManager.h"
#include "GigabyteRGBFusionController.h"
#include "LogManager.h"
#include "RGBController_GigabyteRGBFusion.h"
#include "i2c_smbus.h"
#include "pci_ids.h"

#define DETECTOR_NAME   "Gigabyte RGB Fusion SMBus"
#define VENDOR_NAME     "Gigabyte Technology Co., Ltd."
#define SMBUS_ADDRESS   0x28

bool TestForGigabyteRGBFusionController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    int res = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);

    if (res >= 0)
    {
        pass = true;

        res = bus->i2c_smbus_read_byte_data(address, 0xF2);

        if (res != 0xC4)
        {
            pass = false;
        }
    }

    return(pass);
}

DetectedControllers DetectGigabyteRGBFusionControllers(std::vector<i2c_smbus_interface*>& busses)
{
    DetectedControllers detected_controllers;

    for(unsigned int bus = 0; bus < busses.size(); bus++)
    {
        IF_MOBO_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
        {
            if(busses[bus]->pci_subsystem_vendor == GIGABYTE_SUB_VEN)
            {
                LOG_DEBUG(SMBUS_CHECK_DEVICE_MESSAGE_EN, DETECTOR_NAME, bus, VENDOR_NAME, SMBUS_ADDRESS);

                /*-----------------------------------------*\
                | Check for RGB Fusion controller at 0x28   |
                \*-----------------------------------------*/
                if(TestForGigabyteRGBFusionController(busses[bus], SMBUS_ADDRESS))
                {
                    RGBFusionController*     controller     = new RGBFusionController(busses[bus], SMBUS_ADDRESS);
                    RGBController_RGBFusion* rgb_controller = new RGBController_RGBFusion(controller);

                    detected_controllers.push_back(rgb_controller);
                }
            }
            else
            {
                LOG_DEBUG(SMBUS_CHECK_DEVICE_FAILURE_EN, DETECTOR_NAME, bus, VENDOR_NAME);
            }
        }
    }

    return(detected_controllers);
}

REGISTER_I2C_DETECTOR("Gigabyte RGB Fusion", DetectGigabyteRGBFusionControllers);
