#include "Detector.h"
#include "SapphireGPUController.h"
#include "RGBController.h"
#include "RGBController_SapphireGPU.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

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
    { AMD_GPU_VEN,  AMD_RX580_DEV,  SAPPHIRE_SUB_VEN,   SAPPHIRE_RX580_NITRO_PLUS_SUB_DEV,  "Sapphire RX580 Nitro+" },
};

/******************************************************************************************\
*                                                                                          *
*   TestForSapphireGPUController                                                           *
*                                                                                          *
*       Tests the given address to see if an Sapphire controller exists there.  First      *
*       does a quick write to test for a response                                          *
*                                                                                          *
\******************************************************************************************/

bool TestForSapphireGPUController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;
    int res;

    //Read a byte to test for presence
    res = bus->i2c_smbus_read_byte(address);

    if (res >= 0)
    {
        pass = true;
    }

    return(pass);

}   /* TestForSapphireGPUController() */

/******************************************************************************************\
*                                                                                          *
*   DetectSapphireGPUControllers                                                           *
*                                                                                          *
*       Detect Sapphire GPU controllers on the enumerated I2C busses at address 0x55.      *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where Sapphire GPU device is connected    *
*           dev - I2C address of Sapphire GPU device                                       *
*                                                                                          *
\******************************************************************************************/

void DetectSapphireGPUControllers(std::vector<i2c_smbus_interface*>& busses, std::vector<RGBController*>& rgb_controllers)
{
    SapphireGPUController* new_sapphire_gpu;
    RGBController_SapphireGPU* new_controller;

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        for(unsigned int dev_idx = 0; dev_idx < GPU_NUM_DEVICES; dev_idx++)
        {
            if(busses[bus]->pci_vendor           == device_list[dev_idx].pci_vendor           &&
               busses[bus]->pci_device           == device_list[dev_idx].pci_device           &&
               busses[bus]->pci_subsystem_vendor == device_list[dev_idx].pci_subsystem_vendor &&
               busses[bus]->pci_subsystem_device == device_list[dev_idx].pci_subsystem_device)
            {
                if(TestForSapphireGPUController(busses[bus], 0x55))
                {
                    new_sapphire_gpu     = new SapphireGPUController(busses[bus], 0x55);
                    new_controller       = new RGBController_SapphireGPU(new_sapphire_gpu);
                    new_controller->name = device_list[dev_idx].name;
                    rgb_controllers.push_back(new_controller);
                }
            }
        }
    }
}   /* DetectSapphireGPUControllers() */

REGISTER_I2C_DETECTOR("Sapphire GPU", DetectSapphireGPUControllers);
