/*---------------------------------------------------------*\
| ENESMBusControllerDetect.cpp                              |
|                                                           |
|   Detector for ENE SMBus devices                          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <vector>
#include "Detector.h"
#include "ENESMBusController.h"
#include "ENESMBusInterface_i2c_smbus.h"
#include "LogManager.h"
#include "RGBController.h"
#include "RGBController_ENESMBus.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include "dmiinfo.h"

#define DETECTOR_NAME   "ENE (ASUS Aura) SMBus Controller"
#define VENDOR_NAME     "ASUS"                                  //This should match the Vendor name from DMI

using namespace std::chrono_literals;

/*----------------------------------------------------------------------*\
| Windows defines "interface" for some reason. Work around this          |
\*----------------------------------------------------------------------*/
#ifdef interface
#undef interface
#endif

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

static unsigned char ENERegisterRead(i2c_smbus_interface* bus, ene_dev_id dev, ene_register reg)
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

static void ENERegisterWrite(i2c_smbus_interface* bus, ene_dev_id dev, ene_register reg, unsigned char val)
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

    int res = bus->i2c_smbus_read_byte(address);

    if(res < 0)
    {
        res = bus->i2c_smbus_read_byte_data(address, 0x00);
    }

    if(res >= 0)
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
                break;
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

                if(res < 0)
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
                    ENESMBusInterface_i2c_smbus* interface      = new ENESMBusInterface_i2c_smbus(busses[bus]);
                    ENESMBusController*          controller     = new ENESMBusController(interface, ene_ram_addresses[address_list_idx], "ENE DRAM", DEVICE_TYPE_DRAM);
                    RGBController_ENESMBus*      rgb_controller = new RGBController_ENESMBus(controller);

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

                        ENESMBusInterface_i2c_smbus* interface      = new ENESMBusInterface_i2c_smbus(busses[bus]);
                        ENESMBusController*          controller     = new ENESMBusController(interface, aura_mobo_addresses[address_list_idx], "ASUS " + dmi.getMainboard(), DEVICE_TYPE_MOTHERBOARD);
                        RGBController_ENESMBus*      rgb_controller = new RGBController_ENESMBus(controller);

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

/******************************************************************************************\
*                                                                                          *
*   DetectENESMBusGPUControllers                                                           *
*                                                                                          *
*           Detects ENE (ASUS Aura) SMBus controllers on ASUS GPU devices                  *
*                                                                                          *
\******************************************************************************************/

#define GPU_CHECK_DEVICE_MESSAGE_EN     "[%s] Bus %02d is a GPU and the subvendor matches the one for %s, looking for a device at 0x%02X"

void DetectENESMBusGPUControllers(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name)
{
    if(TestForENESMBusController(bus, i2c_addr))
    {
        ENESMBusInterface_i2c_smbus* interface      = new ENESMBusInterface_i2c_smbus(bus);
        ENESMBusController*          controller     = new ENESMBusController(interface, i2c_addr, name, DEVICE_TYPE_GPU);
        RGBController_ENESMBus*      rgb_controller = new RGBController_ENESMBus(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
    else
    {
        LOG_DEBUG("[ENE SMBus ASUS GPU] Testing for controller at %d failed", i2c_addr);
    }
} /* DetectENESMBusGPUControllers() */

REGISTER_I2C_DETECTOR("ENE SMBus DRAM",                 DetectENESMBusDRAMControllers);
REGISTER_I2C_DETECTOR("ASUS Aura SMBus Motherboard",    DetectENESMBusMotherboardControllers);

/*-----------------------------------------*\
|  Nvidia GPUs                              |
\*-----------------------------------------*/

REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 3050 Gaming",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3050_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX3050_8G_GAMING,               0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS KO GeForce RTX 3060 Gaming OC",                         DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060_DEV,         ASUS_SUB_VEN,   ASUS_KO_RTX_3060_OC_O12G_GAMING,                0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 3060 Gaming OC",                        DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_3060_O12G_GAMING,                  0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 3060 V2 Gaming OC",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060_GA104_DEV,   ASUS_SUB_VEN,   ASUS_TUF_RTX_3060_O12G_V2_GAMING,               0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 3060 V2 Gaming OC",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060_LHR_DEV,     ASUS_SUB_VEN,   ASUS_TUF_RTX_3060_O12G_LHR_GAMING,              0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 3060 Gaming",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3060_12G_GAMING,             0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 3060 Gaming OC",                  DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3060_O12G_GAMING,            0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 3060 V2 Gaming OC",               DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060_LHR_DEV,     ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3060_O12G_LHR_GAMING,        0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS KO GeForce RTX 3060 V2 Gaming OC",                      DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060_LHR_DEV,     ASUS_SUB_VEN,   ASUS_KO_RTX_3060_O12G_V2_GAMING,                0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS KO GeForce RTX 3060 Ti V2 Gaming OC",                   DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060TI_LHR_DEV,   ASUS_SUB_VEN,   ASUS_KO_RTX3060TI_O8G_V2_GAMING,                0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 3060 Ti OC",                            DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060TI_GDDR6X_DEV,ASUS_SUB_VEN,   ASUS_TUF_RTX_3060TI_O8G,                        0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS KO GeForce RTX 3060 Ti Gaming OC",                      DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060TI_DEV,       ASUS_SUB_VEN,   ASUS_KO_RTX3060TI_O8G_GAMING,                   0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS KO GeForce RTX 3060 Ti Gaming OC",                      DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060TI_DEV,       ASUS_SUB_VEN,   ASUS_KO_RTX3060TI_08G_GAMING_2,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 3060 Ti Gaming OC",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060TI_DEV,       ASUS_SUB_VEN,   ASUS_TUF_RTX_3060TI_O8G_OC,                     0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 3060 Ti OC",                      DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060TI_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3060TI_O8G_OC,               0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 3060 Ti OC",                            DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060TI_LHR_DEV,   ASUS_SUB_VEN,   ASUS_TUF_RTX_3060TI_O8G_OC_V2,                  0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 3060 Ti V2 OC",                   DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060TI_LHR_DEV,   ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3060TI_O8G_V2_GAMING,        0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 3070 OC",                         DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3070_OC,                     0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 3070 Gaming OC",                  DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3070_O8G_GAMING,             0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 3070 White OC",                   DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3070_O8G_WHITE,              0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 3070 V2 Gaming",                  DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070_LHR_DEV,     ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3070_8G_V2_GAMING,           0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 3070 V2 Gaming OC",               DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070_LHR_DEV,     ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3070_O8G_V2_GAMING,          0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 3070 V2 White OC",                DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070_LHR_DEV,     ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3070_O8G_V2_WHITE,           0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 3070 Gaming",                           DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_3070_8G_GAMING,                    0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 3070 Gaming OC",                        DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_3070_O8G_GAMING,                   0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS KO GeForce RTX 3070 Gaming OC",                         DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070_DEV,         ASUS_SUB_VEN,   ASUS_KO_RTX_3070_O8G_GAMING_V1,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS KO GeForce RTX 3070 V2 Gaming OC",                      DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070_LHR_DEV,     ASUS_SUB_VEN,   ASUS_KO_RTX_3070_O8G_GAMING,                    0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS KO GeForce RTX 3070 V2 Gaming OC",                      DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070_LHR_DEV,     ASUS_SUB_VEN,   ASUS_KO_RTX_3070_O8G_GAMING_V2,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 3070 V2 Gaming OC",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070_LHR_DEV,     ASUS_SUB_VEN,   ASUS_TUF_RTX_3070_O8G_V2_GAMING,                0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 3070 Ti Gaming OC",               DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070TI_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3070TI_O8G_GAMING,           0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 3070 Ti V2 Gaming OC",                  DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070TI_DEV,       ASUS_SUB_VEN,   ASUS_TUF_RTX_3070TI_O8G_V2_GAMING,              0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 3070 Ti V2 Gaming OC",                  DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070TI_GA102_DEV, ASUS_SUB_VEN,   ASUS_TUF_RTX_3070TI_O8G_V2_GAMING_2,            0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 3070 Ti V2 Gaming OC",                  DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070TI_GA102_DEV, ASUS_SUB_VEN,   ASUS_TUF_RTX_3070TI_O8G_V2_GAMING_3,            0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 3070 Ti Gaming OC",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070TI_DEV,       ASUS_SUB_VEN,   ASUS_TUF_RTX_3070TI_O8G_GAMING,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 3080 Gaming",                           DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_3080_10G_GAMING_PD,                0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 3080 Gaming",                           DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_3080_10G_GAMING,                   0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG GeForce RTX 3080 GUNDAM EDITION",                   DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_3080_10G_GUNDAM_EDITION,           0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 3080 Gaming OC",                        DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_3080_O10G_OC,                      0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 3080 Gaming",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3080_10G_GAMING,             0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 3080 Gaming OC",                  DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3080_O10G_GAMING,            0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 3080 White OC",                   DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3080_O10G_WHITE,             0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 3080 V2 Gaming",                  DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_LHR_DEV,     ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3080_10G_V2_GAMING,          0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 3080 V2 Gaming OC",               DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_LHR_DEV,     ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3080_O10G_V2_GAMING,         0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 3080 V2 White OC",                DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_LHR_DEV,     ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3080_O10G_V2_WHITE,          0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 3080 V2 Gaming OC ",                    DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_LHR_DEV,     ASUS_SUB_VEN,   ASUS_TUF_RTX_3080_O10G_V2_GAMING_8822,          0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 3080 V2 Gaming OC",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_LHR_DEV,     ASUS_SUB_VEN,   ASUS_TUF_RTX_3080_O10G_V2_GAMING_882B,          0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 3080 V2 Gaming OC",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_LHR_DEV,     ASUS_SUB_VEN,   ASUS_TUF_RTX_3080_O10G_V2_GAMING,               0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 3080 12G Gaming",                       DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_12G_LHR_DEV, ASUS_SUB_VEN,   ASUS_TUF_RTX_3080_12G_GAMING,                   0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 3080 12G Gaming OC",                    DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_12G_LHR_DEV, ASUS_SUB_VEN,   ASUS_TUF_RTX_3080_O12G_GAMING,                  0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 3080 12G",                        DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_12G_LHR_DEV, ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3080_12G,                    0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 3080 12G OC",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_12G_LHR_DEV, ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3080_O12G_OC,                0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 3080 12G OC EVA EDITION",         DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_12G_LHR_DEV, ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3080_O12G_EVA,               0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 3090 OC EVA EDITION",             DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3090_O24G_EVA,               0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 3090 GUNDAM EDITION",             DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3090_GUNDAM_EDITION,         0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 3080 Ti Gaming",                        DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080TI_DEV,       ASUS_SUB_VEN,   ASUS_TUF_RTX_3080TI_12G_GAMING,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 3080 Ti Gaming OC",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080TI_DEV,       ASUS_SUB_VEN,   ASUS_TUF_RTX_3080TI_O12G_GAMING,                0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG GeForce RTX STRIX 3080 Ti Gaming OC",               DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080TI_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3080TI_O12G_GAMING,          0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX LC GeForce RTX 3080 Ti Gaming OC",            DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080TI_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3080TI_O12G_GAMING_LC,       0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 3090 Gaming",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3090_24G_GAMING,             0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 3090 Gaming",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3090_24G_GAMING_V2,          0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 3090 Gaming OC",                  DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3090_O24G_GAMING,            0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 3090 Gaming White OC",            DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3090_O24G_GAMING_WHITE,      0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 3090 Gaming OC",                        DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3090_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_3090_O24G,                         0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 3090 Gaming OC",                        DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3090_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_3090_O24G_OC,                      0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 3090 Ti Gaming",                        DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3090TI_DEV,       ASUS_SUB_VEN,   ASUS_TUF_RTX_3090TI_24G_GAMING,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 3090 Ti Gaming OC",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3090TI_DEV,       ASUS_SUB_VEN,   ASUS_TUF_RTX_3090TI_O24G_OC_GAMING,             0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX LC GeForce RTX 3090 Ti Gaming OC",            DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3090TI_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_LC_RTX_3090TI_O24G_GAMING,       0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 4060 Ti Gaming OC",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4060TI_DEV,       ASUS_SUB_VEN,   ASUS_TUF_RTX_4060TI_O8G_GAMING,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 4060 Gaming OC",                  DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4060_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4060_O8G_GAMING,             0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 4060 Ti Gaming OC",               DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4060TI_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4060TI_O8G_GAMING,           0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 4070 Gaming",                           DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_4070_12G_GAMING,                   0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 4070 Gaming OC",                        DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_4070_O12G_GAMING,                  0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 4070 Gaming OC",                        DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_4070_O12G_GAMING_2,                0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 4070 Gaming OC",                        DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_4070_O12G_GAMING_3,                0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 4070 SUPER Gaming",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070S_DEV,        ASUS_SUB_VEN,   ASUS_TUF_RTX_4070S_12G_GAMING,                  0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 4070 Gaming OC",                  DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4070_O12G_GAMING,            0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 4070 SUPER Gaming",               DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070S_DEV,        ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4070S_12G_GAMING,            0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 4070 SUPER Gaming OC",            DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070S_DEV,        ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4070S_O12G_GAMING,           0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 4070 Ti Gaming",                        DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070TI_DEV,       ASUS_SUB_VEN,   ASUS_TUF_RTX_4070TI_12G_GAMING,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 4070 Ti Gaming",                        DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070TI_DEV,       ASUS_SUB_VEN,   ASUS_TUF_RTX_4070TI_12G_GAMING_88DD,            0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 4070 Ti Gaming OC",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070TI_DEV,       ASUS_SUB_VEN,   ASUS_TUF_RTX_4070TI_O12G_GAMING,                0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 4070 Ti Gaming OC",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070TI_DEV,       ASUS_SUB_VEN,   ASUS_TUF_RTX_4070TI_O12G_GAMING_88DC,           0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 4070 Ti Gaming White OC",               DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070TI_DEV,       ASUS_SUB_VEN,   ASUS_TUF_RTX_4070TI_O12G_GAMING_WHITE,          0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 4070 Ti SUPER Gaming OC",               DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070TIS_DEV,      ASUS_SUB_VEN,   ASUS_TUF_RTX_4070TI_SUPER_16G_GAMING,           0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 4070 Ti SUPER Gaming OC",               DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070TIS_DEV,      ASUS_SUB_VEN,   ASUS_TUF_RTX_4070TI_SUPER_O16G_GAMING,          0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 4070 Ti SUPER Gaming White OC",         DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070TIS_DEV,      ASUS_SUB_VEN,   ASUS_TUF_RTX_4070TI_SUPER_O16G_GAMING_WHITE,    0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 4070 Ti Gaming",                  DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070TI_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4070TI_12G_GAMING,           0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 4070 Ti Gaming OC",               DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070TI_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4070TI_O12G_GAMING,          0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 4070 Ti Gaming OC",               DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070TI_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4070TI_O12G_GAMING_2,        0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 4070 Ti SUPER Gaming",            DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070TIS_DEV,      ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4070TI_SUPER_16G_GAMING,     0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 4070 Ti SUPER Gaming OC",         DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070TIS_DEV,      ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4070TI_SUPER_O16G_GAMING,    0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 4080 Gaming",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4080_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4080_16G_GAMING,             0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 4080 Gaming White",               DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4080_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4080_16G_GAMING_WHITE,       0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 4080 Gaming White OC",            DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4080_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4080_O16G_GAMING_WHITE,      0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 4080 Gaming OC",                  DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4080_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4080_O16G_GAMING,            0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 4080 Gaming OC",                  DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4080_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4080_O16G_GAMING_2,          0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 4080 Gaming",                           DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4080_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_4080_16G_GAMING,                   0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 4080 Gaming",                           DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4080_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_4080_16G_GAMING_2,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 4080 Gaming OC",                        DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4080_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_4080_O16G_GAMING,                  0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 4080 Gaming OC",                        DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4080_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_4080_O16G_OC_GAMING,               0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 4080 SUPER Gaming",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4080S_DEV,        ASUS_SUB_VEN,   ASUS_TUF_RTX_4080S_16G_GAMING,                  0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 4080 SUPER OC",                   DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4080S_DEV,        ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4080S_016G_GAMING,           0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 4080 SUPER White",                DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4080S_DEV,        ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4080S_16G_GAMING_WHITE,      0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 4080 SUPER White OC",             DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4080S_DEV,        ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4080S_016G_GAMING_WHITE,     0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 4080 SUPER Gaming OC",                  DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4080S_DEV,        ASUS_SUB_VEN,   ASUS_TUF_RTX_4080S_O16G_OC_GAMING,              0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 4080 SUPER Gaming OC",                  DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4080S_DEV,        ASUS_SUB_VEN,   ASUS_TUF_RTX_4080S_O16G_OC_GAMING_2,            0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 4090 Gaming OC",                        DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_4090_O24G_OC_GAMING,               0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 4090 Gaming OG OC",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_4090_O24G_OG_OC_GAMING,            0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 4090 Gaming OC",                        DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_4090_O24G_GAMING,                  0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 4090 Gaming OC",                        DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_4090_O24G_GAMING_2,                0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 4090 Gaming OC",                        DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_4090_O24G_GAMING_3,                0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX LC GeForce RTX 4090 Gaming OC",               DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_LC_RTX_4090_O24G_GAMING,         0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 4090 Gaming",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4090_24G_GAMING,             0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 4090 Gaming OC",                  DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4090_O24G_GAMING,            0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 4090 Gaming OC",                  DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4090_O24G_GAMING_213S,       0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 4090 Gaming",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4090_24G_GAMING_88F0,        0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 4090 OC EVA-02",                  DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4090_024G_EVA_02,            0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 4090 Gaming OC",                  DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4090_O24G_GAMING_8932,       0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 4090 Gaming White",               DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4090_24G_GAMING_WHITE,       0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 4090 Gaming White",               DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4090_24G_GAMING_WHITE_2,     0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 4090 Gaming White OC",            DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4090_O24G_GAMING_WHITE,      0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 4090 Gaming White OC",            DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4090_O24G_GAMING_WHITE_2,    0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG MATRIX PLATINUM GeForce RTX 4090",                  DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_MATRIX_PLATINUM_RTX_4090_24G,          0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 5070 Gaming",                           DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX5070_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_5070_O12G_GAMING,                  0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 5070 Ti Gaming OC",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX5070TI_DEV,       ASUS_SUB_VEN,   ASUS_TUF_RTX_5070TI_O16G_GAMING,                0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX GeForce RTX 5070 Ti Gaming OC",               DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX5070TI_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_5070TI_O16G_GAMING_OC,       0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 5080 Gaming OC",                        DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX5080_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_5080_O16G_GAMING,                  0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 5090 Gaming OC",                        DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX5090_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_5090_O32G_GAMING,                  0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF GeForce RTX 5090 Gaming",                           DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX5090_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_5090_32G_GAMING,                   0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG ASTRAL GeForce RTX 5080 OC",                        DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX5080_DEV,         ASUS_SUB_VEN,   ASUS_ROG_ASTRAL_RTX_5080_O16G_GAMING,           0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG ASTRAL GeForce RTX 5080",                           DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX5080_DEV,         ASUS_SUB_VEN,   ASUS_ROG_ASTRAL_RTX_5080_16G_GAMING,            0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG ASTRAL GeForce RTX 5090 OC",                        DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX5090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_ASTRAL_RTX_5090_O32G_GAMING,           0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG ASTRAL GeForce RTX 5090 OC BTF",                    DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX5090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_ASTRAL_RTX_5090_O32G_GAMING_BTF,       0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG ASTRAL GeForce RTX 5090 OC WHITE",                  DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX5090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_ASTRAL_RTX_5090_O32G_GAMING_WHITE,     0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG ASTRAL LC GeForce RTX 5090 OC",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX5090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_ASTRAL_LC_RTX_5090_O32G_GAMING,        0x67);

/*-----------------------------------------*\
|  AMD GPUs                                 |
\*-----------------------------------------*/

REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX Radeon RX 6600 XT Gaming OC",                 DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI23_DEV,             ASUS_SUB_VEN,   ASUS_ROG_STRIX_RX_6600XT_O8G_GAMING,            0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX Radeon RX 6650 XT Gaming",                    DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI23_DEV1,            ASUS_SUB_VEN,   ASUS_ROG_STRIX_RX_6650XT_O8G_GAMING,            0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF Radeon RX 6700 XT Gaming OC",                       DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI22_DEV,             ASUS_SUB_VEN,   ASUS_TUF_RX_6700XT_O12G_GAMING,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX Radeon RX 6700 XT Gaming OC",                 DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI22_DEV,             ASUS_SUB_VEN,   ASUS_ROG_STRIX_RX_6700XT_O12G_GAMING,           0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX Radeon RX 6750 XT Gaming OC",                 DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI22_DEV,             ASUS_SUB_VEN,   ASUS_ROG_STRIX_RX_6750XT_O12G_GAMING,           0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF Radeon RX 6800 Gaming OC",                          DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI21_DEV1,            ASUS_SUB_VEN,   ASUS_RX6800_TUF_GAMING_OC,                      0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX Radeon RX 6800 Gaming OC",                    DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI21_DEV1,            ASUS_SUB_VEN,   ASUS_ROG_STRIX_RX_6800_O16G_GAMING,             0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF Radeon RX 6800 XT Gaming OC",                       DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI21_DEV1,            ASUS_SUB_VEN,   ASUS_TUF_RX_6800XT_O16G_GAMING,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX LC Radeon RX 6800 XT Gaming OC",              DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI21_DEV1,            ASUS_SUB_VEN,   ASUS_ROG_STRIX_LC_RX6800XT_O16G_GAMING,         0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX LC Radeon RX 6900 XT Gaming OC",              DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI21_DEV1,            ASUS_SUB_VEN,   ASUS_ROG_STRIX_LC_RX6900XT_O16G_GAMING,         0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX LC Radeon RX 6900 XT Gaming OC TOP",          DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI21_DEV2,            ASUS_SUB_VEN,   ASUS_ROG_STRIX_LC_RX6900XT_O16G_GAMING_TOP,     0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF Radeon RX 6900 XT Gaming OC",                       DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI21_DEV1,            ASUS_SUB_VEN,   ASUS_TUF_RX_6900XT_O16G_GAMING,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF Radeon RX 6900 XT T16G Gaming",                     DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI21_DEV2,            ASUS_SUB_VEN,   ASUS_TUF_RX_6900XT_T16G_GAMING,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF Radeon RX 6950 XT Gaming OC",                       DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI21_DEV3,            ASUS_SUB_VEN,   ASUS_TUF_RX_6950XT_016G_GAMING,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX LC Radeon RX 6950 XT Gaming OC",              DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI21_DEV3,            ASUS_SUB_VEN,   ASUS_ROG_STRIX_LC_RX_6950XT_O16G_GAMING,        0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF Radeon RX 7600 XT O16G Gaming",                     DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI33_DEV,             ASUS_SUB_VEN,   ASUS_TUF_RX_7600XT_O16G_GAMING,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF Radeon RX 7700 XT Gaming OC",                       DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI32_DEV,             ASUS_SUB_VEN,   ASUS_TUF_RX_7700XT_012G_GAMING,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF Radeon RX 7800 XT Gaming OC",                       DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI32_DEV,             ASUS_SUB_VEN,   ASUS_TUF_RX_7800XT_O16G_GAMING,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF Radeon RX 7800 XT Gaming OC",                       DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI32_DEV,             ASUS_SUB_VEN,   ASUS_TUF_RX_7800XT_O16G_GAMING_0606,            0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF Radeon RX 7800 XT Gaming White OC",                 DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI32_DEV,             ASUS_SUB_VEN,   ASUS_TUF_RX_7800XT_O16G_WHITE_GAMING,           0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF Radeon RX 7900 XT Gaming OC",                       DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI31_DEV,             ASUS_SUB_VEN,   ASUS_TUF_RX_7900XT_020G_GAMING,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF Radeon RX 7900 XTX Gaming OC",                      DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI31_DEV,             ASUS_SUB_VEN,   ASUS_TUF_RX_7900XTX_O24G_GAMING,                0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF Radeon RX 9070 XT Gaming OC",                       DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI48_DEV,             ASUS_SUB_VEN,   ASUS_TUF_RX_9070XT_016G_GAMING,                 0x67);
