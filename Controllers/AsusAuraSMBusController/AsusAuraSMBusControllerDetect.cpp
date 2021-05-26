#include "Detector.h"
#include "AsusAuraSMBusController.h"
#include "LogManager.h"
#include "RGBController.h"
#include "RGBController_AsusAuraSMBus.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "dependencies/dmiinfo.h"

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

void AsusAuraRegisterWrite(i2c_smbus_interface* bus, aura_dev_id dev, aura_register reg, unsigned char val)
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

bool TestForAsusAuraSMBusController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    LOG_DEBUG("Testing address %02X for Aura SMBus controller", address);

    int res = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);

    if (res >= 0)
    {
        pass = true;

        LOG_DEBUG("Detected an I2C device at address %02X, testing register range for Aura SMBus", address);

        for (int i = 0xA0; i < 0xB0; i++)
        {
            res = bus->i2c_smbus_read_byte_data(address, i);

            if (res != (i - 0xA0))
            {
                LOG_DEBUG("Detection failed testing register %02X.  Expected %02X, got %02X.", i, (i - 0xA0), res);

                pass = false;
            }
        }

        if(pass)
        {
            LOG_DEBUG("Detection successful, address %02X appears to be an Aura SMBus controller", address);
        }
    }

    return(pass);

}   /* TestForAuraSMBusController() */

/******************************************************************************************\
*                                                                                          *
*   DetectAuraSMBusControllers                                                             *
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

void DetectAsusAuraSMBusDRAMControllers(std::vector<i2c_smbus_interface*> &busses)
{
    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        int address_list_idx = -1;

        IF_DRAM_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
        {
            LOG_DEBUG("Remapping Aura SMBus RAM modules on 0x77");

            for (unsigned int slot = 0; slot < 8; slot++)
            {
                int res = busses[bus]->i2c_smbus_write_quick(0x77, I2C_SMBUS_WRITE);

                if (res < 0)
                {
                    LOG_DEBUG("No device detected at address 0x77, aborting remap");

                    break;
                }

                do
                {
                    address_list_idx++;

                    if(address_list_idx < AURA_RAM_ADDRESS_COUNT)
                    {
                        LOG_DEBUG("Testing address %02X to see if there is a device there", aura_ram_addresses[address_list_idx]);

                        res = busses[bus]->i2c_smbus_write_quick(aura_ram_addresses[address_list_idx], I2C_SMBUS_WRITE);
                    }
                    else
                    {
                        break;
                    }
                } while (res >= 0);

                if(address_list_idx < AURA_RAM_ADDRESS_COUNT)
                {
                    LOG_DEBUG("Remapping slot %d to address %02X", slot, aura_ram_addresses[address_list_idx]);

                    AsusAuraRegisterWrite(busses[bus], 0x77, AURA_REG_SLOT_INDEX, slot);
                    AsusAuraRegisterWrite(busses[bus], 0x77, AURA_REG_I2C_ADDRESS, (aura_ram_addresses[address_list_idx] << 1));
                }
            }

            // Add Aura-enabled controllers at their remapped addresses
            for (unsigned int address_list_idx = 0; address_list_idx < AURA_RAM_ADDRESS_COUNT; address_list_idx++)
            {
                if (TestForAsusAuraSMBusController(busses[bus], aura_ram_addresses[address_list_idx]))
                {
                    AuraSMBusController* controller = new AuraSMBusController(busses[bus], aura_ram_addresses[address_list_idx]);
                    RGBController_AuraSMBus* rgb_controller = new RGBController_AuraSMBus(controller);
                    ResourceManager::get()->RegisterRGBController(rgb_controller);
                }

                std::this_thread::sleep_for(1ms);
            }
        }
    }
}   /* DetectAuraSMBusDRAMControllers() */

void DetectAsusAuraSMBusMotherboardControllers(std::vector<i2c_smbus_interface*> &busses)
{
    AuraSMBusController* new_aura;
    RGBController_AuraSMBus* new_controller;

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        int address_list_idx = -1;

        // Add Aura-enabled motherboard controllers
        IF_MOBO_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
        {
            for (unsigned int address_list_idx = 0; address_list_idx < AURA_MOBO_ADDRESS_COUNT; address_list_idx++)
            {
                if (TestForAsusAuraSMBusController(busses[bus], aura_mobo_addresses[address_list_idx]))
                {
                    DMIInfo dmi;
                    AuraSMBusController* controller = new AuraSMBusController(busses[bus], aura_mobo_addresses[address_list_idx]);
                    RGBController_AuraSMBus* rgb_controller = new RGBController_AuraSMBus(controller);
                    rgb_controller->name = "ASUS " + dmi.getMainboard();
                    ResourceManager::get()->RegisterRGBController(rgb_controller);
                }

                std::this_thread::sleep_for(1ms);
            }
        }
    }

}   /* DetectAuraSMBusMotherboardControllers() */

REGISTER_I2C_DETECTOR("ASUS Aura SMBus DRAM", DetectAsusAuraSMBusDRAMControllers);
REGISTER_I2C_DETECTOR("ASUS Aura SMBus Motherboard", DetectAsusAuraSMBusMotherboardControllers);
