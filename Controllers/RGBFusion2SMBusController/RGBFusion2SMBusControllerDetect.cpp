#include "Detector.h"
#include "RGBFusion2SMBusController.h"
#include "RGBController.h"
#include "RGBController_RGBFusion2SMBus.h"
#include "i2c_smbus.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string>

/******************************************************************************************\
*                                                                                          *
*   TestForRGBFusion2SMBusController                                                       *
*                                                                                          *
*       Tests the given address to see if an RGB 2 Fusion controller exists there.  First  *
*       does a quick write to test for a response                                          *
*                                                                                          *
\******************************************************************************************/

bool TestForRGBFusion2SMBusController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    int res = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);
    if (res >= 0)
    {
        pass = true;

        // res = bus->i2c_smbus_read_byte_data(address, 0xF2);

        // if (res != 0xC4)
        // {
        //     pass = false;
        // }
    }

    return(pass);

}   /* TestForRGBFusion2SMBusController() */

/******************************************************************************************\
*                                                                                          *
*   DetectRGBFusion2SMBusControllers                                                       *
*                                                                                          *
*       Detect RGB Fusion 2 controllers on the enumerated I2C busses at address 0x68.      *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where RGB Fusion device is connected      *
*           dev - I2C address of RGB Fusion device                                         *
*                                                                                          *
\******************************************************************************************/

void DetectRGBFusion2SMBusControllers(std::vector<i2c_smbus_interface*>& busses, std::vector<RGBController*>& rgb_controllers)
{
    RGBFusion2SMBusController* new_rgb_fusion;
    RGBController_RGBFusion2SMBus* new_controller;

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        // TODO - Is this necessary? Or an artifact of my own system?
        // Skip dmcd devices
        std::string device_name = std::string(busses[bus]->device_name);
        if (device_name.find("dmdc") == std::string::npos)
        {
            // Check for RGB Fusion 2 controller at 0x68
            if (TestForRGBFusion2SMBusController(busses[bus], 0x68))
            {
                new_rgb_fusion = new RGBFusion2SMBusController(busses[bus], 0x68);
                new_controller = new RGBController_RGBFusion2SMBus(new_rgb_fusion);
                rgb_controllers.push_back(new_controller);
            }
        }
    }

}   /* DetectRGBFusion2SMBusControllers() */

// This detector is disabled as proper detection is not implemented
//REGISTER_DETECTOR("Gigabyte RGB Fusion 2 SMBus", DetectRGBFusion2SMBusControllers);
