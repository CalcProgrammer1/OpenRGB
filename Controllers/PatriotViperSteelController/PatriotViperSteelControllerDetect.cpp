#include "Detector.h"
#include "PatriotViperSteelController.h"
#include "LogManager.h"
#include "RGBController.h"
#include "RGBController_PatriotViperSteel.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std::chrono_literals;
#define PATRIOT_CONTROLLER_NAME "Patriot Viper Steel"

/******************************************************************************************\
*                                                                                          *
*   TestForPatriotViperSteelController                                                     *
*                                                                                          *
*       Tests the given address to see if a Patriot Viper Steel controller exists there.   *
*                                                                                          *
\******************************************************************************************/

bool TestForPatriotViperSteelController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    int res = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);

    LOG_DEBUG("[%s] Writing at address %02X, res=%02X", PATRIOT_CONTROLLER_NAME, address, res);

    if (res >= 0)
    {
        pass = true;
    }

    return(pass);

}   /* TestForPatriotViperSteelController() */


/******************************************************************************************\
*                                                                                          *
*   DetectPatriotViperSteelControllers                                                     *
*                                                                                          *
*       Detect Patriot Viper Steel RGB controllers on the enumerated I2C busses.           *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where Aura device is connected            *
*           dev - I2C address of Aura device                                               *
*                                                                                          *
\******************************************************************************************/

void DetectPatriotViperSteelControllers(std::vector<i2c_smbus_interface *> &busses)
{
    for(unsigned int bus = 0; bus < busses.size(); bus++)
    {
        unsigned char slots_valid = 0x00;

        IF_DRAM_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
        {
            // Check for Patriot Viper Steel controller at 0x77
            LOG_DEBUG("[%s] Testing bus %d at address 0x77", PATRIOT_CONTROLLER_NAME, bus);

            if(TestForPatriotViperSteelController(busses[bus], 0x77))
            {
                for(int slot_addr = 0x50; slot_addr <= 0x57; slot_addr++)
                {
                    // Test for Patriot Viper Steel RGB SPD at slot_addr
                    // This test was copied from PatriotViperControllerDetect
                    // Tests SPD addresses in order: 0x00, 0x40, 0x41, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68

                    busses[bus]->i2c_smbus_write_byte_data(0x36, 0x00, 0xFF);

                    std::this_thread::sleep_for(1ms);

                    int res = busses[bus]->i2c_smbus_read_byte_data(slot_addr, 0x00);

                    LOG_DEBUG("[%s] Trying to read 0x%02X RAM module data at 0x00 expect: 0x23 res: %02X", PATRIOT_CONTROLLER_NAME, slot_addr, res);
                    if(res == 0x23)
                    {
                        busses[bus]->i2c_smbus_write_byte_data(0x37, 0x00, 0xFF);

                        std::this_thread::sleep_for(1ms);

                        if((busses[bus]->i2c_smbus_read_byte_data(slot_addr, 0x40) == 0xFF)
                        &&(busses[bus]->i2c_smbus_read_byte_data(slot_addr, 0x41) == 0xFF)
                        &&(busses[bus]->i2c_smbus_read_byte_data(slot_addr, 0x61) == 0x50)
                        &&(busses[bus]->i2c_smbus_read_byte_data(slot_addr, 0x62) == 0x44)
                        &&(busses[bus]->i2c_smbus_read_byte_data(slot_addr, 0x63) == 0x41)
                        &&(busses[bus]->i2c_smbus_read_byte_data(slot_addr, 0x64) == 0x31)
                        &&(busses[bus]->i2c_smbus_read_byte_data(slot_addr, 0x65) == 0x00)
                        &&(busses[bus]->i2c_smbus_read_byte_data(slot_addr, 0x66) == 0x00)
                        &&(busses[bus]->i2c_smbus_read_byte_data(slot_addr, 0x67) == 0x00)
                        &&(busses[bus]->i2c_smbus_read_byte_data(slot_addr, 0x68) == 0x00))
                        {
                            LOG_DEBUG("[%s] The 0x%02X RAM module detected", PATRIOT_CONTROLLER_NAME, slot_addr);
                            slots_valid |= (1 << (slot_addr - 0x50));
                        }
                    }

                    std::this_thread::sleep_for(1ms);
                }

                if(slots_valid != 0)
                {
                    PatriotViperSteelController*     controller     = new PatriotViperSteelController(busses[bus], 0x77);
                    RGBController_PatriotViperSteel* rgb_controller = new RGBController_PatriotViperSteel(controller);
                    ResourceManager::get()->RegisterRGBController(rgb_controller);
                }
            }
        }
    }

} /* DetectPatriotViperSteelControllers() */

REGISTER_I2C_DETECTOR(PATRIOT_CONTROLLER_NAME, DetectPatriotViperSteelControllers);
