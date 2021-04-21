#include "Detector.h"
#include "EVGAGPUv1Controller.h"
#include "EVGAGPUv2Controller.h"
#include "RGBController.h"
#include "RGBController_EVGAGPUv1.h"
#include "RGBController_EVGAGPUv2.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

enum
{
    EVGA_RGB_V1,
    EVGA_RGB_V2,
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
    { NVIDIA_VEN,   NVIDIA_GTX1070_DEV,       EVGA_SUB_VEN,   EVGA_GTX1070_FTW_SUB_DEV,               EVGA_RGB_V1,    "EVGA GeForce GTX 1070 FTW"               },
    { NVIDIA_VEN,   NVIDIA_GTX1070TI_DEV,     EVGA_SUB_VEN,   EVGA_GTX1070TI_FTW2_SUB_DEV,            EVGA_RGB_V1,    "EVGA GeForce GTX 1070 Ti FTW2"           },
    { NVIDIA_VEN,   NVIDIA_GTX1080_DEV,       EVGA_SUB_VEN,   EVGA_GTX1080_FTW_SUB_DEV,               EVGA_RGB_V1,    "EVGA GeForce GTX 1080 FTW"               },
    { NVIDIA_VEN,   NVIDIA_RTX2070_OC_DEV,    EVGA_SUB_VEN,   EVGA_RTX2070_XC_GAMING_SUB_DEV,         EVGA_RGB_V2,    "EVGA GeForce RTX 2070 XC Gaming"         },
    { NVIDIA_VEN,   NVIDIA_RTX2070_OC_DEV,    EVGA_SUB_VEN,   EVGA_RTX2070_XC_OC_SUB_DEV,             EVGA_RGB_V2,    "EVGA GeForce RTX 2070 XC OC"             },
    { NVIDIA_VEN,   NVIDIA_RTX2070S_DEV,      EVGA_SUB_VEN,   EVGA_RTX2070S_XC_ULTRA_SUB_DEV,         EVGA_RGB_V2,    "EVGA GeForce RTX 2070 SUPER XC Ultra"    },
    { NVIDIA_VEN,   NVIDIA_RTX2070S_DEV,      EVGA_SUB_VEN,   EVGA_RTX2070S_XC_ULTRA_PLUS_SUB_DEV,    EVGA_RGB_V2,    "EVGA GeForce RTX 2070 SUPER XC Ultra+"   },
    { NVIDIA_VEN,   NVIDIA_RTX2080_A_DEV,     EVGA_SUB_VEN,   EVGA_RTX2080_XC_BLACK_SUB_DEV,          EVGA_RGB_V2,    "EVGA GeForce RTX 2080 XC Black"          },
    { NVIDIA_VEN,   NVIDIA_RTX2080_A_DEV,     EVGA_SUB_VEN,   EVGA_RTX2080_XC_GAMING_SUB_DEV,         EVGA_RGB_V2,    "EVGA GeForce RTX 2080 XC Gaming"         },
    { NVIDIA_VEN,   NVIDIA_RTX2080_A_DEV,     EVGA_SUB_VEN,   EVGA_RTX2080_XC_ULTRA_GAMING_SUB_DEV,   EVGA_RGB_V2,    "EVGA GeForce RTX 2080 XC Ultra Gaming"   },
    { NVIDIA_VEN,   NVIDIA_RTX2080TI_DEV,     EVGA_SUB_VEN,   EVGA_RTX2080TI_XC_ULTRA_GAMING_SUB_DEV, EVGA_RGB_V2,    "EVGA GeForce RTX 2080Ti XC Ultra"        },
    { NVIDIA_VEN,   NVIDIA_RTX2080TI_DEV,     EVGA_SUB_VEN,   EVGA_RTX2080TI_FTW3_ULTRA_SUB_DEV     , EVGA_RGB_V2,    "EVGA GeForce RTX 2080Ti FTW3 Ultra"      },

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

void DetectEVGAGPUControllers(std::vector<i2c_smbus_interface*>& busses)
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
                switch(device_list[dev_idx].gpu_rgb_version)
                {
                    case EVGA_RGB_V1:
                        {
                            EVGAGPUv1Controller*     new_controller;
                            RGBController_EVGAGPUv1* new_rgbcontroller;

                            new_controller          = new EVGAGPUv1Controller(busses[bus], 0x49);
                            new_rgbcontroller       = new RGBController_EVGAGPUv1(new_controller);
                            new_rgbcontroller->name = device_list[dev_idx].name;
                            ResourceManager::get()->RegisterRGBController(new_rgbcontroller);
                        }
                        break;

                    case EVGA_RGB_V2:
                        {
                            EVGAGPUv2Controller*     new_controller;
                            RGBController_EVGAGPUv2* new_rgbcontroller;

                            new_controller          = new EVGAGPUv2Controller(busses[bus], 0x49);
                            new_rgbcontroller       = new RGBController_EVGAGPUv2(new_controller);
                            new_rgbcontroller->name = device_list[dev_idx].name;
                            ResourceManager::get()->RegisterRGBController(new_rgbcontroller);
                        }
                        break;
                }
            }
        }
    }
}   /* DetectEVGAGPUControllers() */

REGISTER_I2C_DETECTOR("EVGA GPU", DetectEVGAGPUControllers);
