#include "Detector.h"
#include "LogManager.h"
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

#define PNY_SUB_VEN_STR     "PNY"
#define PALIT_SUB_VEN_STR   "Palit"
#define GENERIC_SUB_VEN_STR "Generic"

static const gpu_pci_device device_list[] =
{
    { NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         PNY_SUB_VEN,    PNY_RTX_3090_XLR8_REVEL_EPIC_X_SUB_DEV, PNY_RGB,    "PNY XLR8 Revel EPIC-X RTX 3090"    },
    { NVIDIA_VEN,   NVIDIA_RTX3060_DEV,         PALIT_SUB_VEN,  PALIT_RTX3060_SUB_DEV,                  PNY_RGB,    "Palit 3060"                        },
    { NVIDIA_VEN,   NVIDIA_RTX3060_LHR_DEV,     PALIT_SUB_VEN,  PALIT_RTX3060_LHR_SUB_DEV,              PNY_RGB,    "Palit 3060 LHR"                    },
    { NVIDIA_VEN,   NVIDIA_RTX3060TI_DEV,       PALIT_SUB_VEN,  PALIT_RTX3060TI_SUB_DEV,                PNY_RGB,    "Palit 3060Ti"                      },
    { NVIDIA_VEN,   NVIDIA_RTX3060TI_LHR_DEV,   PALIT_SUB_VEN,  PALIT_RTX3060TI_LHR_SUB_DEV,            PNY_RGB,    "Palit 3060Ti"                      },
    { NVIDIA_VEN,   NVIDIA_RTX3070_DEV,         PALIT_SUB_VEN,  PALIT_RTX3070_SUB_DEV,                  PNY_RGB,    "Palit 3070"                        },
    { NVIDIA_VEN,   NVIDIA_RTX3070_LHR_DEV,     PALIT_SUB_VEN,  PALIT_RTX3070_LHR_SUB_DEV,              PNY_RGB,    "Palit 3070 LHR"                    },
    { NVIDIA_VEN,   NVIDIA_RTX3070TI_DEV,       PALIT_SUB_VEN,  PALIT_RTX3070TI_GAMING_PRO_SUB_DEV,     PNY_RGB,    "Palit 3070Ti GamingPro"            },
    { NVIDIA_VEN,   NVIDIA_RTX3070TI_DEV,       PALIT_SUB_VEN,  PALIT_RTX3070TI_SUB_DEV,                PNY_RGB,    "Palit 3070Ti"                      },
    { NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         PALIT_SUB_VEN,  PALIT_RTX3080_SUB_DEV,                  PNY_RGB,    "Palit 3080"                        },
    { NVIDIA_VEN,   NVIDIA_RTX3080_LHR_DEV,     PALIT_SUB_VEN,  PALIT_RTX3080_LHR_SUB_DEV,              PNY_RGB,    "Palit 3080 LHR"                    },
    { NVIDIA_VEN,   NVIDIA_RTX3080TI_DEV,       PALIT_SUB_VEN,  PALIT_RTX3080TI_SUB_DEV,                PNY_RGB,    "Palit 3080Ti"                      },
    { NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         PALIT_SUB_VEN,  PALIT_RTX3090_SUB_DEV,                  PNY_RGB,    "Palit 3090"                        },
    { NVIDIA_VEN,   NVIDIA_RTX2080S_DEV,        NVIDIA_SUB_VEN, NVIDIA_RTX2080S_DEV,                    PNY_RGB,    "NVIDIA RTX2080S"                   },
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
                            LOG_DEBUG(GPU_DETECT_MESSAGE, "PNY GPU", bus, device_list[dev_idx].pci_device, device_list[dev_idx].pci_subsystem_device, device_list[dev_idx].name );
                            PNYGPUController*     new_controller;
                            RGBController_PNYGPU* new_rgbcontroller;

                            new_controller            = new PNYGPUController(busses[bus], 0x49);
                            new_rgbcontroller         = new RGBController_PNYGPU(new_controller);
                            new_rgbcontroller->name   = device_list[dev_idx].name;
                            switch(device_list[dev_idx].pci_subsystem_vendor)
                            {
                            case PNY_SUB_VEN:
                                new_rgbcontroller->vendor = PNY_SUB_VEN_STR;
                                break;

                            case PALIT_SUB_VEN:
                                new_rgbcontroller->vendor = PALIT_SUB_VEN_STR;
                                break;
                            default:
                                new_rgbcontroller->vendor = GENERIC_SUB_VEN_STR;
                                break;
                            }
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
