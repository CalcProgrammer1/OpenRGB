/*---------------------------------------------------------*\
| CrucialControllerDetect.cpp                               |
|                                                           |
|   Detector for Crucial Ballistix RAM                      |
|                                                           |
|   Adam Honse (CalcProgrammer1)                19 Jan 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <stdio.h>
#include <vector>
#include "Detector.h"
#include "CrucialController.h"
#include "LogManager.h"
#include "RGBController_Crucial.h"
#include "i2c_smbus.h"
#include "pci_ids.h"

using namespace std::chrono_literals;

/*----------------------------------------------------------------------*\
| This list contains the available SMBus addresses for Crucial RAM       |
\*----------------------------------------------------------------------*/
#define CRUCIAL_ADDRESS_COUNT  8

static const unsigned char crucial_addresses[] =
{
/*-----------------------------------------------------*\
| These addresses have been disabled due to conflict    |
| with ASUS Aura DRAM.  Since the detection scheme is   |
| the same, Aura RAM will be detected as Crucial.       |
| We need to improve the Crucial detection scheme.      |
\*-----------------------------------------------------*/
    0x39,
    0x3A,
    0x3B,
    0x3C,
    0x20,
    0x21,
    0x22,
    0x23
};

#define CRUCIAL_CONTROLLER_NAME "Crucial DRAM"
std::string concatHexArray(const unsigned char array[], int count, const char split_char[])
{
    std::string addresses = "";
    for(int i = 0; i < count; i++)
    {
        char buffer[6];
        snprintf(buffer, 6, "0x%02X%s", array[i], (i < count-1)? split_char: "");
        addresses += buffer;
    }
    return addresses;
}
#define TESTING_ADDRESSES concatHexArray(crucial_addresses, CRUCIAL_ADDRESS_COUNT, "|").c_str()

/******************************************************************************************\
*                                                                                          *
*   CrucialRegisterRead                                                                    *
*                                                                                          *
*       A standalone version of the AuraSMBusController::AuraRegisterRead function for     *
*       access to Aura devices without instancing the AuraSMBusController class or reading *
*       the config table from the device.                                                  *
*                                                                                          *
\******************************************************************************************/

unsigned char CrucialRegisterRead(i2c_smbus_interface* bus, crucial_dev_id dev, crucial_register reg)
{
    //Write Aura register
    bus->i2c_smbus_write_word_data(dev, 0x00, ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF));

    //Read Aura value
    return(bus->i2c_smbus_read_byte_data(dev, 0x81));
}

/******************************************************************************************\
*                                                                                          *
*   TestForCrucialController                                                               *
*                                                                                          *
*       Tests the given address to see if an Crucial controller exists there.  First does a*
*       quick write to test for a response, and if so does a simple read at 0xA0 to test   *
*       for incrementing values 0...F which was observed at this location during data dump *
*                                                                                          *
\******************************************************************************************/

bool TestForCrucialController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    int res = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);

    if (res >= 0)
    {
        pass = true;

        LOG_DEBUG("[%s] Detected an I2C device at address %02X", CRUCIAL_CONTROLLER_NAME, address);

        for (int i = 0xA0; i < 0xB0; i++)
        {
            res = bus->i2c_smbus_read_byte_data(address, i);

            if (res != (i - 0xA0))
            {
                LOG_VERBOSE("[%s] Detection failed testing register %02X.  Expected %02X, got %02X.", CRUCIAL_CONTROLLER_NAME, i, (i - 0xA0), res);

                pass = false;
            }
        }

        if(pass)
        {
            LOG_DEBUG("[%s] Checking for Micron string", CRUCIAL_CONTROLLER_NAME);

            char buf[16];
            for(int i = 0; i < 16; i++)
            {
                buf[i] = CrucialRegisterRead(bus, address, CRUCIAL_REG_MICRON_CHECK_1 + i);
            }

            if(strcmp(buf, "Micron") == 0)
            {
                LOG_DEBUG("[%s] Device %02X is a Micron device, continuing", CRUCIAL_CONTROLLER_NAME, address);
            }
            else
            {
                for(int i = 0; i < 16; i++)
                {
                    buf[i] = CrucialRegisterRead(bus, address, CRUCIAL_REG_MICRON_CHECK_2 + i);
                }

                if(strcmp(buf, "Micron") == 0)
                {
                    LOG_DEBUG("[%s] Device %02X is a Micron device, continuing", CRUCIAL_CONTROLLER_NAME, address);
                }
                else
                {
                    LOG_DEBUG("[%s] Device %02X is not a Micron device, skipping", CRUCIAL_CONTROLLER_NAME, address);
                    pass = false;
                }
            }
        }
    }

    return(pass);

}   /* TestForCrucialController() */

void CrucialRegisterWrite(i2c_smbus_interface* bus, unsigned char dev, unsigned short reg, unsigned char val)
{
    //Write Crucial register
    bus->i2c_smbus_write_word_data(dev, 0x00, ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF));

    //Write Crucial value
    bus->i2c_smbus_write_byte_data(dev, 0x01, val);
}

/******************************************************************************************\
*                                                                                          *
*   DetectCrucialControllers                                                               *
*                                                                                          *
*       Detect Crucial controllers on the enumerated I2C busses.                           *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where Aura device is connected            *
*           dev - I2C address of Aura device                                               *
*                                                                                          *
\******************************************************************************************/

void DetectCrucialControllers(std::vector<i2c_smbus_interface*> &busses)
{
    for(unsigned int bus = 0; bus < busses.size(); bus++)
    {
        int address_list_idx = -1;

        IF_DRAM_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
        {
            for(unsigned int slot = 0; slot < 4; slot++)
            {
                int res = busses[bus]->i2c_smbus_write_quick(0x27, I2C_SMBUS_WRITE);

                if(res < 0)
                {
                    break;
                }

                LOG_DEBUG("[%s] Remapping RAM module on 0x27", CRUCIAL_CONTROLLER_NAME);
                do
                {
                    address_list_idx++;

                    if(address_list_idx < CRUCIAL_ADDRESS_COUNT)
                    {
                        res = busses[bus]->i2c_smbus_write_quick(crucial_addresses[address_list_idx], I2C_SMBUS_WRITE);
                    }
                    else
                    {
                        break;
                    }
                } while(res >= 0);

                if(address_list_idx < CRUCIAL_ADDRESS_COUNT)
                {
                    LOG_DEBUG("[%s] Remapping slot %d to address %02X", CRUCIAL_CONTROLLER_NAME, slot, crucial_addresses[address_list_idx]);
                    CrucialRegisterWrite(busses[bus], 0x27, 0x82EE, slot);
                    CrucialRegisterWrite(busses[bus], 0x27, 0x82EF, (crucial_addresses[address_list_idx] << 1));
                    CrucialRegisterWrite(busses[bus], 0x27, 0x82F0, 0xF0);
                }

                std::this_thread::sleep_for(1ms);
            }

            LOG_DEBUG("[%s] In bus: %02X:%02X looking for devices at [%s]", CRUCIAL_CONTROLLER_NAME, busses[bus]->pci_vendor, busses[bus]->pci_device, TESTING_ADDRESSES);

            // Add Crucial controllers
            for(unsigned int address_list_idx = 0; address_list_idx < CRUCIAL_ADDRESS_COUNT; address_list_idx++)
            {
                LOG_DEBUG("[%s] Testing address %02X to see if there is a device there", CRUCIAL_CONTROLLER_NAME, crucial_addresses[address_list_idx]);

                if(TestForCrucialController(busses[bus], crucial_addresses[address_list_idx]))
                {
                    CrucialController*     controller     = new CrucialController(busses[bus], crucial_addresses[address_list_idx]);
                    RGBController_Crucial* rgb_controller = new RGBController_Crucial(controller);

                    ResourceManager::get()->RegisterRGBController(rgb_controller);
                }

                std::this_thread::sleep_for(1ms);
            }
        }
    }

}   /* DetectCrucialControllers() */

REGISTER_I2C_DETECTOR("Crucial Ballistix", DetectCrucialControllers);
