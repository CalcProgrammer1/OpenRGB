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
        IF_DRAM_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
        {
            std::this_thread::sleep_for(1ms);

            if((busses[bus]->i2c_smbus_read_byte_data(0x20, 0x1D) == 0x0F)
             &&(busses[bus]->i2c_smbus_read_byte_data(0x20, 0x1E) == 0x0C)
             &&(busses[bus]->i2c_smbus_read_byte_data(0x20, 0x39) == 0x0F)
             &&(busses[bus]->i2c_smbus_read_byte_data(0x20, 0x3A) == 0x0C))
            {
                PatriotViperSteelController*     controller     = new PatriotViperSteelController(busses[bus], 0x77);
                RGBController_PatriotViperSteel* rgb_controller = new RGBController_PatriotViperSteel(controller);

                ResourceManager::get()->RegisterRGBController(rgb_controller);
            }
        }
    }

} /* DetectPatriotViperSteelControllers() */

REGISTER_I2C_DETECTOR(PATRIOT_CONTROLLER_NAME, DetectPatriotViperSteelControllers);
