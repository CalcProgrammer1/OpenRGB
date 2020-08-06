#include "Detector.h"
#include "RGBFusionController.h"
#include "RGBController.h"
#include "RGBController_RGBFusion.h"
#include "i2c_smbus.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************************\
*                                                                                          *
*   TestForRGBFusionController                                                             *
*                                                                                          *
*       Tests the given address to see if an RGB Fusion controller exists there.  First    *
*       does a quick write to test for a response                                          *
*                                                                                          *
\******************************************************************************************/

bool TestForRGBFusionController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    int res = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);

    if (res >= 0)
    {
        pass = true;

        res = bus->i2c_smbus_read_byte_data(address, 0xF2);

        if (res != 0xC4)
        {
            pass = false;
        }
    }

    return(pass);

}   /* TestForRGBFusionController() */

/******************************************************************************************\
*                                                                                          *
*   DetectRGBFusionControllers                                                             *
*                                                                                          *
*       Detect RGB Fusion controllers on the enumerated I2C busses at address 0x28.        *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where RGB Fusion device is connected      *
*           dev - I2C address of RGB Fusion device                                         *
*                                                                                          *
\******************************************************************************************/

void DetectRGBFusionControllers(std::vector<i2c_smbus_interface*>& busses, std::vector<RGBController*>& rgb_controllers)
{
    RGBFusionController* new_rgb_fusion;
    RGBController_RGBFusion* new_controller;

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        // Check for RGB Fusion controller at 0x28
        if (TestForRGBFusionController(busses[bus], 0x28))
        {
            new_rgb_fusion = new RGBFusionController(busses[bus], 0x28);
            new_controller = new RGBController_RGBFusion(new_rgb_fusion);
            rgb_controllers.push_back(new_controller);
        }
    }

}   /* DetectRGBFusionControllers() */

REGISTER_I2C_DETECTOR("Gigabyte RGB Fusion", DetectRGBFusionControllers);
