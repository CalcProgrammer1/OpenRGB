#include "Detector.h"
#include "HyperXDRAMController.h"
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

bool TestForHyperXDRAMController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    int res = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);

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
    HyperXDRAMController* new_hyperx;
    RGBController_HyperXDRAM* new_controller;

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        unsigned char slots_valid = 0x00;
        bool fury_detected = false;
        bool pred_detected = false;

        IF_DRAM_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
        {
            // Check for HyperX controller at 0x27
            if (TestForHyperXDRAMController(busses[bus], 0x27))
            {
                busses[bus]->i2c_smbus_write_byte_data(0x37, 0x00, 0xFF);

                std::this_thread::sleep_for(1ms);
                
                for(int slot_addr = 0x50; slot_addr <= 0x57; slot_addr++)
                {
                    // Test for HyperX SPD at slot_addr
                    // This test was copied from NGENUITY software
                    // Tests SPD addresses in order: 0x40, 0x41
                    if((busses[bus]->i2c_smbus_read_byte_data(slot_addr, 0x40) == 0x01)
                    &&(busses[bus]->i2c_smbus_read_byte_data(slot_addr, 0x41) == 0x98))
                    {
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

                    std::this_thread::sleep_for(1ms);
                }

                if(slots_valid != 0)
                {
                    new_hyperx = new HyperXDRAMController(busses[bus], 0x27, slots_valid);
                    new_controller = new RGBController_HyperXDRAM(new_hyperx);

                    if(fury_detected && !pred_detected)
                    {
                        new_controller->name = "HyperX Fury RGB";
                    }
                    else if(!fury_detected && pred_detected)
                    {
                        new_controller->name = "HyperX Predator RGB";
                    }

                    ResourceManager::get()->RegisterRGBController(new_controller);
                }
            }
        }
    }

}   /* DetectHyperXDRAMControllers() */

REGISTER_I2C_DETECTOR("HyperX DRAM", DetectHyperXDRAMControllers);
