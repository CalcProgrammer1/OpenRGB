#include "Detector.h"
#include "HyperXDRAMController.h"
#include "LogManager.h"
#include "RGBController.h"
#include "RGBController_HyperXDRAM.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std::chrono_literals;

/******************************************************************************************\
*                                                                                          *
*   TestForHyperXDRAMController                                                            *
*                                                                                          *
*       Tests the given address to see if a HyperX controller exists there.                *
*                                                                                          *
\******************************************************************************************/
#define HYPERX_CONTROLLER_NAME "HyperX DRAM"

bool TestForHyperXDRAMController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    int res = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);

    LOG_DEBUG("[%s] Writing at address %02X, res=%02X", HYPERX_CONTROLLER_NAME, address, res);

    if (res >= 0)
    {
        pass = true;

        for (int i = 0xA0; i < 0xB0; i++)
        {
            res = bus->i2c_smbus_read_byte_data(address, i);

            if (res != i)
            {
                pass = false;
            }
        }
    }

    return(pass);

}   /* TestForHyperXDRAMController() */


/******************************************************************************************\
*                                                                                          *
*   DetectHyperXDRAMControllers                                                            *
*                                                                                          *
*       Detect HyperX DRAM controllers on the enumerated I2C busses.                       *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where Aura device is connected            *
*           dev - I2C address of Aura device                                               *
*                                                                                          *
\******************************************************************************************/

void DetectHyperXDRAMControllers(std::vector<i2c_smbus_interface*> &busses)
{
    for(unsigned int bus = 0; bus < busses.size(); bus++)
    {
        unsigned char slots_valid   = 0x00;
        bool          fury_detected = false;
        bool          pred_detected = false;

        LOG_DEBUG("[%s] Checking VID/PID on bus %d...", HYPERX_CONTROLLER_NAME, bus);

        IF_DRAM_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
        {
            // Check for HyperX controller at 0x27
            LOG_DEBUG("[%s] Testing bus %d at address 0x27", HYPERX_CONTROLLER_NAME, bus);

            if(TestForHyperXDRAMController(busses[bus], 0x27))
            {
                busses[bus]->i2c_smbus_write_byte_data(0x37, 0x00, 0xFF);

                std::this_thread::sleep_for(1ms);
                
                for(int slot_addr = 0x50; slot_addr <= 0x57; slot_addr++)
                {
                    // Test for HyperX SPD at slot_addr
                    // This test was copied from NGENUITY software
                    // Tests SPD addresses in order: 0x40, 0x41
                    int read_0x40 = busses[bus]->i2c_smbus_read_byte_data(slot_addr, 0x40);
                    int read_0x41 = busses[bus]->i2c_smbus_read_byte_data(slot_addr, 0x41);

                    LOG_DEBUG("[%s] SPD check: 0x40 => %02X, 0x41 => %02X, ",
                              HYPERX_CONTROLLER_NAME, read_0x40, read_0x41);

                    if((read_0x40 == 0x01) && (read_0x41 == 0x98))
                    {
                        LOG_DEBUG("[%s] SPD check success", HYPERX_CONTROLLER_NAME);

                        slots_valid |= (1 << (slot_addr - 0x50));

                        if(busses[bus]->i2c_smbus_read_byte_data(slot_addr, 0x67) == 0x01)
                        {
                            fury_detected = true;
                        }
                        else
                        {
                            pred_detected = true;
                        }
                    }
                    else
                    {
                        LOG_DEBUG("[%s] SPD check failed", HYPERX_CONTROLLER_NAME);
                    }

                    std::this_thread::sleep_for(1ms);
                }

                LOG_DEBUG("[%s] slots_valid=%d fury_detected=%d pred_detected=%d",
                          HYPERX_CONTROLLER_NAME, slots_valid, fury_detected, pred_detected);

                if(slots_valid != 0)
                {
                    HyperXDRAMController*     controller     = new HyperXDRAMController(busses[bus], 0x27, slots_valid);
                    RGBController_HyperXDRAM* rgb_controller = new RGBController_HyperXDRAM(controller);

                    if(fury_detected && !pred_detected)
                    {
                        rgb_controller->name = "HyperX Fury RGB";
                    }
                    else if(!fury_detected && pred_detected)
                    {
                        rgb_controller->name = "HyperX Predator RGB";
                    }

                    ResourceManager::get()->RegisterRGBController(rgb_controller);
                }
            }
        }
        else
        {
            LOG_DEBUG("[%s] IF_DRAM_SMBUS was false for %04X %04X", HYPERX_CONTROLLER_NAME, busses[bus]->pci_vendor, busses[bus]->pci_device);
        }
    }

}   /* DetectHyperXDRAMControllers() */

REGISTER_I2C_DETECTOR("HyperX DRAM", DetectHyperXDRAMControllers);
