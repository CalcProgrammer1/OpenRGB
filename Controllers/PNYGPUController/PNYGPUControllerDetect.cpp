#include "Detector.h"
#include "PNYGPUController.h"
#include "RGBController.h"
#include "RGBController_PNYGPU.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

enum
{
    PNY_RGB,
};

typedef struct
{
    int             pci_vendor;
    int             pci_device;
    int             pci_subsystem_vendor;
    int             pci_subsystem_device;
    int             gpu_rgb_version;
    const char *    name;
} gpu_pci_device;

#define GPU_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[ 0 ]))

static const gpu_pci_device device_list[] =
{
    { NVIDIA_VEN,   NVIDIA_RTX3090_DEV,     PNY_SUB_VEN,   PNY_RTX_3090_XLR8_REVEL_EPIC_X_SUB_DEV,  PNY_RGB,    "PNY XLR8 Revel EPIC-X RTX 3090" },
};

/******************************************************************************************\
 *                                                                                        *
 *   DetectPNYGPUControllers                                                              *
 *                                                                                        *
 *       Detect PNY GPU controllers on the enumerated I2C busses at address 0x49.         *
 *                                                                                        *
 *       bus - pointer to i2c_smbus_interface where PNY GPU device is connected           *
 *       dev - I2C address of PNY GPU device                                              *
 *                                                                                        *
\******************************************************************************************/

void DetectPNYGPUControllers(std::vector<i2c_smbus_interface*>& busses)
{
    for(unsigned int bus = 0; bus < busses.size(); bus++)
    {
        for(unsigned int dev_idx = 0; dev_idx < GPU_NUM_DEVICES; dev_idx++)
        {
            if(busses[bus]->port_id != 1)
            {
                break;
            }

            if(busses[bus]->pci_vendor           == device_list[dev_idx].pci_vendor           &&
               busses[bus]->pci_device           == device_list[dev_idx].pci_device           &&
               busses[bus]->pci_subsystem_vendor == device_list[dev_idx].pci_subsystem_vendor &&
               busses[bus]->pci_subsystem_device == device_list[dev_idx].pci_subsystem_device)
            {
                switch(device_list[dev_idx].gpu_rgb_version)
                {
                    case PNY_RGB:
                        {
                            PNYGPUController*     new_controller;
                            RGBController_PNYGPU* new_rgbcontroller;

                            new_controller          = new PNYGPUController(busses[bus], 0x49);
                            new_rgbcontroller       = new RGBController_PNYGPU(new_controller);
                            new_rgbcontroller->name = device_list[dev_idx].name;
                            ResourceManager::get()->RegisterRGBController(new_rgbcontroller);
                        }
                        break;
                }
            }
        }
    }
}
/* DetectPNYGPUControllers() */
REGISTER_I2C_DETECTOR("PNY GPU", DetectPNYGPUControllers);
