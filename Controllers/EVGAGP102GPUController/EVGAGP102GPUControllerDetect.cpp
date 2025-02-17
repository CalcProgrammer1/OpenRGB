/*---------------------------------------------------------*\
| EVGAGP102ControllerDetect.cpp                             |
|                                                           |
|   Detector for EVGA GP102 GPU                             |
|                                                           |
|   Fabricio Murta (avengerx)                   31 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <vector>
#include "Detector.h"
#include "EVGAGP102Controller.h"
#include "LogManager.h"
#include "RGBController_EVGAGP102.h"
#include "i2c_smbus.h"
#include "pci_ids.h"

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
    { NVIDIA_VEN,   NVIDIA_GTX1070_DEV,         EVGA_SUB_VEN,   EVGA_GTX1070_FTW2_SUB_DEV,              "EVGA GeForce GTX 1070 FTW2 Gaming"                     },
    { NVIDIA_VEN,   NVIDIA_GTX1080_DEV,         EVGA_SUB_VEN,   EVGA_GTX1080_FTW2_SUB_DEV,              "EVGA GeForce GTX 1080 FTW2 Gaming"                     },
    { NVIDIA_VEN,   NVIDIA_GTX1080_DEV,         EVGA_SUB_VEN,   EVGA_GTX1080_FTW2_11G_SUB_DEV,          "EVGA GeForce GTX 1080 FTW2 11G"                        },
    { NVIDIA_VEN,   NVIDIA_GTX1080_DEV,         EVGA_SUB_VEN,   EVGA_GTX1080_FTW2_DT_SUB_DEV,           "EVGA GeForce GTX 1080 FTW2 DT"                         },
    { NVIDIA_VEN,   NVIDIA_GTX1080TI_DEV,       EVGA_SUB_VEN,   EVGA_GTX1080TI_SC2_GAMING_SUB_DEV,      "EVGA GeForce GTX 1080 Ti SC2 Gaming"                   },
    { NVIDIA_VEN,   NVIDIA_GTX1080TI_DEV,       EVGA_SUB_VEN,   EVGA_GTX1080TI_FTW3_SUB_DEV,            "EVGA GeForce GTX 1080 Ti FTW3"                         },
    { NVIDIA_VEN,   NVIDIA_GTX1080TI_DEV,       EVGA_SUB_VEN,   EVGA_GTX1080TI_FTW3_HYBRID_SUB_DEV,     "EVGA GeForce GTX 1080 Ti FTW3 Hybrid"                  },
    { NVIDIA_VEN,   NVIDIA_GTX1080TI_DEV,       EVGA_SUB_VEN,   EVGA_GTX1080TI_KINGPIN_SUB_DEV,         "EVGA GeForce GTX 1080 Ti K|NGP|N"                      },
};
/******************************************************************************************\
*                                                                                          *
*   DetectEVGAGP102GPUControllers                                                          *
*                                                                                          *
*       Detect EVGA GP102 GPU controllers on the enumerated I2C busses at address 0x49.    *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where EVGA GPU device is connected        *
*           dev - I2C address of EVGA GPU device                                           *
*                                                                                          *
\******************************************************************************************/

void DetectEVGAGP102GPUControllers(std::vector<i2c_smbus_interface*>& busses)
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
                LOG_DEBUG(GPU_DETECT_MESSAGE, EVGA_GP102_CONTROLLER_NAME, bus, device_list[dev_idx].pci_device, device_list[dev_idx].pci_subsystem_device, device_list[dev_idx].name );
                RGBController_EVGAGP102* new_rgbcontroller;
                std::vector<EVGAGP102Controller*>   controllers;

                for(unsigned int i = 0; i < sizeof(gpuzoneinfos) / sizeof(zoneinfo); i++)
                {
                    EVGAGP102Controller* contr = new EVGAGP102Controller(busses[bus], gpuzoneinfos[i]);
                    if (contr->IsValid())
                    {
                        controllers.push_back(contr);
                    }
                    else
                    {
                        delete contr;
                    }
                }
                if(controllers.size() != 0)
                {
                    new_rgbcontroller       = new RGBController_EVGAGP102(controllers);
                    new_rgbcontroller->name = device_list[dev_idx].name;
                    ResourceManager::get()->RegisterRGBController(new_rgbcontroller);
                }
            }
        }
    }
}   /* DetectEVGAGP102GPUControllers() */

REGISTER_I2C_DETECTOR("EVGA GP102 GPU", DetectEVGAGP102GPUControllers);
