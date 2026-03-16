/*---------------------------------------------------------*\
| TForceXtreemControllerDetect.cpp                          |
|                                                           |
|   Detector for T-Force Xtreem RAM                         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <vector>
#include "Detector.h"
#include "TForceXtreemController.h"
#include "LogManager.h"
#include "RGBController_TForceXtreem.h"
#include "i2c_smbus.h"
#include "pci_ids.h"

#define DETECTOR_NAME   "TForce Xtreem Controller"

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
#define XTREEM_RAM_ADDRESS_COUNT  13

static const unsigned char xtreem_ram_addresses[] =
{
    0x70,
    0x71,
    0x72,
    0x73,
    0x74,
    0x75,
    0x76,
    0x78,
    0x39,
    0x3A,
    0x3B,
    0x3C,
    0x3D
};

/******************************************************************************************\
*                                                                                          *
*   XtreemRegisterWrite                                                                    *
*                                                                                          *
*       A standalone version of the TForceXtreemController::ENERegisterWrite function for  *
*       access to ENE devices without instancing the TForceXtreemController class.         *
*                                                                                          *
\******************************************************************************************/

static void XtreemRegisterWrite(i2c_smbus_interface* bus, ene_dev_id dev, ene_register reg, unsigned char val)
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
*       quick write to test for a response, and if so does a simple read at 0x90 to test   *
*       for incrementing values 10...1F which was observed at this location                *
*                                                                                          *
\******************************************************************************************/

bool TestForTForceXtreemController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    LOG_DEBUG("[%s] looking for devices at 0x%02X...", DETECTOR_NAME, address);

    int res = bus->i2c_smbus_read_byte(address);

    if(res < 0)
    {
        res = bus->i2c_smbus_read_byte_data(address, 0x00);
    }

    if(res >= 0)
    {
        pass = true;

        LOG_DEBUG("[%s] Detected an I2C device at address %02X, testing register range", DETECTOR_NAME, address);

        for(int i = 0x90; i < 0xA1; i++)
        {
            res = bus->i2c_smbus_read_byte_data(address, i);

            if(res != (i - 0x80))
            {
                LOG_VERBOSE("[%s] Detection failed testing register %02X.  Expected %02X, got %02X.", DETECTOR_NAME, i, (i - 0x80), res);

                pass = false;
                break;
            }
        }
    }

    return(pass);

}   /* TestForTForceXtreemController() */

/******************************************************************************************\
*                                                                                          *
*   TestForTForceDeltaController                                                           *
*                                                                                          *
*       Tests the given address to see if an ENE controller exists there using the          *
*       standard ENE test pattern (0xA0-0xAF incrementing 0x00-0x0F).  The Delta DDR5      *
*       does not respond to the Xtreem-specific test at 0x90-0xA0.                         *
*                                                                                          *
\******************************************************************************************/

static bool TestForTForceDeltaController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    LOG_DEBUG("[%s] looking for Delta DDR5 devices at 0x%02X...", DETECTOR_NAME, address);

    int res = bus->i2c_smbus_read_byte(address);

    if(res < 0)
    {
        res = bus->i2c_smbus_read_byte_data(address, 0x00);
    }

    if(res >= 0)
    {
        pass = true;

        LOG_DEBUG("[%s] Detected an I2C device at address %02X, testing ENE register range", DETECTOR_NAME, address);

        for(int i = 0xA0; i < 0xB0; i++)
        {
            res = bus->i2c_smbus_read_byte_data(address, i);

            if(res != (i - 0xA0))
            {
                LOG_VERBOSE("[%s] Delta detection failed testing register %02X.  Expected %02X, got %02X.", DETECTOR_NAME, i, (i - 0xA0), res);

                pass = false;
                break;
            }
        }
    }

    return(pass);

}   /* TestForTForceDeltaController() */

/******************************************************************************************\
*                                                                                          *
*   RemapENERamModules                                                                     *
*                                                                                          *
*       Remaps ENE RAM modules from the default 0x77 master address to individual         *
*       per-slot addresses from the xtreem_ram_addresses list.                            *
*                                                                                          *
\******************************************************************************************/

typedef bool (*ENETestFunction)(i2c_smbus_interface*, unsigned char);

static void RemapENERamModules(i2c_smbus_interface* bus, std::vector<SPDWrapper*>& slots, ENETestFunction test_func)
{
    LOG_DEBUG("[%s] Remapping ENE SMBus RAM modules on 0x77", DETECTOR_NAME);

    for(SPDWrapper* slot : slots)
    {
        int address_list_idx = slot->index() - 1;
        int res;

        /*-------------------------------------------------*\
        | Full test to avoid conflicts with other ENE DRAMs |
        \*-------------------------------------------------*/
        if(!test_func(bus, 0x77))
        {
            LOG_DEBUG("[%s] No device detected at 0x77, aborting remap", DETECTOR_NAME);

            break;
        }

        do
        {
            address_list_idx++;

            if(address_list_idx < XTREEM_RAM_ADDRESS_COUNT)
            {
                LOG_DEBUG("[%s] Testing address %02X to see if there is a device there", DETECTOR_NAME, xtreem_ram_addresses[address_list_idx]);

                res = bus->i2c_smbus_write_quick(xtreem_ram_addresses[address_list_idx], I2C_SMBUS_WRITE);
            }
            else
            {
                break;
            }
        } while(res >= 0);

        if(address_list_idx < XTREEM_RAM_ADDRESS_COUNT)
        {
            LOG_DEBUG("[%s] Remapping slot %d to address %02X", DETECTOR_NAME, slot, xtreem_ram_addresses[address_list_idx]);

            XtreemRegisterWrite(bus, 0x77, XTREEM_REG_SLOT_INDEX, slot->index());
            XtreemRegisterWrite(bus, 0x77, XTREEM_REG_I2C_ADDRESS, (xtreem_ram_addresses[address_list_idx] << 1));
        }
    }
}   /* RemapENERamModules() */

/******************************************************************************************\
*                                                                                          *
*   DetectTForceXtreemDRAMControllers                                                      *
*                                                                                          *
*           Detects T-Force Xtreem controllers on DDR4 DRAM devices                       *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where device is connected                 *
*           slots - SPD accessors to occupied slots                                        *
*                                                                                          *
\******************************************************************************************/

void DetectTForceXtreemControllers(i2c_smbus_interface* bus, std::vector<SPDWrapper*> &slots, const std::string &/*name*/)
{
    RemapENERamModules(bus, slots, TestForTForceXtreemController);

    // Add ENE controllers at their remapped addresses
    for(unsigned int address_list_idx = 0; address_list_idx < XTREEM_RAM_ADDRESS_COUNT; address_list_idx++)
    {
        if(TestForTForceXtreemController(bus, xtreem_ram_addresses[address_list_idx]))
        {
            TForceXtreemController*      controller     = new TForceXtreemController(bus, xtreem_ram_addresses[address_list_idx]);
            RGBController_TForceXtreem*  rgb_controller = new RGBController_TForceXtreem(controller);

            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
    }
}   /* DetectTForceXtreemControllers() */

/******************************************************************************************\
*                                                                                          *
*   DetectTForceDeltaControllers                                                           *
*                                                                                          *
*       Detects T-Force Delta RGB controllers on DDR5 DRAM devices.                        *
*       The Delta DDR5 uses the ENE 0xExxx register range (same as Xtreem) but responds    *
*       to the standard ENE test pattern (0xA0-0xAF) rather than the Xtreem-specific       *
*       pattern (0x90-0xA0).                                                               *
*                                                                                          *
*       NOTE: The generic "ENE SMBus DRAM" detector also detects these devices but uses     *
*       the wrong register range (0x8xxx), resulting in only partial LED control. Users     *
*       should disable "ENE SMBus DRAM" in OpenRGB settings to avoid duplicate devices.    *
*                                                                                          *
*       bus - pointer to i2c_smbus_interface where device is connected                     *
*       slots - SPD accessors to occupied slots                                            *
*                                                                                          *
\******************************************************************************************/

void DetectTForceDeltaDRAMControllers(std::vector<i2c_smbus_interface*> &busses)
{
    for(unsigned int bus = 0; bus < busses.size(); bus++)
    {
        int address_list_idx = -1;

        IF_DRAM_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
        {
            LOG_DEBUG("[T-Force Delta DDR5] Remapping ENE SMBus RAM modules on 0x77");

            for(unsigned int slot = 0; slot < 8; slot++)
            {
                if(!TestForTForceDeltaController(busses[bus], 0x77))
                {
                    LOG_DEBUG("[T-Force Delta DDR5] No device detected at 0x77, aborting remap");
                    break;
                }

                do
                {
                    address_list_idx++;

                    if(address_list_idx < XTREEM_RAM_ADDRESS_COUNT)
                    {
                        LOG_DEBUG("[T-Force Delta DDR5] Testing address %02X to see if there is a device there", xtreem_ram_addresses[address_list_idx]);

                        int res = busses[bus]->i2c_smbus_write_quick(xtreem_ram_addresses[address_list_idx], I2C_SMBUS_WRITE);

                        if(res < 0)
                        {
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                } while(true);

                if(address_list_idx < XTREEM_RAM_ADDRESS_COUNT)
                {
                    LOG_DEBUG("[T-Force Delta DDR5] Remapping slot %d to address %02X", slot, xtreem_ram_addresses[address_list_idx]);

                    XtreemRegisterWrite(busses[bus], 0x77, XTREEM_REG_SLOT_INDEX, slot);
                    XtreemRegisterWrite(busses[bus], 0x77, XTREEM_REG_I2C_ADDRESS, (xtreem_ram_addresses[address_list_idx] << 1));
                }
            }

            // Add controllers at their remapped addresses
            for(unsigned int addr_idx = 0; addr_idx < XTREEM_RAM_ADDRESS_COUNT; addr_idx++)
            {
                if(TestForTForceDeltaController(busses[bus], xtreem_ram_addresses[addr_idx]))
                {
                    TForceXtreemController*      controller     = new TForceXtreemController(busses[bus], xtreem_ram_addresses[addr_idx], DELTA_LED_COUNT, false);
                    RGBController_TForceXtreem*  rgb_controller = new RGBController_TForceXtreem(controller, "T-Force Delta RGB");

                    ResourceManager::get()->RegisterRGBController(rgb_controller);
                }

                std::this_thread::sleep_for(1ms);
            }
        }
    }
}   /* DetectTForceDeltaDRAMControllers() */

REGISTER_I2C_DIMM_DETECTOR("T-Force Xtreem DDR4 DRAM", DetectTForceXtreemControllers, JEDEC_TEAMGROUP, SPD_DDR4_SDRAM);
REGISTER_I2C_DETECTOR("T-Force Delta DDR5 DRAM",       DetectTForceDeltaDRAMControllers);
