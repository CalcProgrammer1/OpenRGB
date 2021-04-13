#include "Detector.h"
#include "CrucialController.h"
#include "RGBController.h"
#include "RGBController_Crucial.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std::chrono_literals;

/*----------------------------------------------------------------------*\
| This list contains the available SMBus addresses for Crucial RAM       |
\*----------------------------------------------------------------------*/
#define CRUCIAL_ADDRESS_COUNT  4

static const unsigned char crucial_addresses[] =
{
/*-----------------------------------------------------*\
| These addresses have been disabled due to conflict    |
| with ASUS Aura DRAM.  Since the detection scheme is   |
| the same, Aura RAM will be detected as Crucial.       |
| We need to improve the Crucial detection scheme.      |
\*-----------------------------------------------------*/
//  0x39,
//  0x3A,
//  0x3B,
//  0x3C,
    0x20,
    0x21,
    0x22,
    0x23
};


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

        for (int i = 0xA0; i < 0xB0; i++)
        {
            res = bus->i2c_smbus_read_byte_data(address, i);

            if (res != (i - 0xA0))
            {
                pass = false;
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
    CrucialController* new_crucial;
    RGBController_Crucial* new_controller;

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        int address_list_idx = -1;

        IF_DRAM_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
        {
            // Remap Crucial RAM modules on 0x27
            for (unsigned int slot = 0; slot < 4; slot++)
            {
                int res = busses[bus]->i2c_smbus_write_quick(0x27, I2C_SMBUS_WRITE);

                if (res < 0)
                {
                    break;
                }

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
                } while (res >= 0);

                if(address_list_idx < CRUCIAL_ADDRESS_COUNT)
                {
                    CrucialRegisterWrite(busses[bus], 0x27, 0x82EE, slot);
                    CrucialRegisterWrite(busses[bus], 0x27, 0x82EF, (crucial_addresses[address_list_idx] << 1));
                    CrucialRegisterWrite(busses[bus], 0x27, 0x82F0, 0xF0);
                }

                std::this_thread::sleep_for(1ms);
            }

            // Add Crucial controllers
            for (unsigned int address_list_idx = 0; address_list_idx < CRUCIAL_ADDRESS_COUNT; address_list_idx++)
            {
                if (TestForCrucialController(busses[bus], crucial_addresses[address_list_idx]))
                {
                    new_crucial = new CrucialController(busses[bus], crucial_addresses[address_list_idx]);
                    new_controller = new RGBController_Crucial(new_crucial);
                    ResourceManager::get()->RegisterRGBController(new_controller);
                }
            }
        }
    }

}   /* DetectCrucialControllers() */

REGISTER_I2C_DETECTOR("Crucial", DetectCrucialControllers);
