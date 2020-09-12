#include "Detector.h"
#include "SapphireGPUController.h"
#include "RGBController.h"
#include "RGBController_SapphireGPU.h"
#include "i2c_smbus.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

/*-----------------------------------------------------*\
| AMD vendor ID                                         |
\*-----------------------------------------------------*/
#define AMD_VEN                                 0x1002

/*-----------------------------------------------------*\
| AMD device IDs                                        |
\*-----------------------------------------------------*/
#define AMD_RX580_DEV                           0x67DF

/*-----------------------------------------------------*\
| Sapphire sub-vendor ID                                |
\*-----------------------------------------------------*/
#define SAPPHIRE_SUB_VEN                        0x1DA2

/*-----------------------------------------------------*\
| Sapphire sub-device IDs                               |
\*-----------------------------------------------------*/
#define SAPPHIRE_RX580_NITRO_PLUS_SUB_DEV       0xE366

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
    { AMD_VEN,      AMD_RX580_DEV,  SAPPHIRE_SUB_VEN,   SAPPHIRE_RX580_NITRO_PLUS_SUB_DEV,  "Sapphire RX580 Nitro+" },
};

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
            if (busses[bus]->port_id != 1)
            {
                break;
            }

            if(busses[bus]->pci_vendor           == device_list[dev_idx].pci_vendor           &&
               busses[bus]->pci_device           == device_list[dev_idx].pci_device           &&
               busses[bus]->pci_subsystem_vendor == device_list[dev_idx].pci_subsystem_vendor &&
               busses[bus]->pci_subsystem_device == device_list[dev_idx].pci_subsystem_device)
            {
                new_sapphire_gpu     = new SapphireGPUController(busses[bus], 0x55);
                new_controller       = new RGBController_SapphireGPU(new_sapphire_gpu);
                new_controller->name = device_list[dev_idx].name;
                rgb_controllers.push_back(new_controller);
            }
        }
    }
}   /* DetectSapphireGPUControllers() */

REGISTER_I2C_DETECTOR("Sapphire GPU", DetectSapphireGPUControllers);
