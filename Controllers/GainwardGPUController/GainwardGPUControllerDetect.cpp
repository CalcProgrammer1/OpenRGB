/*-----------------------------------------*\
|  GainwardGPUControllerDetect.cpp          |
|                                           |
|  Driver for Gainward RGB on GPUs          |
|                                           |
|  TheRogueZeta 11/05/2020                  |
\*-----------------------------------------*/

#include "Detector.h"
#include "GainwardGPUController.h"
#include "RGBController.h"
#include "RGBController_GainwardGPU.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std::chrono_literals;

typedef struct
{
    int             pci_vendor;
    int             pci_device;
    int             pci_subsystem_vendor;
    int             pci_subsystem_device;
    const char *    name;
} gpu_pci_device;

#define GPU_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[ 0 ]))

static const gpu_pci_device device_list[] =
{
    { NVIDIA_VEN,   NVIDIA_GTX1080_DEV,     GAINWARD_SUB_VEN,       GAINWARD_GTX_1080_PHOENIX,      "Gainward GTX 1080 Phoenix"             },
};

/******************************************************************************************\
*                                                                                          *
*   TestForGainwardGPUController                                                           *
*                                                                                          *
*       Tests the given address to see if a Gainward GPU controller exists there.          *
*                                                                                          *
\******************************************************************************************/

bool TestForGainwardGPUController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    pass = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);

    return(pass);

}   /* TestForGainwardGPUController() */


/******************************************************************************************\
*                                                                                          *
*   DetectGainwardGPUControllers                                                           *
*                                                                                          *
*       Detect Gainward GPU controllers on the enumerated I2C busses.                      *
*                                                                                          *
\******************************************************************************************/

void DetectGainwardGPUControllers(std::vector<i2c_smbus_interface*> &busses, std::vector<RGBController*> &rgb_controllers)
{
    GainwardGPUController* new_GainwardGPU;
    RGBController_GainwardGPU* new_controller;

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        // Check for Gainward controller at 0x08
        for(unsigned int dev_idx = 0; dev_idx < GPU_NUM_DEVICES; dev_idx++)
        {
            if(busses[bus]->pci_vendor           == device_list[dev_idx].pci_vendor           &&
               busses[bus]->pci_device           == device_list[dev_idx].pci_device           &&
               busses[bus]->pci_subsystem_vendor == device_list[dev_idx].pci_subsystem_vendor &&
               busses[bus]->pci_subsystem_device == device_list[dev_idx].pci_subsystem_device)
            {
                if (TestForGainwardGPUController(busses[bus], 0x08))
                {
                    new_GainwardGPU         = new GainwardGPUController(busses[bus], 0x08);
                    new_controller          = new RGBController_GainwardGPU(new_GainwardGPU);
                    new_controller->name    = device_list[dev_idx].name;
                    rgb_controllers.push_back(new_controller);
                }
            }
        }
    }

} /* DetectGainwardGPUControllers() */

REGISTER_I2C_DETECTOR("Gainward GPU", DetectGainwardGPUControllers);
