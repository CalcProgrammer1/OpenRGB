#include "Detector.h"
#include "AuraSMBusController.h"
#include "RGBController.h"
#include "RGBController_AuraSMBus.h"
#include "i2c_smbus.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std::chrono_literals;

/*----------------------------------------------------------------------*\
| This list contains the available SMBus addresses for mapping Aura RAM  |
\*----------------------------------------------------------------------*/
#define AURA_RAM_ADDRESS_COUNT  23

static const unsigned char aura_ram_addresses[] =
{
    0x70,
    0x71,
    0x72,
    0x73,
    0x74,
    0x75,
    0x76,
    0x78,
    0x79,
    0x7A,
    0x7B,
    0x7C,
    0x7D,
    0x7E,
    0x7F,
    0x4F,
    0x66,
    0x67,
    0x39,
    0x3A,
    0x3B,
    0x3C,
    0x3D
};

/*---------------------------------------------------------------------------------*\
| This list contains the available SMBus addresses for mapping Aura motherboards    |
\*---------------------------------------------------------------------------------*/
#define AURA_MOBO_ADDRESS_COUNT 3

static const unsigned char aura_mobo_addresses[] =
{
    0x40,
    0x4E,
    0x4F
};

/******************************************************************************************\
*                                                                                          *
*   AuraRegisterWrite                                                                      *
*                                                                                          *
*       A standalone version of the AuraSMBusController::AuraRegisterWrite function for    *
*       access to Aura devices without instancing the AuraSMBusController class or reading *
*       the config table from the device.                                                  *
*                                                                                          *
\******************************************************************************************/

void AuraRegisterWrite(i2c_smbus_interface* bus, aura_dev_id dev, aura_register reg, unsigned char val)
{
    //Write Aura register
    bus->i2c_smbus_write_word_data(dev, 0x00, ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF));

    //Write Aura value
    bus->i2c_smbus_write_byte_data(dev, 0x01, val);
}

/******************************************************************************************\
*                                                                                          *
*   TestForAuraSMBusController                                                                  *
*                                                                                          *
*       Tests the given address to see if an Aura controller exists there.  First does a   *
*       quick write to test for a response, and if so does a simple read at 0xA0 to test   *
*       for incrementing values 0...F which was observed at this location during data dump *
*                                                                                          *
\******************************************************************************************/

bool TestForAuraSMBusController(i2c_smbus_interface* bus, unsigned char address)
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

}   /* TestForAuraSMBusController() */

/******************************************************************************************\
*                                                                                          *
*   DetectAuraSMBusControllers                                                                  *
*                                                                                          *
*       Detect Aura controllers on the enumerated I2C busses.  Searches for Aura-enabled   *
*       RAM at 0x77 and tries to initialize their slot addresses, then searches for them   *
*       at their correct initialized addresses.  Also looks for motherboard controller at  *
*       address 0x4E.                                                                      *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where Aura device is connected            *
*           dev - I2C address of Aura device                                               *
*                                                                                          *
\******************************************************************************************/

void DetectAuraSMBusControllers(std::vector<i2c_smbus_interface*> &busses, std::vector<RGBController*> &rgb_controllers)
{
    AuraSMBusController* new_aura;
    RGBController_AuraSMBus* new_controller;

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        int address_list_idx = -1;

        // Remap Aura-enabled RAM modules on 0x77
        for (unsigned int slot = 0; slot < 8; slot++)
        {
            int res = busses[bus]->i2c_smbus_write_quick(0x77, I2C_SMBUS_WRITE);

            if (res < 0)
            {
                break;
            }

            do
            {
                address_list_idx++;

                if(address_list_idx < AURA_RAM_ADDRESS_COUNT)
                {
                    res = busses[bus]->i2c_smbus_write_quick(aura_ram_addresses[address_list_idx], I2C_SMBUS_WRITE);
                }
                else
                {
                    break;
                }
            } while (res >= 0);

            if(address_list_idx < AURA_RAM_ADDRESS_COUNT)
            {
                AuraRegisterWrite(busses[bus], 0x77, AURA_REG_SLOT_INDEX, slot);
                AuraRegisterWrite(busses[bus], 0x77, AURA_REG_I2C_ADDRESS, (aura_ram_addresses[address_list_idx] << 1));
            }
        }

        // Add Aura-enabled controllers at their remapped addresses
        for (unsigned int address_list_idx = 0; address_list_idx < AURA_RAM_ADDRESS_COUNT; address_list_idx++)
        {
            if (TestForAuraSMBusController(busses[bus], aura_ram_addresses[address_list_idx]))
            {
                new_aura = new AuraSMBusController(busses[bus], aura_ram_addresses[address_list_idx]);
                new_controller = new RGBController_AuraSMBus(new_aura);
                rgb_controllers.push_back(new_controller);
            }

            std::this_thread::sleep_for(1ms);
        }

        // Add Aura-enabled motherboard controllers
        for (unsigned int address_list_idx = 0; address_list_idx < AURA_MOBO_ADDRESS_COUNT; address_list_idx++)
        {
            if (TestForAuraSMBusController(busses[bus], aura_mobo_addresses[address_list_idx]))
            {
                new_aura = new AuraSMBusController(busses[bus], aura_mobo_addresses[address_list_idx]);
                new_controller = new RGBController_AuraSMBus(new_aura);
                rgb_controllers.push_back(new_controller);
            }

            std::this_thread::sleep_for(1ms);
        }
    }

}   /* DetectAuraSMBusControllers() */

REGISTER_I2C_DETECTOR("ASUS Aura SMBus", DetectAuraSMBusControllers);
