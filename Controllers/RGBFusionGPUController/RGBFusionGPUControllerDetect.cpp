#include "Detector.h"
#include "RGBFusionGPUController.h"
#include "RGBController.h"
#include "RGBController_RGBFusionGPU.h"
#include "i2c_smbus.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************************\
*                                                                                          *
*   TestForRGBFusionGPUController                                                          *
*                                                                                          *
*       Tests the given address to see if an RGB Fusion controller exists there.  First    *
*       does a quick write to test for a response                                          *
*                                                                                          *
\******************************************************************************************/

bool TestForRGBFusionGPUController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;
    int res;

    //Write out 0xAB 0x00 0x00 0x00 sequence
    res = bus->i2c_smbus_write_byte(address, 0xAB);

    if (res >= 0)
    {
        bus->i2c_smbus_write_byte(address, 0x00);
        bus->i2c_smbus_write_byte(address, 0x00);
        bus->i2c_smbus_write_byte(address, 0x00);

        pass = true;

        res = bus->i2c_smbus_read_byte(address);

        if (res != 0xAB)
        {
            pass = false;
        }

        res = bus->i2c_smbus_read_byte(address);

        if(res != 0x14)
        {
            pass = false;
        }

        bus->i2c_smbus_read_byte(address);
        bus->i2c_smbus_read_byte(address);
    }

    return(pass);

}   /* TestForRGBFusionGPUController() */

/******************************************************************************************\
*                                                                                          *
*   DetectRGBFusionGPUControllers                                                          *
*                                                                                          *
*       Detect RGB Fusion controllers on the enumerated I2C busses at address 0x47.        *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where RGB Fusion device is connected      *
*           dev - I2C address of RGB Fusion device                                         *
*                                                                                          *
\******************************************************************************************/

void DetectRGBFusionGPUControllers(std::vector<i2c_smbus_interface*>& busses, std::vector<RGBController*>& rgb_controllers)
{
    RGBFusionGPUController* new_rgb_fusion;
    RGBController_RGBFusionGPU* new_controller;

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        // Check for RGB Fusion controller at 0x47
        if (TestForRGBFusionGPUController(busses[bus], 0x47))
        {
            new_rgb_fusion = new RGBFusionGPUController(busses[bus], 0x47);
            new_controller = new RGBController_RGBFusionGPU(new_rgb_fusion);
            rgb_controllers.push_back(new_controller);
        }
    }

}   /* DetectRGBFusionGPUControllers() */

REGISTER_I2C_DETECTOR("Gigabyte RGB Fusion GPU", DetectRGBFusionGPUControllers);
