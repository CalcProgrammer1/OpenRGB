#include "Detector.h"
#include "EVGAGPUController.h"
#include "RGBController.h"
#include "RGBController_EVGAGPU.h"
#include "i2c_smbus.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************************\
*                                                                                          *
*   TestForEVGAGPUController                                                               *
*                                                                                          *
*       Tests the given address to see if a EVGA GPU controller exists there.  First       *
*       does a quick write to test for a response                                          *
*                                                                                          *
\******************************************************************************************/

bool TestForEVGAGPUController(i2c_smbus_interface* bus, unsigned char address)
{
    return(false);
}   /* TestForEVGAGPUController() */

/******************************************************************************************\
*                                                                                          *
*   DetectEVGAGPUControllers                                                               *
*                                                                                          *
*       Detect EVGA GPU controllers on the enumerated I2C busses at address 0x49.          *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where EVGA GPU device is connected        *
*           dev - I2C address of EVGA GPU device                                           *
*                                                                                          *
\******************************************************************************************/

void DetectEVGAGPUControllers(std::vector<i2c_smbus_interface*>& busses, std::vector<RGBController*>& rgb_controllers)
{
    EVGAGPUController* new_evga;
    RGBController_EVGAGPU* new_controller;

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        // Check for EVGA GPU controller at 0x49
        if (TestForEVGAGPUController(busses[bus], 0x49))
        {
            new_evga       = new EVGAGPUController(busses[bus], 0x49);
            new_controller = new RGBController_EVGAGPU(new_evga);
            rgb_controllers.push_back(new_controller);
        }
    }

}   /* DetectEVGAGPUControllers() */

// This detector is disabled as it does not properly detect
//REGISTER_I2C_DETECTOR("EVGA GPU", DetectEVGAGPUControllers);
