#include "CrucialController.h"
#include "RGBController.h"
#include "RGBController_Crucial.h"
#include "i2c_smbus.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************************\
*                                                                                          *
*   TestForCrucialController                                                               *
*                                                                                          *
*       Tests the given address to see if an Crucial controller exists there.  First does a*
*       quick write to test for a response, and if so does a simple read at 0xA0 to test   *
*       for incrementing values 0...F which was observed at this location during data dump *
*                                                                                          *
\******************************************************************************************/

bool TestForCrucialController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    int res = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);

    if (res >= 0)
    {
        pass = true;

        for (int i = 0xA0; i < 0xB0; i++)
        {
            res = bus->i2c_smbus_read_byte_data(address, i);

            if (res != (i - 0xA0))
            {
                pass = false;
            }
        }
    }

    return(pass);

}   /* TestForCrucialController() */

/******************************************************************************************\
*                                                                                          *
*   DetectCrucialControllers                                                               *
*                                                                                          *
*       Detect Crucial controllers on the enumerated I2C busses.                           *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where Aura device is connected            *
*           dev - I2C address of Aura device                                               *
*                                                                                          *
\******************************************************************************************/

void DetectCrucialControllers(std::vector<i2c_smbus_interface*> &busses, std::vector<RGBController*> &rgb_controllers)
{
    CrucialController* new_crucial;
    RGBController_Crucial* new_controller;

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        if (TestForCrucialController(busses[bus], 0x23))
        {
            new_crucial = new CrucialController(busses[bus], 0x23);
            new_controller = new RGBController_Crucial(new_crucial);
            rgb_controllers.push_back(new_controller);
        }

    }

}   /* DetectCrucialControllers() */
