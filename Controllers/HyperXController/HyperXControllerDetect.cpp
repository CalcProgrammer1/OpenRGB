#include "HyperXController.h"
#include "RGBController.h"
#include "RGBController_HyperX.h"
#include "i2c_smbus.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************************\
*                                                                                          *
*   TestForHyperXController                                                               *
*                                                                                          *
*       Tests the given address to see if a HyperX controller exists there.               *
*                                                                                          *
\******************************************************************************************/

bool TestForHyperXController(i2c_smbus_interface* bus, unsigned char address)
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

}   /* TestForHyperXController() */


/******************************************************************************************\
*                                                                                          *
*   DetectHyperXControllers                                                                *
*                                                                                          *
*       Detect HyperX controllers on the enumerated I2C busses.                            *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where Aura device is connected            *
*           dev - I2C address of Aura device                                               *
*                                                                                          *
\******************************************************************************************/

void DetectHyperXControllers(std::vector<i2c_smbus_interface*> &busses, std::vector<RGBController*> &rgb_controllers)
{
    HyperXController* new_hyperx;
    RGBController_HyperX* new_controller;

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        // Check for HyperX controller at 0x27
        if (TestForHyperXController(busses[bus], 0x27))
        {
            new_hyperx = new HyperXController(busses[bus], 0x27);
            new_controller = new RGBController_HyperX(new_hyperx);
            rgb_controllers.push_back(new_controller);
        }
    }

}   /* DetectHyperXControllers() */