#include "Detector.h"
#include "PolychromeController.h"
#include "RGBController.h"
#include "RGBController_Polychrome.h"
#include "i2c_smbus.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************************\
*                                                                                          *
*   TestForPolychromeController                                                            *
*                                                                                          *
*       Tests the given address to see if an ASRock Polychrome RGB controller exists there.*
*       First does a quick write to test for a response                                    *
*                                                                                          *
\******************************************************************************************/

bool TestForPolychromeController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    int res = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);

    if (res >= 0)
    {
        pass = true;
    }

    return(pass);

}   /* TestForPolychromeController() */

/******************************************************************************************\
*                                                                                          *
*   DetectPolychromeControllers                                                            *
*                                                                                          *
*       Detect ASRock Polychrome RGB controllers on the enumerated I2C busses at address   *
*       0x6A.                                                                              *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where Polychrome device is connected      *
*           dev - I2C address of Polychrome device                                         *
*                                                                                          *
\******************************************************************************************/

void DetectPolychromeControllers(std::vector<i2c_smbus_interface*>& busses, std::vector<RGBController*>& rgb_controllers)
{
    PolychromeController* new_polychrome;
    RGBController_Polychrome* new_controller;

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        // Check for Polychrome controller at 0x6A
        if (TestForPolychromeController(busses[bus], 0x6A))
        {
            new_polychrome = new PolychromeController(busses[bus], 0x6A);

            if(new_polychrome->GetLEDCount() != 0)
            {
                new_controller = new RGBController_Polychrome(new_polychrome);
                rgb_controllers.push_back(new_controller);
            }
            else
            {
                delete new_polychrome;
            }   
        }
    }

}   /* DetectPolychromeControllers() */

REGISTER_I2C_DETECTOR("ASRock Polychrome", DetectPolychromeControllers);
