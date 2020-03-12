#include "HyperXDRAMController.h"
#include "RGBController.h"
#include "RGBController_HyperXDRAM.h"
#include "i2c_smbus.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>

static void Sleep(unsigned int milliseconds)
{
    usleep(1000 * milliseconds);
}
#endif

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

void DetectHyperXDRAMControllers(std::vector<i2c_smbus_interface*> &busses, std::vector<RGBController*> &rgb_controllers)
{
    HyperXDRAMController* new_hyperx;
    RGBController_HyperXDRAM* new_controller;

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        unsigned char slots_valid = 0x00;

        // Check for HyperX controller at 0x27
        if (TestForHyperXDRAMController(busses[bus], 0x27))
        {
            busses[bus]->i2c_smbus_write_byte_data(0x37, 0x00, 0xFF);

            Sleep(1);
            
            for(int slot_addr = 0x50; slot_addr <= 0x57; slot_addr++)
            {
                // Test for HyperX SPD at slot_addr
                // This test was copied from NGENUITY software
                // Tests SPD addresses in order: 0x40, 0x41
                if((busses[bus]->i2c_smbus_read_byte_data(slot_addr, 0x40) == 0x01)
                 &&(busses[bus]->i2c_smbus_read_byte_data(slot_addr, 0x41) == 0x98))
                {
                    slots_valid |= (1 << (slot_addr - 0x50));
                }

                Sleep(1);
            }

            if(slots_valid != 0)
            {
                new_hyperx = new HyperXDRAMController(busses[bus], 0x27, slots_valid);
                new_controller = new RGBController_HyperXDRAM(new_hyperx);
                rgb_controllers.push_back(new_controller);
            }
        }
    }

}   /* DetectHyperXDRAMControllers() */
