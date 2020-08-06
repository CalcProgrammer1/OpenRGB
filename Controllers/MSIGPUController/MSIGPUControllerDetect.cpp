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
#define NVIDIA_RTX2060_DEV                      0x1F08
#define NVIDIA_RTX2060S_DEV                     0x1F06
#define NVIDIA_RTX2070_DEV                      0x1F02
#define NVIDIA_RTX2070S_DEV                     0x1E84
#define NVIDIA_RTX2080_DEV                      0x1E87
#define NVIDIA_RTX2080S_DEV                     0x1E81
#define NVIDIA_RTX2080TI_DEV                    0x1E07

/*-----------------------------------------------------*\
| MSI sub-vendor ID                                     |
\*-----------------------------------------------------*/
#define MSI_SUB_VEN                             0x1462

/*-----------------------------------------------------*\
| MSI sub-device IDs                                    |
\*-----------------------------------------------------*/
#define MSI_RTX2060S_GAMING_X_SUB_DEV           0xC752
#define MSI_RTX2070S_GAMING_X_SUB_DEV           0x373e
#define MSI_RTX2070S_GAMING_X_TRIO_SUB_DEV      0xC726
#define MSI_RTX2080_GAMING_X_TRIO_SUB_DEV       0x3726
#define MSI_RTX2080S_GAMING_X_TRIO_SUB_DEV      0xC724
#define MSI_RTX2080TI_GAMING_X_TRIO_SUB_DEV     0x3715
#define MSI_RTX2060_GAMING_Z_6G_SUB_DEV         0x3754
#define MSI_RTX2070_ARMOR_SUB_DEV               0x3734
#define MSI_RTX2060S_ARMOR_OC_SUB_DEV           0xC754
#define MSI_RTX2080_SEA_HAWK_EK_X_SUB_DEV       0x3728
#define MSI_RTX2080TI_SEA_HAWK_EK_X_SUB_DEV     0x3717

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
    { NVIDIA_VEN,   NVIDIA_RTX2060S_DEV,    MSI_SUB_VEN,    MSI_RTX2060S_GAMING_X_SUB_DEV,          "MSI GeForce RTX 2060 Super Gaming X"           },
    { NVIDIA_VEN,   NVIDIA_RTX2070S_DEV,    MSI_SUB_VEN,    MSI_RTX2070S_GAMING_X_SUB_DEV,          "MSI GeForce RTX 2070 Super Gaming X"           },
    { NVIDIA_VEN,   NVIDIA_RTX2070S_DEV,    MSI_SUB_VEN,    MSI_RTX2070S_GAMING_X_TRIO_SUB_DEV,     "MSI GeForce RTX 2070 Super Gaming X Trio"      },
    { NVIDIA_VEN,   NVIDIA_RTX2080_DEV,     MSI_SUB_VEN,    MSI_RTX2080_GAMING_X_TRIO_SUB_DEV,      "MSI GeForce RTX 2080 Gaming X Trio"            },
    { NVIDIA_VEN,   NVIDIA_RTX2080S_DEV,    MSI_SUB_VEN,    MSI_RTX2080S_GAMING_X_TRIO_SUB_DEV,     "MSI GeForce RTX 2080 Super Gaming X Trio"      },
    { NVIDIA_VEN,   NVIDIA_RTX2080TI_DEV,   MSI_SUB_VEN,    MSI_RTX2080TI_GAMING_X_TRIO_SUB_DEV,    "MSI GeForce RTX 2080Ti Gaming X Trio"          },
    { NVIDIA_VEN,   NVIDIA_RTX2060_DEV,     MSI_SUB_VEN,    MSI_RTX2060_GAMING_Z_6G_SUB_DEV,        "MSI GeForce RTX 2060 Gaming Z 6G"              },
    { NVIDIA_VEN,   NVIDIA_RTX2060S_DEV,    MSI_SUB_VEN,    MSI_RTX2060S_ARMOR_OC_SUB_DEV,          "MSI GeForce RTX 2060 Super ARMOR OC"           },
    { NVIDIA_VEN,   NVIDIA_RTX2070_DEV,     MSI_SUB_VEN,    MSI_RTX2070_ARMOR_SUB_DEV,              "MSI GeForce RTX 2070 ARMOR"                    },
    { NVIDIA_VEN,   NVIDIA_RTX2080_DEV,     MSI_SUB_VEN,    MSI_RTX2080_SEA_HAWK_EK_X_SUB_DEV,      "MSI GeForce RTX 2080 Sea Hawk EK X"            },
    { NVIDIA_VEN,   NVIDIA_RTX2080TI_DEV,   MSI_SUB_VEN,    MSI_RTX2080TI_SEA_HAWK_EK_X_SUB_DEV,    "MSI GeForce RTX 2080Ti Sea Hawk EK X"          },
};


/******************************************************************************************\
*                                                                                          *
*   DetectMSIGPUControllers                                                               *
*                                                                                          *
*       Detect MSI GPU controllers on the enumerated I2C busses.                          *
*                                                                                          *
\******************************************************************************************/

void DetectMSIGPUControllers(std::vector<i2c_smbus_interface*> &busses, std::vector<RGBController*> &rgb_controllers)
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
                rgb_controllers.push_back(new_controller);
            }
        }
    }
} /* DetectMSIGPUControllers() */

REGISTER_I2C_DETECTOR("MSI GPU", DetectMSIGPUControllers);
