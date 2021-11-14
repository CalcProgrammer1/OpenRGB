#include "Detector.h"
#include "ENESMBusController.h"
#include "LogManager.h"
#include "RGBController.h"
#include "RGBController_ENESMBus.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "dependencies/dmiinfo.h"

#define DETECTOR_NAME   "ENE (ASUS Aura) SMBus Controller"
#define VENDOR_NAME     "ASUS"                                  //This should match the Vendor name from DMI

using namespace std::chrono_literals;

/*----------------------------------------------------------------------*\
| This list contains the available SMBus addresses for mapping ENE RAM   |
\*----------------------------------------------------------------------*/
#define ENE_RAM_ADDRESS_COUNT  23

static const unsigned char ene_ram_addresses[] =
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
*   ENERegisterRead                                                                        *
*                                                                                          *
*       A standalone version of the ENESMBusController::ENERegisterRead function for       *
*       access to ENE devices without instancing the ENESMBusController class or reading   *
*       the config table from the device.                                                  *
*                                                                                          *
\******************************************************************************************/

unsigned char ENERegisterRead(i2c_smbus_interface* bus, ene_dev_id dev, ene_register reg)
{
    //Write ENE register
    bus->i2c_smbus_write_word_data(dev, 0x00, ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF));

    //Read ENE value
    return(bus->i2c_smbus_read_byte_data(dev, 0x81));
}

/******************************************************************************************\
*                                                                                          *
*   ENERegisterWrite                                                                       *
*                                                                                          *
*       A standalone version of the ENESMBusController::ENERegisterWrite function for      *
*       access to ENE devices without instancing the ENESMBusController class or reading   *
*       the config table from the device.                                                  *
*                                                                                          *
\******************************************************************************************/

void ENERegisterWrite(i2c_smbus_interface* bus, ene_dev_id dev, ene_register reg, unsigned char val)
{
    //Write ENE register
    bus->i2c_smbus_write_word_data(dev, 0x00, ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF));

    //Write ENE value
    bus->i2c_smbus_write_byte_data(dev, 0x01, val);
}

/******************************************************************************************\
*                                                                                          *
*   TestForENESMBusController                                                              *
*                                                                                          *
*       Tests the given address to see if an ENE controller exists there.  First does a    *
*       quick write to test for a response, and if so does a simple read at 0xA0 to test   *
*       for incrementing values 0...F which was observed at this location during data dump *
*                                                                                          *
*       Also tests for the string "Micron" in the ENE register space.  Crucial (Micron)    *
*       DRAM modules use an ENE controller with custom, incompatible firmware and must     *
*       be excluded from this controller.                                                  *
*                                                                                          *
\******************************************************************************************/

bool TestForENESMBusController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    LOG_DEBUG("[ENE SMBus] looking for devices at 0x%02X...", address);

    int res = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);

    if (res >= 0)
    {
        pass = true;

        LOG_DEBUG("[ENE SMBus] Detected an I2C device at address %02X, testing register range", address);

        for (int i = 0xA0; i < 0xB0; i++)
        {
            res = bus->i2c_smbus_read_byte_data(address, i);

            if (res != (i - 0xA0))
            {
                LOG_VERBOSE("[ENE SMBus] Detection failed testing register %02X.  Expected %02X, got %02X.", i, (i - 0xA0), res);

                pass = false;
            }
        }

        if(pass)
        {
            LOG_DEBUG("[ENE SMBus] Checking for Micron string");

            char buf[16];
            for(int i = 0; i < 16; i++)
            {
                buf[i] = ENERegisterRead(bus, address, ENE_REG_MICRON_CHECK + i);
            }

            if(strcmp(buf, "Micron") == 0)
            {
                LOG_DEBUG("[ENE SMBus] Device %02X is a Micron device, skipping", address);
                pass = false;
            }
            else
            {
                LOG_VERBOSE("[ENE SMBus] Detection successful, address %02X", address);
            }
        }
    }

    return(pass);

}   /* TestForENESMBusController() */

/******************************************************************************************\
*                                                                                          *
*   DetectENESMBusDRAMControllers                                                          *
*                                                                                          *
*           Detects ENE SMBus controllers on DRAM devices                                  *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where device is connected                 *
*           dev - I2C address of device                                                    *
*                                                                                          *
\******************************************************************************************/

void DetectENESMBusDRAMControllers(std::vector<i2c_smbus_interface*> &busses)
{
    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        int address_list_idx = -1;

        IF_DRAM_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
        {
            LOG_DEBUG("[ENE SMBus DRAM] Remapping ENE SMBus RAM modules on 0x77");

            for (unsigned int slot = 0; slot < 8; slot++)
            {
                int res = busses[bus]->i2c_smbus_write_quick(0x77, I2C_SMBUS_WRITE);

                if (res < 0)
                {
                    LOG_DEBUG("[ENE SMBus DRAM] No device detected at 0x77, aborting remap");

                    break;
                }

                do
                {
                    address_list_idx++;

                    if(address_list_idx < ENE_RAM_ADDRESS_COUNT)
                    {
                        LOG_DEBUG("[ENE SMBus DRAM] Testing address %02X to see if there is a device there", ene_ram_addresses[address_list_idx]);

                        res = busses[bus]->i2c_smbus_write_quick(ene_ram_addresses[address_list_idx], I2C_SMBUS_WRITE);
                    }
                    else
                    {
                        break;
                    }
                } while (res >= 0);

                if(address_list_idx < ENE_RAM_ADDRESS_COUNT)
                {
                    LOG_DEBUG("[ENE SMBus DRAM] Remapping slot %d to address %02X", slot, ene_ram_addresses[address_list_idx]);

                    ENERegisterWrite(busses[bus], 0x77, ENE_REG_SLOT_INDEX, slot);
                    ENERegisterWrite(busses[bus], 0x77, ENE_REG_I2C_ADDRESS, (ene_ram_addresses[address_list_idx] << 1));
                }
            }

            // Add ENE controllers at their remapped addresses
            for (unsigned int address_list_idx = 0; address_list_idx < ENE_RAM_ADDRESS_COUNT; address_list_idx++)
            {
                if (TestForENESMBusController(busses[bus], ene_ram_addresses[address_list_idx]))
                {
                    ENESMBusController* controller = new ENESMBusController(busses[bus], ene_ram_addresses[address_list_idx]);
                    RGBController_ENESMBus* rgb_controller = new RGBController_ENESMBus(controller);
                    ResourceManager::get()->RegisterRGBController(rgb_controller);
                }

                std::this_thread::sleep_for(1ms);
            }
        }
    }
}   /* DetectENESMBusDRAMControllers() */

/******************************************************************************************\
*                                                                                          *
*   DetectENESMBusMotherboardControllers                                                   *
*                                                                                          *
*           Detects ENE (ASUS Aura) SMBus controllers on ASUS motherboard devices          *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where Aura device is connected            *
*           dev - I2C address of Aura device                                               *
*                                                                                          *
\******************************************************************************************/

void DetectENESMBusMotherboardControllers(std::vector<i2c_smbus_interface*> &busses)
{
    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        // Add ENE (ASUS Aura) motherboard controllers
        IF_MOBO_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
        {
            if(busses[bus]->pci_subsystem_vendor == ASUS_SUB_VEN || busses[bus]->pci_subsystem_vendor == 0)
            {
                for (unsigned int address_list_idx = 0; address_list_idx < AURA_MOBO_ADDRESS_COUNT; address_list_idx++)
                {
                    LOG_DEBUG(SMBUS_CHECK_DEVICE_MESSAGE_EN, DETECTOR_NAME, bus, VENDOR_NAME, aura_mobo_addresses[address_list_idx]);

                    if (TestForENESMBusController(busses[bus], aura_mobo_addresses[address_list_idx]))
                    {
                        DMIInfo dmi;
                        ENESMBusController* controller = new ENESMBusController(busses[bus], aura_mobo_addresses[address_list_idx]);
                        RGBController_ENESMBus* rgb_controller = new RGBController_ENESMBus(controller);
                        rgb_controller->name = "ASUS " + dmi.getMainboard();
                        ResourceManager::get()->RegisterRGBController(rgb_controller);
                    }

                    std::this_thread::sleep_for(1ms);
                }
            }
            else
            {
                LOG_DEBUG(SMBUS_CHECK_DEVICE_FAILURE_EN, DETECTOR_NAME, bus, VENDOR_NAME);
            }
        }
    }
}   /* DetectENESMBusMotherboardControllers() */

REGISTER_I2C_DETECTOR("ENE SMBus DRAM",                 DetectENESMBusDRAMControllers);
REGISTER_I2C_DETECTOR("ASUS Aura SMBus Motherboard",    DetectENESMBusMotherboardControllers);
