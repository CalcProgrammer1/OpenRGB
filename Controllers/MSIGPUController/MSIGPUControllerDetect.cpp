/*-----------------------------------------*\
|  MSIGPUControllerDetect.cpp               |
|                                           |
|  Driver for MSI GPUs                      |
|                                           |
\*-----------------------------------------*/

#include "Detector.h"
#include "MSIGPUController.h"
#include "RGBController.h"
#include "RGBController_MSIGPU.h"
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
} msi_gpu_pci_device;

#define MSI_GPU_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[ 0 ]))

static const msi_gpu_pci_device device_list[] =
{
    { NVIDIA_VEN,   NVIDIA_GTX1070_DEV,         MSI_SUB_VEN,    MSI_GTX1070_GAMING_X_SUB_DEV,           "MSI GeForce GTX 1070 Gaming X"                 },
    { NVIDIA_VEN,   NVIDIA_GTX1660_DEV,         MSI_SUB_VEN,    MSI_GTX1660_GAMING_X_6G_SUB_DEV,        "MSI GeForce GTX 1660 Gaming X 6G"              },
    { NVIDIA_VEN,   NVIDIA_GTX1660TI_DEV,       MSI_SUB_VEN,    MSI_GTX1660TI_GAMING_X_6G_SUB_DEV,      "MSI GeForce GTX 1660Ti Gaming X 6G"            },
    { NVIDIA_VEN,   NVIDIA_GTX1660S_DEV,        MSI_SUB_VEN,    MSI_GTX1660S_GAMING_6G_SUB_DEV,         "MSI GeForce GTX 1660 Super Gaming 6G"          },
    { NVIDIA_VEN,   NVIDIA_GTX1660S_DEV,        MSI_SUB_VEN,    MSI_GTX1660S_GAMING_X_6G_SUB_DEV,       "MSI GeForce GTX 1660 Super Gaming X 6G"        },
    { NVIDIA_VEN,   NVIDIA_RTX2060_TU104_DEV,   MSI_SUB_VEN,    MSI_RTX2060_GAMING_Z_6G_SUB_DEV,        "MSI GeForce RTX 2060 Gaming Z 6G"              },
    { NVIDIA_VEN,   NVIDIA_RTX2060_TU106_DEV,   MSI_SUB_VEN,    MSI_RTX2060_GAMING_Z_6G_SUB_DEV,        "MSI GeForce RTX 2060 Gaming Z 6G"              },
    { NVIDIA_VEN,   NVIDIA_RTX2060_TU106_DEV,   MSI_SUB_VEN,    MSI_RTX2060_GAMING_Z_6G_SUB_DEV_2,      "MSI GeForce RTX 2060 Gaming Z 6G"              },
    { NVIDIA_VEN,   NVIDIA_RTX2060S_OC_DEV,     MSI_SUB_VEN,    MSI_RTX2060S_GAMING_X_SUB_DEV,          "MSI GeForce RTX 2060 Super Gaming X"           },
    { NVIDIA_VEN,   NVIDIA_RTX2060S_OC_DEV,     MSI_SUB_VEN,    MSI_RTX2060S_ARMOR_OC_SUB_DEV,          "MSI GeForce RTX 2060 Super ARMOR OC"           },
    { NVIDIA_VEN,   NVIDIA_RTX2070_OC_DEV,      MSI_SUB_VEN,    MSI_RTX2070_GAMING_Z_SUB_DEV,           "MSI GeForce RTX 2070 Gaming Z"                 },
    { NVIDIA_VEN,   NVIDIA_RTX2070_DEV,         MSI_SUB_VEN,    MSI_RTX2070_GAMING_SUB_DEV,             "MSI GeForce RTX 2070 Gaming"                   },
    { NVIDIA_VEN,   NVIDIA_RTX2070_DEV,         MSI_SUB_VEN,    MSI_RTX2070_ARMOR_SUB_DEV,              "MSI GeForce RTX 2070 ARMOR"                    },
    { NVIDIA_VEN,   NVIDIA_RTX2070_OC_DEV,      MSI_SUB_VEN,    MSI_RTX2070_ARMOR_SUB_DEV,              "MSI GeForce RTX 2070 ARMOR OC"                 },
    { NVIDIA_VEN,   NVIDIA_RTX2070S_DEV,        MSI_SUB_VEN,    MSI_RTX2070S_GAMING_TRIO_SUB_DEV,       "MSI GeForce RTX 2070 Super Gaming Trio"        },
    { NVIDIA_VEN,   NVIDIA_RTX2070S_DEV,        MSI_SUB_VEN,    MSI_RTX2070S_GAMING_X_SUB_DEV,          "MSI GeForce RTX 2070 Super Gaming X"           },
    { NVIDIA_VEN,   NVIDIA_RTX2070S_DEV,        MSI_SUB_VEN,    MSI_RTX2070S_GAMING_X_TRIO_SUB_DEV,     "MSI GeForce RTX 2070 Super Gaming X Trio"      },
    { NVIDIA_VEN,   NVIDIA_RTX2070S_DEV,        MSI_SUB_VEN,    MSI_RTX2070S_GAMING_Z_TRIO_SUB_DEV,     "MSI GeForce RTX 2070 Super Gaming Z Trio"      },
    { NVIDIA_VEN,   NVIDIA_RTX2080_DEV,         MSI_SUB_VEN,    MSI_RTX2080_GAMING_TRIO_SUB_DEV,        "MSI GeForce RTX 2080 Gaming Trio"              },
    { NVIDIA_VEN,   NVIDIA_RTX2080_A_DEV,       MSI_SUB_VEN,    MSI_RTX2080_GAMING_X_TRIO_SUB_DEV,      "MSI GeForce RTX 2080 Gaming X Trio"            },
    { NVIDIA_VEN,   NVIDIA_RTX2080_A_DEV,       MSI_SUB_VEN,    MSI_RTX2080_SEA_HAWK_EK_X_SUB_DEV,      "MSI GeForce RTX 2080 Sea Hawk EK X"            },
    { NVIDIA_VEN,   NVIDIA_RTX2080S_DEV,        MSI_SUB_VEN,    MSI_RTX2080S_GAMING_X_TRIO_SUB_DEV,     "MSI GeForce RTX 2080 Super Gaming X Trio"      },
    { NVIDIA_VEN,   NVIDIA_RTX2080TI_DEV,       MSI_SUB_VEN,    MSI_RTX2080TI_GAMING_X_TRIO_SUB_DEV,    "MSI GeForce RTX 2080Ti Gaming X Trio"          },
    { NVIDIA_VEN,   NVIDIA_RTX2080TI_DEV,       MSI_SUB_VEN,    MSI_RTX2080TI_SEA_HAWK_EK_X_SUB_DEV,    "MSI GeForce RTX 2080Ti Sea Hawk EK X"          },
    { NVIDIA_VEN,   NVIDIA_RTX3060TI_DEV,       MSI_SUB_VEN,    MSI_RTX3070_GAMING_X_TRIO_SUB_DEV,      "MSI GeForce RTX 3060 Ti 8GB Gaming X Trio"     },
    { NVIDIA_VEN,   NVIDIA_RTX3070_DEV,         MSI_SUB_VEN,    MSI_RTX3070_GAMING_X_TRIO_SUB_DEV,      "MSI GeForce RTX 3070 8GB Gaming X Trio"        },
    { NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         MSI_SUB_VEN,    MSI_RTX3080_GAMING_X_TRIO_SUB_DEV,      "MSI GeForce RTX 3080 10GB Gaming X Trio"       },
    { NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         MSI_SUB_VEN,    MSI_RTX3080_SUPRIM_X_SUB_DEV,           "MSI GeForce RTX 3080 Suprim X 10G"             },
    { NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         MSI_SUB_VEN,    MSI_RTX3090_GAMING_X_TRIO_SUB_DEV,      "MSI GeForce RTX 3090 24GB Gaming X Trio"       },
    { NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         MSI_SUB_VEN,    MSI_RTX3090_SUPRIM_X_SUB_DEV,           "MSI GeForce RTX 3090 Suprim X 24G"             }, 
};


/******************************************************************************************\
*                                                                                          *
*   DetectMSIGPUControllers                                                               *
*                                                                                          *
*       Detect MSI GPU controllers on the enumerated I2C busses.                          *
*                                                                                          *
\******************************************************************************************/

void DetectMSIGPUControllers(std::vector<i2c_smbus_interface*> &busses)
{
    MSIGPUController* new_msi_gpu;
    RGBController_MSIGPU* new_controller;

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        for(unsigned int dev_idx = 0; dev_idx < MSI_GPU_NUM_DEVICES; dev_idx++)
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
                new_msi_gpu = new MSIGPUController(busses[bus]);
                new_controller = new RGBController_MSIGPU(new_msi_gpu);
                new_controller->name = device_list[dev_idx].name;
                ResourceManager::get()->RegisterRGBController(new_controller);
            }
        }
    }
} /* DetectMSIGPUControllers() */

REGISTER_I2C_DETECTOR("MSI GPU", DetectMSIGPUControllers);
