#include "Detector.h"
#include "SapphireGPUController.h"
#include "RGBController.h"
#include "RGBController_SapphireGPU.h"
#include "i2c_smbus.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************************\
*                                                                                          *
*   TestForSapphireGPUController                                                           *
*                                                                                          *
*       Tests the given address to see if a Sapphire GPU controller exists there.  First   *
*       does a quick write to test for a response                                          *
*                                                                                          *
\******************************************************************************************/

bool TestForSapphireGPUController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;
    int res;

    if(bus->i2c_smbus_read_byte_data(address, 0x06) == 0xCC)
    {
        pass = true;
    }

    return(false); //TODO - better detection, leaving disabled for now

}   /* TestForSapphireGPUController() */

/******************************************************************************************\
*                                                                                          *
*   DetectSapphireGPUControllers                                                           *
*                                                                                          *
*       Detect Sapphire GPU controllers on the enumerated I2C busses at address 0x55.      *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where Sapphire GPU device is connected    *
*           dev - I2C address of Sapphire GPU device                                       *
*                                                                                          *
\******************************************************************************************/

void DetectSapphireGPUControllers(std::vector<i2c_smbus_interface*>& busses, std::vector<RGBController*>& rgb_controllers)
{
    SapphireGPUController* new_sapphire;
    RGBController_SapphireGPU* new_controller;

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        // Check for Sapphire GPU controller at 0x55
        if (TestForSapphireGPUController(busses[bus], 0x55))
        {
            new_sapphire   = new SapphireGPUController(busses[bus], 0x55);
            new_controller = new RGBController_SapphireGPU(new_sapphire);
            rgb_controllers.push_back(new_controller);
        }
    }

}   /* DetectSapphireGPUControllers() */

// This detector is disabled as it does not properly detect
//REGISTER_I2C_DETECTOR("Sapphire GPU", DetectSapphireGPUControllers);
