#include "CorsairProController.h"
#include "RGBController.h"
#include "RGBController_CorsairPro.h"
#include "i2c_smbus.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************************\
*                                                                                          *
*   TestForCorsairProController                                                            *
*                                                                                          *
*       Tests the given address to see if a Corsair Pro controller exists there.           *
*                                                                                          *
\******************************************************************************************/

bool TestForCorsairProController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    int res = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);

    if (res >= 0)
    {
        pass = true;

        res = bus->i2c_smbus_read_byte_data(address, 0x24);

        if (res != 0x02)
        {
            pass = false;
        }

        res = bus->i2c_smbus_read_byte_data(address, 0x25);

        if (res != 0x02)
        {
            pass = false;
        }

        res = bus->i2c_smbus_read_byte_data(address, 0x43);

        if (res != 0x1C)
        {
            pass = false;
        }

        res = bus->i2c_smbus_read_byte_data(address, 0x44);

        if (res != 0x03)
        {
            pass = false;
        }
    }

    return(pass);

}   /* TestForCorsairProController() */

/******************************************************************************************\
*                                                                                          *
*   DetectCorsairProControllers                                                            *
*                                                                                          *
*       Detect Corsair Pro controllers on the enumerated I2C busses.                       *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where Aura device is connected            *
*           dev - I2C address of Aura device                                               *
*                                                                                          *
\******************************************************************************************/

void DetectCorsairProControllers(std::vector<i2c_smbus_interface*> &busses, std::vector<RGBController*> &rgb_controllers)
{
    CorsairProController* new_corsair_pro;
    RGBController_CorsairPro* new_controller;

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        // Check for Corsair controller at 0x58
        if (TestForCorsairProController(busses[bus], 0x58))
        {
            new_corsair_pro = new CorsairProController(busses[bus], 0x58);
            new_controller = new RGBController_CorsairPro(new_corsair_pro);
            rgb_controllers.push_back(new_controller);
        }

        // Check for Corsair controller at 0x59
        if (TestForCorsairProController(busses[bus], 0x59))
        {
            new_corsair_pro = new CorsairProController(busses[bus], 0x59);
            new_controller = new RGBController_CorsairPro(new_corsair_pro);
            rgb_controllers.push_back(new_controller);
        }

        // Check for Corsair controller at 0x5A
        if (TestForCorsairProController(busses[bus], 0x5A))
        {
            new_corsair_pro = new CorsairProController(busses[bus], 0x5A);
            new_controller = new RGBController_CorsairPro(new_corsair_pro);
            rgb_controllers.push_back(new_controller);
        }

        // Check for Corsair controller at 0x5B
        if (TestForCorsairProController(busses[bus], 0x5B))
        {
            new_corsair_pro = new CorsairProController(busses[bus], 0x5B);
            new_controller = new RGBController_CorsairPro(new_corsair_pro);
            rgb_controllers.push_back(new_controller);
        }

        // Check for Corsair controller at 0x5C
        if (TestForCorsairProController(busses[bus], 0x5C))
        {
            new_corsair_pro = new CorsairProController(busses[bus], 0x5C);
            new_controller = new RGBController_CorsairPro(new_corsair_pro);
            rgb_controllers.push_back(new_controller);
        }

        // Check for Corsair controller at 0x5D
        if (TestForCorsairProController(busses[bus], 0x5D))
        {
            new_corsair_pro = new CorsairProController(busses[bus], 0x5D);
            new_controller = new RGBController_CorsairPro(new_corsair_pro);
            rgb_controllers.push_back(new_controller);
        }
    }

}   /* DetectCorsairProControllers() */