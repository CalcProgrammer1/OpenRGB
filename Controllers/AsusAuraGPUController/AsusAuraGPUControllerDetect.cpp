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
    unsigned char   controller_address;
    const char *    name;
} gpu_pci_device;

#define GPU_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[ 0 ]))

static const gpu_pci_device device_list[] =
{
    { NVIDIA_VEN,       NVIDIA_GTX1060_DEV,         ASUS_SUB_VEN,       ASUS_GTX1060_STRIX,                     0x29,   "ASUS GTX 1060 Strix"                       },
    { NVIDIA_VEN,       NVIDIA_GTX1070_DEV,         ASUS_SUB_VEN,       ASUS_GTX1070_STRIX_GAMING,              0x29,   "ASUS GTX 1070 Strix Gaming"                },
    { NVIDIA_VEN,       NVIDIA_GTX1070_DEV,         ASUS_SUB_VEN,       ASUS_GTX1070_STRIX_OC,                  0x29,   "ASUS GTX 1070 Strix OC"                    },
    { NVIDIA_VEN,       NVIDIA_GTX1080_DEV,         ASUS_SUB_VEN,       ASUS_GTX1080_STRIX,                     0x29,   "ASUS GTX 1080 Strix OC"                    },
    { NVIDIA_VEN,       NVIDIA_GTX1080_DEV,         ASUS_SUB_VEN,       ASUS_ROG_STRIX_GTX1080_A8G_GAMING,      0x29,   "ASUS ROG Strix GTX1080 A8G Gaming"         },
    { NVIDIA_VEN,       NVIDIA_GTX1080_DEV,         ASUS_SUB_VEN,       ASUS_ROG_STRIX_GTX1080_O8G_GAMING,      0x29,   "ASUS ROG Strix GTX1080 O8G Gaming"         },
    { NVIDIA_VEN,       NVIDIA_GTX1080TI_DEV,       ASUS_SUB_VEN,       ASUS_ROG_STRIX_GTX1080TI_GAMING,        0x29,   "ASUS ROG Strix GTX1080 Ti Gaming"          },
    { NVIDIA_VEN,       NVIDIA_GTX1660TI_DEV,       ASUS_SUB_VEN,       ASUS_ROG_GTX1660TI_OC,                  0x2A,   "ASUS ROG GTX 1660 Ti OC 6G"                },
    { NVIDIA_VEN,       NVIDIA_RTX2060_TU106_DEV,   ASUS_SUB_VEN,       ASUS_ROG_STRIX_RTX2060_EVO_GAMING,      0x2A,   "ASUS ROG STRIX RTX 2060 EVO Gaming 6G"     },
    { NVIDIA_VEN,       NVIDIA_RTX2060S_DEV,        ASUS_SUB_VEN,       ASUS_ROG_STRIX_RTX2060S_A8G_EVO_GAMING, 0x2A,   "ASUS ROG STRIX RTX 2060S A8G EVO Gaming 6G"},
    { NVIDIA_VEN,       NVIDIA_RTX2070_OC_DEV,      ASUS_SUB_VEN,       ASUS_ROG_STRIX_RTX2070_A8G_GAMING,      0x2A,   "ASUS ROG STRIX RTX 2070 A8G Gaming 8G"     },
    { NVIDIA_VEN,       NVIDIA_RTX2070_OC_DEV,      ASUS_SUB_VEN,       ASUS_ROG_STRIX_RTX2070_O8G_GAMING,      0x2A,   "ASUS ROG STRIX RTX 2070 O8G Gaming 8G"     },
    { NVIDIA_VEN,       NVIDIA_RTX2070S_DEV,        ASUS_SUB_VEN,       ASUS_ROG_STRIX_RTX2070S_A8G_GAMING,     0x2A,   "ASUS ROG STRIX RTX 2070S A8G Gaming 8G"    },
    { NVIDIA_VEN,       NVIDIA_RTX2070S_DEV,        ASUS_SUB_VEN,       ASUS_ROG_STRIX_RTX2070S_O8G_GAMING,     0x2A,   "ASUS ROG STRIX RTX 2070S O8G Gaming 8G"    },
    { NVIDIA_VEN,       NVIDIA_RTX2080_A_DEV,       ASUS_SUB_VEN,       ASUS_ROG_STRIX_RTX2080_O8G_GAMING,      0x2A,   "ASUS ROG STRIX RTX 2080 O8G Gaming"        },
    { NVIDIA_VEN,       NVIDIA_RTX2080TI_DEV,       ASUS_SUB_VEN,       ASUS_ROG_STRIX_RTX2080TI_O11G_GAMING,   0x2A,   "ASUS ROG STRIX RTX 2080 Ti O11G Gaming"    },
    { AMD_GPU_VEN,      AMD_VEGA10_DEV,             ASUS_SUB_VEN,       ASUS_VEGA64_STRIX,                      0x29,   "ASUS Vega 64 Strix"                        },
    { AMD_GPU_VEN,      AMD_NAVI10_DEV,             ASUS_SUB_VEN,       ASUS_RX5700XT_STRIX_GAMING_OC,          0x2A,   "ASUS RX 5700XT Strix Gaming OC"            },
    { AMD_GPU_VEN,      AMD_POLARIS_DEV,            ASUS_SUB_VEN,       ASUS_RX570_STRIX_O4G_GAMING_OC,         0x29,   "ASUS RX 570 Strix O4G Gaming OC"           },
    { AMD_GPU_VEN,      AMD_POLARIS_DEV,            ASUS_SUB_VEN,       ASUS_RX580_STRIX_GAMING_OC,             0x29,   "ASUS RX 580 Strix Gaming OC"               }
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

void DetectAsusAuraGPUControllers(std::vector<i2c_smbus_interface*> &busses)
{
    AuraGPUController* new_aura_gpu;
    RGBController_AuraGPU* new_controller;

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        for(unsigned int dev_idx = 0; dev_idx < GPU_NUM_DEVICES; dev_idx++)
        {
            if(busses[bus]->pci_vendor           == device_list[dev_idx].pci_vendor           &&
               busses[bus]->pci_device           == device_list[dev_idx].pci_device           &&
               busses[bus]->pci_subsystem_vendor == device_list[dev_idx].pci_subsystem_vendor &&
               busses[bus]->pci_subsystem_device == device_list[dev_idx].pci_subsystem_device)
            {
                if (TestForAsusAuraGPUController(busses[bus], device_list[dev_idx].controller_address))
                {
                    new_aura_gpu         = new AuraGPUController(busses[bus], device_list[dev_idx].controller_address);
                    new_controller       = new RGBController_AuraGPU(new_aura_gpu);
                    new_controller->name = device_list[dev_idx].name;
                    ResourceManager::get()->RegisterRGBController(new_controller);
                }
            }
        }
    }
} /* DetectAsusAuraGPUControllers() */

REGISTER_I2C_DETECTOR("ASUS Aura GPU", DetectAsusAuraGPUControllers);
