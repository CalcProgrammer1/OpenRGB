/*---------------------------------------------------------*\
| EVGASMBusControllerDetect.cpp                             |
|                                                           |
|   Detector for SMBus EVGA ACX 30 motherboards             |
|                                                           |
|   Balázs Triszka (balika011)                  21 May 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <vector>
#include "DetectionManager.h"
#include "EVGAACX30SMBusController.h"
#include "i2c_smbus.h"
#include "LogManager.h"
#include "pci_ids.h"
#include "RGBController_EVGAACX30SMBus.h"

#define EVGA_DETECTOR_NAME          "EVGA SMBus Detector"
#define VENDOR_NAME                 "EVGA"
#define SMBUS_ADDRESS               0x28

bool TestForAcx30SMBusController(i2c_smbus_interface *bus, uint8_t address)
{
    bool pass = false;

    int res = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);

    if (res >= 0)
    {
        res = bus->i2c_smbus_read_byte_data(address, 0x01);

        if (res > 0 && (res & 1))
        {
            pass = true;
        }
    }

    return(pass);
}

DetectedControllers DetectAcx30SMBusControllers(std::vector<i2c_smbus_interface *> &busses)
{
    DetectedControllers detected_controllers;

    for(unsigned int bus = 0; bus < busses.size(); bus++)
    {
        IF_MOBO_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
        {
            if(busses[bus]->pci_subsystem_vendor == EVGA_SUB_VEN)
            {
                LOG_DEBUG(SMBUS_CHECK_DEVICE_MESSAGE_EN, EVGA_DETECTOR_NAME, bus, VENDOR_NAME, SMBUS_ADDRESS);
                // Check for ACX 30 controller at 0x28
                if(TestForAcx30SMBusController(busses[bus], SMBUS_ADDRESS))
                {
                    EVGAACX30SMBusController *     controller     = new EVGAACX30SMBusController(busses[bus], SMBUS_ADDRESS);
                    RGBController_EVGAACX30SMBus * rgb_controller = new RGBController_EVGAACX30SMBus(controller);

                    detected_controllers.push_back(rgb_controller);
                }
            }
            else
            {
                LOG_DEBUG(SMBUS_CHECK_DEVICE_FAILURE_EN, EVGA_DETECTOR_NAME, bus, VENDOR_NAME);
            }
        }
    }

    return(detected_controllers);
}

REGISTER_I2C_DETECTOR("EVGA Motherboard SMBus Controllers", DetectAcx30SMBusControllers);
