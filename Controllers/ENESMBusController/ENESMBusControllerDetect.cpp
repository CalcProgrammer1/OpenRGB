/*---------------------------------------------------------*\
| ENESMBusControllerDetect.cpp                              |
|                                                           |
|   Detector for ENE SMBus devices                          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
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
| Windows defines "interface" for some reason.  Work around this         |
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
                    ENESMBusInterface_i2c_smbus* interface      = new ENESMBusInterface_i2c_smbus(busses[bus]);
                    ENESMBusController*          controller     = new ENESMBusController(interface, ene_ram_addresses[address_list_idx]);
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
                        ENESMBusController*          controller     = new ENESMBusController(interface, aura_mobo_addresses[address_list_idx]);
                        RGBController_ENESMBus*      rgb_controller = new RGBController_ENESMBus(controller);

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
        ENESMBusController*          controller     = new ENESMBusController(interface, i2c_addr);
        RGBController_ENESMBus*      rgb_controller = new RGBController_ENESMBus(controller);

        rgb_controller->name                        = name;
        rgb_controller->type                        = DEVICE_TYPE_GPU;

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
|  NVidia GPUs                              |
\*-----------------------------------------*/

REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 3050 8G GAMING",                  DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3050_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX3050_8G_GAMING,               0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS KO RTX 3060 OC O12G GAMING",                    DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060_DEV,         ASUS_SUB_VEN,   ASUS_KO_RTX_3060_OC_O12G_GAMING,                0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF 3060 O12G GAMING",                          DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_3060_O12G_GAMING,                  0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF 3060 O12G V2 GAMING",                       DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060_GA104_DEV,   ASUS_SUB_VEN,   ASUS_TUF_RTX_3060_O12G_V2_GAMING,               0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF 3060 O12G V2 GAMING",                       DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060_LHR_DEV,     ASUS_SUB_VEN,   ASUS_TUF_RTX_3060_O12G_LHR_GAMING,              0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX 3060 12G GAMING",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3060_12G_GAMING,             0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX 3060 O12G GAMING",                    DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3060_O12G_GAMING,            0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX 3060 O12G V2 GAMING",                 DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060_LHR_DEV,     ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3060_O12G_LHR_GAMING,        0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS KO RTX 3060 O12G V2 GAMING",                    DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060_LHR_DEV,     ASUS_SUB_VEN,   ASUS_KO_RTX_3060_O12G_V2_GAMING,                0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS KO RTX 3060 Ti O8G V2 GAMING",                  DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060TI_LHR_DEV,   ASUS_SUB_VEN,   ASUS_KO_RTX3060TI_O8G_V2_GAMING,                0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 3060 Ti O8G",                           DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060TI_GDDR6X_DEV,ASUS_SUB_VEN,   ASUS_TUF_RTX_3060_TI_O8G,                       0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS KO RTX 3060 Ti O8G GAMING",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060TI_DEV,       ASUS_SUB_VEN,   ASUS_KO_RTX3060TI_O8G_GAMING,                   0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS KO RTX 3060 Ti O8G GAMING",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060TI_DEV,       ASUS_SUB_VEN,   ASUS_KO_RTX3060TI_08G_GAMING_2,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 3060 Ti O8G OC",                        DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060TI_DEV,       ASUS_SUB_VEN,   ASUS_TUF_RTX_3060_TI_O8G_OC,                    0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX 3060 Ti O8G OC",                      DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060TI_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_3060_TI_O8G_OC,                  0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 3060 Ti O8G OC",                        DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060TI_LHR_DEV,   ASUS_SUB_VEN,   ASUS_TUF_RTX_3060_TI_O8G_OC_V2,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX 3060 Ti O8G V2",                      DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060TI_LHR_DEV,   ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3060TI_O8G_V2_GAMING,        0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX 3070 OC",                             DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3070_OC,                     0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX 3070 O8G GAMING",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3070_O8G_GAMING,             0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX 3070 O8G White",                      DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3070_O8G_WHITE,              0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX 3070 O8G V2 GAMING",                  DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070_LHR_DEV,     ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3070_O8G_V2_GAMING,          0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX 3070 O8G V2 White",                   DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070_LHR_DEV,     ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3070_O8G_V2_WHITE,           0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 3070 8G GAMING",                        DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_3070_8G_GAMING,                    0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 3070 O8G GAMING",                       DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_3070_O8G_GAMING,                   0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS KO RTX 3070 O8G GAMING",                        DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070_DEV,         ASUS_SUB_VEN,   ASUS_KO_RTX_3070_O8G_GAMING_V1,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS KO RTX 3070 O8G V2 GAMING",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070_LHR_DEV,     ASUS_SUB_VEN,   ASUS_KO_RTX_3070_O8G_GAMING,                    0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS KO RTX 3070 O8G V2 GAMING",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070_LHR_DEV,     ASUS_SUB_VEN,   ASUS_KO_RTX_3070_O8G_GAMING_V2,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 3070 O8G V2 GAMING",                    DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070_LHR_DEV,     ASUS_SUB_VEN,   ASUS_TUF_RTX_3070_O8G_V2_GAMING,                0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX 3070 Ti O8G GAMING",                  DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070TI_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3070TI_O8G_GAMING,           0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 3070 Ti O8G V2 GAMING",                 DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070TI_DEV,       ASUS_SUB_VEN,   ASUS_TUF_RTX_3070TI_O8G_V2_GAMING,              0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 3070 Ti O8G V2 GAMING",                 DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070TI_GA102_DEV, ASUS_SUB_VEN,   ASUS_TUF_RTX_3070TI_O8G_V2_GAMING_2,            0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 3070 Ti O8G V2 GAMING",                 DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070TI_GA102_DEV, ASUS_SUB_VEN,   ASUS_TUF_RTX_3070TI_O8G_V2_GAMING_3,            0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 3070 Ti O8G GAMING",                    DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3070TI_DEV,       ASUS_SUB_VEN,   ASUS_TUF_RTX_3070TI_O8G_GAMING,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 3080 10G GAMING",                       DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_3080_10G_GAMING_PD,                0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 3080 10G GAMING",                       DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_3080_10G_GAMING,                   0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG RTX 3080 10G GUNDAM EDITION",               DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_3080_10G_GUNDAM_EDITION,           0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 3080 O10G OC",                          DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_3080_O10G_OC,                      0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX 3080 10G GAMING",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3080_10G_GAMING,             0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX 3080 O10G GAMING",                    DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3080_O10G_GAMING,            0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX 3080 O10G WHITE",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3080_O10G_WHITE,             0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX 3080 10G V2 GAMING",                  DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_LHR_DEV,     ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3080_10G_V2_GAMING,          0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX 3080 O10G V2 GAMING",                 DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_LHR_DEV,     ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3080_O10G_V2_GAMING,         0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX 3080 O10G V2 WHITE",                  DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_LHR_DEV,     ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3080_O10G_V2_WHITE,          0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 3080 O10G V2 GAMING",                   DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_LHR_DEV,     ASUS_SUB_VEN,   ASUS_TUF_RTX_3080_O10G_V2_GAMING_8822,          0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 3080 O10G V2 GAMING",                   DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_LHR_DEV,     ASUS_SUB_VEN,   ASUS_TUF_RTX_3080_O10G_V2_GAMING_882B,          0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 3080 O10G V2 GAMING",                   DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_LHR_DEV,     ASUS_SUB_VEN,   ASUS_TUF_RTX_3080_O10G_V2_GAMING,               0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 3080 12G GAMING",                       DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_12G_LHR_DEV, ASUS_SUB_VEN,   ASUS_TUF_RTX_3080_12G_GAMING,                   0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 3080 O12G GAMING",                      DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_12G_LHR_DEV, ASUS_SUB_VEN,   ASUS_TUF_RTX_3080_O12G_GAMING,                  0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 3080 12G",                        DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_12G_LHR_DEV, ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3080_12G,                    0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 3080 O12G",                       DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_12G_LHR_DEV, ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3080_O12G_OC,                0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 3080 O12G EVA EDITION",           DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080_12G_LHR_DEV, ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3080_O12G_EVA,               0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX 3090 O24G EVA",                       DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3090_O24G_EVA,               0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX 3090 GUNDAM EDITION",                 DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3090_GUNDAM_EDITION,         0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 3080 Ti 12G GAMING",                    DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080TI_DEV,       ASUS_SUB_VEN,   ASUS_TUF_RTX_3080TI_12G_GAMING,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 3080 Ti O12G GAMING",                   DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080TI_DEV,       ASUS_SUB_VEN,   ASUS_TUF_RTX_3080TI_O12G_GAMING,                0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX 3080 Ti O12G GAMING",                 DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080TI_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3080TI_O12G_GAMING,          0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX LC 3080 Ti O12G GAMING",              DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3080TI_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3080TI_O12G_GAMING_LC,       0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX 3090 24G GAMING",                     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3090_24G_GAMING,             0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX 3090 O24G GAMING",                    DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3090_O24G_GAMING,            0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX 3090 O24G GAMING White OC",           DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_3090_O24G_GAMING_WHITE,      0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 3090 O24G",                             DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3090_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_3090_O24G,                         0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 3090 O24G OC",                          DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3090_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_3090_O24G_OC,                      0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 3090 Ti 24G GAMING",                    DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3090TI_DEV,       ASUS_SUB_VEN,   ASUS_TUF_RTX_3090TI_24G_GAMING,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 3090 Ti O24G OC GAMING",                DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3090TI_DEV,       ASUS_SUB_VEN,   ASUS_TUF_RTX_3090TI_O24G_OC_GAMING,             0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX LC RTX 3090 Ti O24G OC GAMING",       DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3090TI_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_LC_RTX_3090TI_O24G_GAMING,       0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 4060 Ti 8G Gaming OC",                  DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4060TI_DEV,       ASUS_SUB_VEN,   ASUS_TUF_RTX_4060TI_O8G_GAMING,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 4060 Ti 8G Gaming OC",            DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4060TI_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4060TI_O8G_GAMING,           0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 4070 12G Gaming",                       DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_4070_12G_GAMING,                   0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 4070 O12G Gaming",                      DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_4070_O12G_GAMING,                  0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 4070 O12G Gaming",                      DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_4070_O12G_GAMING_2,                0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 4070 O12G Gaming",                      DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_4070_O12G_GAMING_3,                0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 4070 SUPER 12G Gaming",                 DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070S_DEV,        ASUS_SUB_VEN,   ASUS_TUF_RTX_4070S_12G_GAMING,                  0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 4070 O12G Gaming",                DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4070_O12G_GAMING,            0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 4070 SUPER O12G Gaming",          DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070S_DEV,        ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4070S_O12G_GAMING,           0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 4070 Ti 12G Gaming",                    DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070TI_DEV,       ASUS_SUB_VEN,   ASUS_TUF_RTX_4070TI_12G_GAMING,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 4070 Ti 12G Gaming",                    DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070TI_DEV,       ASUS_SUB_VEN,   ASUS_TUF_RTX_4070TI_12G_GAMING_88DD,            0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 4070 Ti O12G Gaming",                   DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070TI_DEV,       ASUS_SUB_VEN,   ASUS_TUF_RTX_4070TI_O12G_GAMING,                0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 4070 Ti O12G Gaming",                   DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070TI_DEV,       ASUS_SUB_VEN,   ASUS_TUF_RTX_4070TI_O12G_GAMING_88DC,           0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 4070 Ti O12G Gaming White",             DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070TI_DEV,       ASUS_SUB_VEN,   ASUS_TUF_RTX_4070TI_O12G_GAMING_WHITE,          0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 4070 Ti SUPER 16G Gaming",              DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070TIS_DEV,      ASUS_SUB_VEN,   ASUS_TUF_RTX_4070TI_SUPER_16G_GAMING,           0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 4070 Ti SUPER O16G Gaming",             DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070TIS_DEV,      ASUS_SUB_VEN,   ASUS_TUF_RTX_4070TI_SUPER_O16G_GAMING,          0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 4070 Ti SUPER O16G Gaming White",       DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070TIS_DEV,      ASUS_SUB_VEN,   ASUS_TUF_RTX_4070TI_SUPER_O16G_GAMING_WHITE,    0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 4070 Ti O12G Gaming",             DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070TI_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4070TI_O12G_GAMING,          0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 4070 Ti O12G Gaming",             DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070TI_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4070TI_O12G_GAMING_2,        0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 4070 Ti SUPER 16G Gaming",        DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070TIS_DEV,      ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4070TI_SUPER_16G_GAMING,     0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 4070 Ti SUPER O16G Gaming",       DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4070TIS_DEV,      ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4070TI_SUPER_O16G_GAMING,    0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 4080 16G GAMING",                 DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4080_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4080_16G_GAMING,             0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 4080 16G GAMING White",           DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4080_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4080_16G_GAMING_WHITE,       0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 4080 O16G GAMING WHITE",          DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4080_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4080_O16G_GAMING_WHITE,      0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 4080 O16G GAMING",                DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4080_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4080_O16G_GAMING,            0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 4080 O16G GAMING",                DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4080_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4080_O16G_GAMING_2,          0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 4080 16G GAMING",                       DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4080_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_4080_16G_GAMING,                   0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 4080 16G GAMING",                       DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4080_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_4080_16G_GAMING_2,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 4080 O16G GAMING",                      DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4080_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_4080_O16G_GAMING,                  0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 4080 O16G OC",                          DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4080_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_4080_O16G_OC_GAMING,               0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 4080 SUPER 16G GAMING",                 DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4080S_DEV,        ASUS_SUB_VEN,   ASUS_TUF_RTX_4080S_16G_GAMING,                  0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 4080 SUPER O16G GAMING",          DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4080S_DEV,        ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4080S_016G_GAMING,           0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 4080 SUPER 16G GAMING WHITE",     DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4080S_DEV,        ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4080S_16G_GAMING_WHITE,      0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 4080 SUPER O16G GAMING WHITE",    DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4080S_DEV,        ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4080S_016G_GAMING_WHITE,     0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 4080 SUPER O16G GAMING",                DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4080S_DEV,        ASUS_SUB_VEN,   ASUS_TUF_RTX_4080S_O16G_OC_GAMING,              0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 4090 O24G OC",                          DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_4090_O24G_OC_GAMING,               0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 4090 O24G OG OC",                       DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_4090_O24G_OG_OC_GAMING,            0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 4090 O24G",                             DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_4090_O24G_GAMING,                  0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 4090 O24G",                             DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_4090_O24G_GAMING_2,                0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 4090 O24G",                             DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_TUF_RTX_4090_O24G_GAMING_3,                0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX LC RTX 4090 O24G GAMING",             DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_LC_RTX_4090_O24G_GAMING,         0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 4090 24G GAMING",                 DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4090_24G_GAMING,             0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 4090 O24G GAMING",                DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4090_O24G_GAMING,            0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 4090 O24G GAMING",                DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4090_O24G_GAMING_213S,       0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 4090 24G GAMING",                 DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4090_24G_GAMING_88F0,        0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 4090 024G EVA-02",                DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4090_024G_EVA_02,            0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 4090 O24G GAMING",                DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4090_O24G_GAMING_8932,       0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 4090 24G GAMING WHITE",           DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4090_24G_GAMING_WHITE,       0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 4090 O24G GAMING WHITE",          DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4090_O24G_GAMING_WHITE,      0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 4090 O24G GAMING WHITE",          DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX_4090_O24G_GAMING_WHITE_2,    0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG MATRIX PLATINUM RTX 4090 24G",              DetectENESMBusGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         ASUS_SUB_VEN,   ASUS_ROG_MATRIX_PLATINUM_RTX_4090_24G,          0x67);

/*-----------------------------------------*\
|  AMD GPUs                                 |
\*-----------------------------------------*/

REGISTER_I2C_PCI_DETECTOR("ASUS TUF RX 6700 XT O12G GAMING",                    DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI22_DEV,             ASUS_SUB_VEN,   ASUS_TUF_RX_6700XT_O12G_GAMING,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RX 6700 XT O12G GAMING",              DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI22_DEV,             ASUS_SUB_VEN,   ASUS_ROG_STRIX_RX_6700XT_O12G_GAMING,           0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RX 6750 XT O12G GAMING",              DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI22_DEV,             ASUS_SUB_VEN,   ASUS_ROG_STRIX_RX_6750XT_O12G_GAMING,           0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS RX 6800 TUF Gaming OC",                         DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI21_DEV1,            ASUS_SUB_VEN,   ASUS_RX6800_TUF_GAMING_OC,                      0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RX 6800 O16G Gaming",                 DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI21_DEV1,            ASUS_SUB_VEN,   ASUS_ROG_STRIX_RX_6800_O16G_GAMING,             0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RX 6800 XT O16G GAMING",                    DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI21_DEV1,            ASUS_SUB_VEN,   ASUS_TUF_RX_6800XT_O16G_GAMING,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX LC RX 6800 XT O16G GAMING",           DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI21_DEV1,            ASUS_SUB_VEN,   ASUS_ROG_STRIX_LC_RX6800XT_O16G_GAMING,         0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX LC RX 6900 XT O16G GAMING",           DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI21_DEV1,            ASUS_SUB_VEN,   ASUS_ROG_STRIX_LC_RX6900XT_O16G_GAMING,         0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX LC RX 6900 XT O16G GAMING TOP",       DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI21_DEV2,            ASUS_SUB_VEN,   ASUS_ROG_STRIX_LC_RX6900XT_O16G_GAMING_TOP,     0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RX 6900 XT O16G GAMING",                    DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI21_DEV1,            ASUS_SUB_VEN,   ASUS_TUF_RX_6900XT_O16G_GAMING,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RX 6900 XT T16G GAMING",                    DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI21_DEV2,            ASUS_SUB_VEN,   ASUS_TUF_RX_6900XT_T16G_GAMING,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RX 6950 XT O16G GAMING",                    DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI21_DEV3,            ASUS_SUB_VEN,   ASUS_TUF_RX_6950XT_016G_GAMING,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX LC RX 6950 XT O16G GAMING",           DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI21_DEV3,            ASUS_SUB_VEN,   ASUS_ROG_STRIX_LC_RX_6950XT_O16G_GAMING,        0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RX 7700 XT GAMING OC",                      DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI32_DEV,             ASUS_SUB_VEN,   ASUS_TUF_RX_7700XT_012G_GAMING,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RX 7800 XT GAMING OC",                      DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI32_DEV,             ASUS_SUB_VEN,   ASUS_TUF_RX_7800XT_O16G_GAMING,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RX 7800 XT GAMING WHITE OC",                DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI32_DEV,             ASUS_SUB_VEN,   ASUS_TUF_RX_7800XT_O16G_WHITE_GAMING,           0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RX 7900 XT 020G GAMING",                    DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI31_DEV,             ASUS_SUB_VEN,   ASUS_TUF_RX_7900XT_020G_GAMING,                 0x67);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RX 7900 XTX O24G GAMING",                   DetectENESMBusGPUControllers,   AMD_GPU_VEN,    AMD_NAVI31_DEV,             ASUS_SUB_VEN,   ASUS_TUF_RX_7900XTX_O24G_GAMING,                0x67);
