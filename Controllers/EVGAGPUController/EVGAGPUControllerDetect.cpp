#include "Detector.h"
#include "EVGAGPUController.h"
#include "RGBController.h"
#include "RGBController_EVGAGPU.h"
#include "i2c_smbus.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

/*-----------------------------------------------------*\
| NVidia vendor ID                                      |
\*-----------------------------------------------------*/
#define NVIDIA_VEN                              0x10DE

/*-----------------------------------------------------*\
| NVidia device IDs                                     |
\*-----------------------------------------------------*/
#define NVIDIA_GTX1070_DEV                      0x1B81

/*-----------------------------------------------------*\
| EVGA sub-vendor ID                                    |
\*-----------------------------------------------------*/
#define EVGA_SUB_VEN                            0x3842

/*-----------------------------------------------------*\
| EVGA sub-device IDs                                   |
\*-----------------------------------------------------*/
#define EVGA_GTX1070_FTW_SUB_DEV                0x6276

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
    { NVIDIA_VEN,   NVIDIA_GTX1070_DEV,     EVGA_SUB_VEN,   EVGA_GTX1070_FTW_SUB_DEV,       "EVGA GeForce GTX 1070 FTW"     },
};

/******************************************************************************************\
*                                                                                          *
*   DetectEVGAGPUControllers                                                               *
*                                                                                          *
*       Detect EVGA GPU controllers on the enumerated I2C busses at address 0x49.          *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where EVGA GPU device is connected        *
*           dev - I2C address of EVGA GPU device                                           *
*                                                                                          *
\******************************************************************************************/

void DetectEVGAGPUControllers(std::vector<i2c_smbus_interface*>& busses, std::vector<RGBController*>& rgb_controllers)
{
    EVGAGPUController* new_evga_gpu;
    RGBController_EVGAGPU* new_controller;

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
                new_evga_gpu = new EVGAGPUController(busses[bus], 0x49);
                new_controller = new RGBController_EVGAGPU(new_evga_gpu);
                new_controller->name = device_list[dev_idx].name;
                rgb_controllers.push_back(new_controller);
            }
        }
    }
}   /* DetectEVGAGPUControllers() */

REGISTER_I2C_DETECTOR("EVGA GPU", DetectEVGAGPUControllers);
