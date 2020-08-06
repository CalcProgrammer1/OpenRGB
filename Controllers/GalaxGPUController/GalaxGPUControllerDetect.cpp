/*-----------------------------------------*\
|  GalaxGPUControllerDetect.cpp             |
|                                           |
|  Driver for Galax / KFA2 RGB on GPUs      |
|                                           |
|  Niels Westphal (crashniels)  12.07.2020  |
\*-----------------------------------------*/

#include "Detector.h"
#include "GalaxGPUController.h"
#include "RGBController.h"
#include "RGBController_GalaxGPU.h"
#include "i2c_smbus.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std::chrono_literals;


/******************************************************************************************\
*                                                                                          *
*   TestForGalaxGPUController                                                              *
*                                                                                          *
*       Tests the given address to see if a Galax GPU controller exists there.             *
*                                                                                          *
\******************************************************************************************/

bool TestForGalaxGPUController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    unsigned char res  = bus->i2c_smbus_read_byte_data(address, 0x00);
    unsigned char res2 = bus->i2c_smbus_read_byte_data(address, 0x01);
    if(res == 0x27 && res2 == 0x10)
    {
        pass = true;
    }

    return(pass);

}   /* TestForGalaxGPUController() */


/******************************************************************************************\
*                                                                                          *
*   DetectGalaxGPUControllers                                                              *
*                                                                                          *
*       Detect Galax GPU controllers on the enumerated I2C busses.                         *
*                                                                                          *
\******************************************************************************************/

void DetectGalaxGPUControllers(std::vector<i2c_smbus_interface*> &busses, std::vector<RGBController*> &rgb_controllers)
{
    GalaxGPUController* new_GalaxGPU;
    RGBController_GalaxGPU* new_controller;

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        // Check for GALAX controller at 0x23
        if (TestForGalaxGPUController(busses[bus], 0x23))
        {
            new_GalaxGPU = new GalaxGPUController(busses[bus], 0x23);
            new_controller = new RGBController_GalaxGPU(new_GalaxGPU);
            rgb_controllers.push_back(new_controller);
        }
    }

} /* DetectGalaxGPUControllers() */

REGISTER_I2C_DETECTOR("Galax GPU", DetectGalaxGPUControllers);
