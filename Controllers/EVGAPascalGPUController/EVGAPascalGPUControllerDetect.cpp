#include "Detector.h"
#include "EVGAGPUv1Controller.h"
#include "LogManager.h"
#include "RGBController.h"
#include "RGBController_EVGAGPUv1.h"
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
    { NVIDIA_VEN,   NVIDIA_GTX1070_DEV,         EVGA_SUB_VEN,   EVGA_GTX1070_FTW_DT_GAMING_SUB_DEV,     "EVGA GeForce GTX 1070 FTW DT Gaming"                   },
    { NVIDIA_VEN,   NVIDIA_GTX1070_DEV,         EVGA_SUB_VEN,   EVGA_GTX1070_FTW_SUB_DEV,               "EVGA GeForce GTX 1070 FTW"                             },
    { NVIDIA_VEN,   NVIDIA_GTX1070_DEV,         EVGA_SUB_VEN,   EVGA_GTX1070_FTW_HYBRID_SUB_DEV,        "EVGA GeForce GTX 1070 FTW HYBRID"                      },
    { NVIDIA_VEN,   NVIDIA_GTX1070TI_DEV,       EVGA_SUB_VEN,   EVGA_GTX1070TI_FTW2_SUB_DEV,            "EVGA GeForce GTX 1070 Ti FTW2"                         },
    { NVIDIA_VEN,   NVIDIA_GTX1080_DEV,         EVGA_SUB_VEN,   EVGA_GTX1080_FTW_SUB_DEV,               "EVGA GeForce GTX 1080 FTW"                             },
};
/******************************************************************************************\
*                                                                                          *
*   DetectEVGAGPUControllers                                                               *
*                                                                                          *
*       Detect EVGA Pascal GPU controllers on the enumerated I2C busses at address 0x49.   *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where EVGA GPU device is connected        *
*           dev - I2C address of EVGA GPU device                                           *
*                                                                                          *
\******************************************************************************************/

void DetectEVGAPascalGPUControllers(std::vector<i2c_smbus_interface*>& busses)
{
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
                LOG_DEBUG(GPU_DETECT_MESSAGE, EVGAGPUV1_CONTROLLER_NAME, bus, device_list[dev_idx].pci_device, device_list[dev_idx].pci_subsystem_device, device_list[dev_idx].name );
                EVGAGPUv1Controller*     new_controller;
                RGBController_EVGAGPUv1* new_rgbcontroller;

                new_controller          = new EVGAGPUv1Controller(busses[bus], 0x49);
                new_rgbcontroller       = new RGBController_EVGAGPUv1(new_controller);
                new_rgbcontroller->name = device_list[dev_idx].name;
                ResourceManager::get()->RegisterRGBController(new_rgbcontroller);
            }
        }
    }
}   /* DetectEVGAPascalGPUControllers() */

REGISTER_I2C_DETECTOR("EVGA Pascal GPU", DetectEVGAPascalGPUControllers);
