/*---------------------------------------------------------*\
| TForceXtreemControllerDetect.cpp                          |
|                                                           |
|   Detector for T-Force Xtreem RAM                         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <vector>
#include "Detector.h"
#include "TForceXtreemController.h"
#include "LogManager.h"
#include "RGBController_TForceXtreem.h"
#include "i2c_smbus.h"

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
*   DetectTForceXtreemDRAMControllers                                                      *
*                                                                                          *
*           Detects T-Force Xtreem controllers on DRAM devices                             *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where device is connected                 *
*           slots - SPD accessors to occupied slots                                        *
*                                                                                          *
\******************************************************************************************/

void DetectTForceXtreemControllers(i2c_smbus_interface* bus, std::vector<SPDWrapper*> &slots, const std::string &/*name*/)
{

    LOG_DEBUG("[%s] Remapping ENE SMBus RAM modules on 0x77", DETECTOR_NAME);

    for(SPDWrapper *slot : slots)
    {
        int address_list_idx = slot->index() - 1;
        int res;

        /*-------------------------------------------------*\
        | Full test to avoid conflicts with other ENE DRAMs |
        \*-------------------------------------------------*/
        if(!TestForTForceXtreemController(bus, 0x77))
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

REGISTER_I2C_DIMM_DETECTOR("T-Force Xtreem DDR4 DRAM", DetectTForceXtreemControllers, JEDEC_TEAMGROUP, SPD_DDR4_SDRAM);
