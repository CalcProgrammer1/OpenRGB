#include "CorsairController.h"
#include "RGBController.h"
#include "RGBController_Corsair.h"
#include "i2c_smbus.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************************\
*                                                                                          *
*   TestForCorsairController                                                               *
*                                                                                          *
*       Tests the given address to see if a Corsair controller exists there.               *
*                                                                                          *
\******************************************************************************************/

bool TestForCorsairController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    int res = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);

    if (res >= 0)
    {
        pass = true;

        for (int i = 0xA0; i < 0xB0; i++)
        {
            res = bus->i2c_smbus_read_byte_data(address, i);

            if (res != 0xBA)
            {
                pass = false;
            }
        }
    }

    return(pass);

}   /* TestForCorsairController() */

/******************************************************************************************\
*                                                                                          *
*   DetectCorsairControllers                                                               *
*                                                                                          *
*       Detect Corsair controllers on the enumerated I2C busses.                           *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where Aura device is connected            *
*           dev - I2C address of Aura device                                               *
*                                                                                          *
\******************************************************************************************/

void DetectCorsairControllers(std::vector<i2c_smbus_interface*> &busses, std::vector<RGBController*> &rgb_controllers)
{
    CorsairController* new_corsair;
    RGBController_Corsair* new_controller;

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        // Check for Corsair controller at 0x58
        if (TestForCorsairController(busses[bus], 0x58))
        {
            new_corsair = new CorsairController(busses[bus], 0x58);
            new_controller = new RGBController_Corsair(new_corsair);
            rgb_controllers.push_back(new_controller);
        }

        // Check for Corsair controller at 0x59
        if (TestForCorsairController(busses[bus], 0x59))
        {
            new_corsair = new CorsairController(busses[bus], 0x59);
            new_controller = new RGBController_Corsair(new_corsair);
            rgb_controllers.push_back(new_controller);
        }

        // Check for Corsair controller at 0x5A
        if (TestForCorsairController(busses[bus], 0x5A))
        {
            new_corsair = new CorsairController(busses[bus], 0x5A);
            new_controller = new RGBController_Corsair(new_corsair);
            rgb_controllers.push_back(new_controller);
        }

        // Check for Corsair controller at 0x5B
        if (TestForCorsairController(busses[bus], 0x5B))
        {
            new_corsair = new CorsairController(busses[bus], 0x5B);
            new_controller = new RGBController_Corsair(new_corsair);
            rgb_controllers.push_back(new_controller);
        }

        // Check for Corsair controller at 0x5C
        if (TestForCorsairController(busses[bus], 0x5C))
        {
            new_corsair = new CorsairController(busses[bus], 0x5C);
            new_controller = new RGBController_Corsair(new_corsair);
            rgb_controllers.push_back(new_controller);
        }

        // Check for Corsair controller at 0x5D
        if (TestForCorsairController(busses[bus], 0x5D))
        {
            new_corsair = new CorsairController(busses[bus], 0x5D);
            new_controller = new RGBController_Corsair(new_corsair);
            rgb_controllers.push_back(new_controller);
        }

        // Check for Corsair controller at 0x5E
        if (TestForCorsairController(busses[bus], 0x5E))
        {
            new_corsair = new CorsairController(busses[bus], 0x5E);
            new_controller = new RGBController_Corsair(new_corsair);
            rgb_controllers.push_back(new_controller);
        }

        // Check for Corsair controller at 0x5F
        if (TestForCorsairController(busses[bus], 0x5F))
        {
            new_corsair = new CorsairController(busses[bus], 0x5F);
            new_controller = new RGBController_Corsair(new_corsair);
            rgb_controllers.push_back(new_controller);
        }
    }

}   /* DetectCorsairControllers() */