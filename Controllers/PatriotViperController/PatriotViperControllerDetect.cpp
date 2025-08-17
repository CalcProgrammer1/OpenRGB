/*---------------------------------------------------------*\
| PatriotViperControllerDetect.cpp                          |
|                                                           |
|   Detector for Patriot Viper RAM                          |
|                                                           |
|   Adam Honse (CalcProgrammer1)                01 Jan 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <vector>
#include "Detector.h"
#include "PatriotViperController.h"
#include "LogManager.h"
#include "RGBController_PatriotViper.h"
#include "i2c_smbus.h"
#include "pci_ids.h"

using namespace std::chrono_literals;
#define PATRIOT_CONTROLLER_NAME "Patriot Viper"

/******************************************************************************************\
*                                                                                          *
*   TestForPatriotViperController                                                          *
*                                                                                          *
*       Tests the given address to see if a Patriot Viper controller exists there.         *
*                                                                                          *
\******************************************************************************************/

bool TestForPatriotViperController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    int res = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);

    LOG_DEBUG("[%s] Writing at address %02X, res=%02X", PATRIOT_CONTROLLER_NAME, address, res);

    if (res >= 0)
    {
        pass = true;
    }

    return(pass);

}   /* TestForPatriotViperController() */


/******************************************************************************************\
*                                                                                          *
*   DetectPatriotViperControllers                                                          *
*                                                                                          *
*       Detect Patriot Viper RGB controllers on the enumerated I2C busses.                 *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where Aura device is connected            *
*           dev - I2C address of Aura device                                               *
*                                                                                          *
\******************************************************************************************/

void DetectPatriotViperControllers(i2c_smbus_interface* bus, std::vector<SPDWrapper*> &slots, const std::string &/*name*/)
{
    unsigned char slots_valid = 0x00;

    // Check for Patriot Viper controller at 0x77
    LOG_DEBUG("[%s] Testing bus %d at address 0x77", PATRIOT_CONTROLLER_NAME, bus->port_id);

    if(TestForPatriotViperController(bus, 0x77))
    {
        for(SPDWrapper *slot : slots)
        {
            if((slot->manufacturer_data(0x00) == 0x4D)
            &&(slot->manufacturer_data(0x01) == 0x49)
            &&(slot->manufacturer_data(0x02) == 0x43)
            &&(slot->manufacturer_data(0x03) == 0x53)
            &&(slot->manufacturer_data(0x04) == 0x59)
            &&(slot->manufacturer_data(0x05) == 0x53)
            &&(slot->manufacturer_data(0x06) == 0x5f)
            &&(slot->manufacturer_data(0x07) == 0x44))
            {
                LOG_DEBUG("[%s] The RAM module detected in slot %d", PATRIOT_CONTROLLER_NAME, slot->index());
                slots_valid |= (1 << (slot->index()));
            }
        }

        if(slots_valid != 0)
        {
            PatriotViperController*     controller     = new PatriotViperController(bus, 0x77, slots_valid);
            RGBController_PatriotViper* rgb_controller = new RGBController_PatriotViper(controller);
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
    }

}   /* DetectPatriotViperControllers() */

REGISTER_I2C_DIMM_DETECTOR(PATRIOT_CONTROLLER_NAME, DetectPatriotViperControllers, JEDEC_PATRIOT, SPD_DDR4_SDRAM);

