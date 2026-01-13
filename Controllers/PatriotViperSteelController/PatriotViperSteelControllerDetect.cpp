/*---------------------------------------------------------*\
| PatriotViperSteelControllerDetect.cpp                     |
|                                                           |
|   Detector for Patriot Viper Steel RAM                    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <vector>
#include "DetectionManager.h"
#include "PatriotViperSteelController.h"
#include "LogManager.h"
#include "RGBController_PatriotViperSteel.h"
#include "i2c_smbus.h"
#include "pci_ids.h"

using namespace std::chrono_literals;

#define PATRIOT_CONTROLLER_NAME "Patriot Viper Steel"

bool TestForPatriotViperSteelController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    int res = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);

    LOG_DEBUG("[%s] Writing at address %02X, res=%02X", PATRIOT_CONTROLLER_NAME, address, res);

    if (res >= 0)
    {
        pass = true;
    }

    return(pass);
}

DetectedControllers DetectPatriotViperSteelControllers(i2c_smbus_interface* bus, std::vector<SPDWrapper*> &slots, const std::string &/*name*/)
{
    DetectedControllers detected_controllers;
    unsigned char       slots_valid = 0x00;

    /*-----------------------------------------------------*\
    | Check for Patriot Viper controller at 0x77            |
    \*-----------------------------------------------------*/
    LOG_DEBUG("[%s] Testing bus %d at address 0x77", PATRIOT_CONTROLLER_NAME, bus->port_id);

    if(TestForPatriotViperSteelController(bus, 0x77))
    {
        for(SPDWrapper *slot : slots)
        {
            if((slot->manufacturer_data(0x00) == 0x50)
            &&(slot->manufacturer_data(0x01) == 0x44)
            &&(slot->manufacturer_data(0x02) == 0x41)
            &&(slot->manufacturer_data(0x03) == 0x31)
            &&(slot->manufacturer_data(0x04) == 0x00)
            &&(slot->manufacturer_data(0x05) == 0x00)
            &&(slot->manufacturer_data(0x06) == 0x00)
            &&(slot->manufacturer_data(0x07) == 0x00))
            {
                LOG_DEBUG("[%s] The RAM module detected in slot %d", PATRIOT_CONTROLLER_NAME, slot->index());
                slots_valid |= (1 << (slot->index()));
            }
        }

        if(slots_valid != 0)
        {
            PatriotViperSteelController*     controller     = new PatriotViperSteelController(bus, 0x77);
            RGBController_PatriotViperSteel* rgb_controller = new RGBController_PatriotViperSteel(controller);

            detected_controllers.push_back(rgb_controller);
        }
    }

    return(detected_controllers);
}

REGISTER_I2C_DRAM_DETECTOR(PATRIOT_CONTROLLER_NAME, DetectPatriotViperSteelControllers, 0xFF7E, SPD_DDR4_SDRAM);

