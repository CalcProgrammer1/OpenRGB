/*-----------------------------------------*\
|  AsusAuraGPUControllerDetect.cpp          |
|                                           |
|  Driver for ASUS Aura RGB on GPUs         |
|                                           |
|  Jan Rettig (Klapstuhl) 14.02.2020        |
\*-----------------------------------------*/

#include "Detector.h"
#include "AsusAuraGPUController.h"
#include "RGBController.h"
#include "RGBController_AsusAuraGPU.h"
#include "i2c_smbus.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std::chrono_literals;

/*-------------------------------------------------------------*\
| This list contains the available I2C addresses for Aura GPUs  |
\*-------------------------------------------------------------*/
#define AURA_GPU_ADDRESS_COUNT 3

static const unsigned char aura_gpu_addresses[] =
{
    0x29,
    0x2A,
    0x60
};


/******************************************************************************************\
*                                                                                          *
*   TestForAuraGPUController                                                               *
*                                                                                          *
*       Tests the given address to see if an Aura GPU controller exists there.             *
*                                                                                          *
\******************************************************************************************/

bool TestForAsusAuraGPUController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    unsigned char aura_gpu_magic_high = bus->i2c_smbus_read_byte_data(address, 0x20);                        // High Byte of magic (0x15)                        
    unsigned char aura_gpu_magic_low = bus->i2c_smbus_read_byte_data(address, 0x21);                         // Low Byte of magic (0x89)

    if((aura_gpu_magic_high << 8) + aura_gpu_magic_low == AURA_GPU_MAGIC_VAL)
    {
        pass = true;
    }

    return(pass);

}   /* TestForAuraGPUController() */


/******************************************************************************************\
*                                                                                          *
*   DetectAuraGPUControllers                                                               *
*                                                                                          *
*       Detect Aura GPU controllers on the enumerated I2C busses.                          *
*                                                                                          *
\******************************************************************************************/

void DetectAsusAuraGPUControllers(std::vector<i2c_smbus_interface*> &busses, std::vector<RGBController*> &rgb_controllers)
{
    AuraGPUController* new_aura_gpu;
    RGBController_AuraGPU* new_controller;

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        // Add Aura-enabled GPU controllers
        for (unsigned int address_list_idx = 0; address_list_idx < AURA_GPU_ADDRESS_COUNT; address_list_idx++)
        {
            if (TestForAsusAuraGPUController(busses[bus], aura_gpu_addresses[address_list_idx]))
            {
                new_aura_gpu = new AuraGPUController(busses[bus], aura_gpu_addresses[address_list_idx]);
                new_controller = new RGBController_AuraGPU(new_aura_gpu);
                rgb_controllers.push_back(new_controller);
            }

            std::this_thread::sleep_for(1ms);
        }
    }
} /* DetectAuraGPUControllers() */

REGISTER_I2C_DETECTOR("ASUS Aura GPU", DetectAsusAuraGPUControllers);
