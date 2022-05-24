#include "Detector.h"
#include "EVGAGPUv3Controller.h"
#include "LogManager.h"
#include "RGBController.h"
#include "RGBController_EVGAGPUv3.h"
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
    { NVIDIA_VEN,   NVIDIA_RTX3060TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3060TI_FTW3_GAMING_SUB_DEV,         "EVGA GeForce RTX 3060TI FTW3 Gaming"                   },
    { NVIDIA_VEN,   NVIDIA_RTX3060TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3060TI_FTW3_ULTRA_KL_SUB_DEV,       "EVGA GeForce RTX 3060TI FTW3 Ultra LHR"                },
    { NVIDIA_VEN,   NVIDIA_RTX3060TI_LHR_DEV,   EVGA_SUB_VEN,   EVGA_RTX3060TI_FTW3_ULTRA_KL_SUB_DEV,       "EVGA GeForce RTX 3060TI FTW3 Ultra LHR"                },
    { NVIDIA_VEN,   NVIDIA_RTX3070_DEV,         EVGA_SUB_VEN,   EVGA_RTX3070_XC3_BLACK_SUB_DEV,             "EVGA GeForce RTX 3070 Black Gaming"                    },
    { NVIDIA_VEN,   NVIDIA_RTX3070_DEV,         EVGA_SUB_VEN,   EVGA_RTX3070_XC3_GAMING_SUB_DEV,            "EVGA GeForce RTX 3070 XC3 Gaming"                      },
    { NVIDIA_VEN,   NVIDIA_RTX3070_DEV,         EVGA_SUB_VEN,   EVGA_RTX3070_XC3_ULTRA_SUB_DEV,             "EVGA GeForce RTX 3070 XC3 Ultra"                       },
    { NVIDIA_VEN,   NVIDIA_RTX3070_LHR_DEV,     EVGA_SUB_VEN,   EVGA_RTX3070_XC3_ULTRA_LHR_SUB_DEV,         "EVGA GeForce RTX 3070 XC3 Ultra LHR"                   },
    { NVIDIA_VEN,   NVIDIA_RTX3070_DEV,         EVGA_SUB_VEN,   EVGA_RTX3070_FTW3_ULTRA_SUB_DEV,            "EVGA GeForce RTX 3070 FTW3 Ultra"                      },
    { NVIDIA_VEN,   NVIDIA_RTX3070_LHR_DEV,     EVGA_SUB_VEN,   EVGA_RTX3070_FTW3_ULTRA_LHR_SUB_DEV,        "EVGA GeForce RTX 3070 FTW3 Ultra LHR"                  },
    { NVIDIA_VEN,   NVIDIA_RTX3070TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3070TI_XC3_GAMING_SUB_DEV,          "EVGA GeForce RTX 3070Ti XC3 Gaming"                    },
    { NVIDIA_VEN,   NVIDIA_RTX3070TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3070TI_XC3_ULTRA_SUB_DEV,           "EVGA GeForce RTX 3070Ti XC3 Ultra"                     },
    { NVIDIA_VEN,   NVIDIA_RTX3070TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3070TI_FTW3_ULTRA_SUB_DEV,          "EVGA GeForce RTX 3070Ti FTW3 Ultra"                    },
    { NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         EVGA_SUB_VEN,   EVGA_RTX3080_XC3_BLACK_SUB_DEV,             "EVGA GeForce RTX 3080 XC3 Black"                       },
    { NVIDIA_VEN,   NVIDIA_RTX3080_LHR_DEV,     EVGA_SUB_VEN,   EVGA_RTX3080_XC3_BLACK_LHR_SUB_DEV,         "EVGA GeForce RTX 3080 XC3 Black LHR"                   },
    { NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         EVGA_SUB_VEN,   EVGA_RTX3080_XC3_GAMING_SUB_DEV,            "EVGA GeForce RTX 3080 XC3 Gaming"                      },
    { NVIDIA_VEN,   NVIDIA_RTX3080_LHR_DEV,     EVGA_SUB_VEN,   EVGA_RTX3080_XC3_GAMING_LHR_SUB_DEV,        "EVGA GeForce RTX 3080 XC3 Gaming LHR"                  },
    { NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         EVGA_SUB_VEN,   EVGA_RTX3080_XC3_ULTRA_SUB_DEV,             "EVGA GeForce RTX 3080 XC3 Ultra"                       },
    { NVIDIA_VEN,   NVIDIA_RTX3080_LHR_DEV,     EVGA_SUB_VEN,   EVGA_RTX3080_XC3_ULTRA_LHR_SUB_DEV,         "EVGA GeForce RTX 3080 XC3 Ultra LHR"                   },
    { NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         EVGA_SUB_VEN,   EVGA_RTX3080_XC3_ULTRA_HYBRID_SUB_DEV,      "EVGA GeForce RTX 3080 XC3 Ultra Hybrid"                },
    { NVIDIA_VEN,   NVIDIA_RTX3080_LHR_DEV,     EVGA_SUB_VEN,   EVGA_RTX3080_XC3_ULTRA_HYBRID_LHR_SUB_DEV,  "EVGA GeForce RTX 3080 XC3 Ultra Hybrid LHR"            },
    { NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         EVGA_SUB_VEN,   EVGA_RTX3080_XC3_ULTRA_HC_SUB_DEV,          "EVGA GeForce RTX 3080 XC3 Ultra Hydro Copper"          },
    { NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         EVGA_SUB_VEN,   EVGA_RTX3080_FTW3_GAMING_SUB_DEV,           "EVGA GeForce RTX 3080 FTW3 Gaming"                     },
    { NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         EVGA_SUB_VEN,   EVGA_RTX3080_FTW3_ULTRA_SUB_DEV,            "EVGA GeForce RTX 3080 FTW3 Ultra"                      },
    { NVIDIA_VEN,   NVIDIA_RTX3080_12G_LHR_DEV, EVGA_SUB_VEN,   EVGA_RTX3080_FTW3_ULTRA_12G_SUB_DEV,        "EVGA GeForce RTX 3080 FTW3 Ultra 12GB"                 },
    { NVIDIA_VEN,   NVIDIA_RTX3080_LHR_DEV,     EVGA_SUB_VEN,   EVGA_RTX3080_FTW3_ULTRA_LHR_SUB_DEV,        "EVGA GeForce RTX 3080 FTW3 Ultra LHR"                  },
    { NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         EVGA_SUB_VEN,   EVGA_RTX3080_FTW3_ULTRA_HYBRID_SUB_DEV,     "EVGA GeForce RTX 3080 FTW3 Ultra Hybrid"               },
    { NVIDIA_VEN,   NVIDIA_RTX3080_LHR_DEV,     EVGA_SUB_VEN,   EVGA_RTX3080_FTW3_ULTRA_HYBRID_LHR_SUB_DEV, "EVGA GeForce RTX 3080 FTW3 Ultra Hybrid LHR"           },
    { NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         EVGA_SUB_VEN,   EVGA_RTX3080_FTW3_ULTRA_HC_SUB_DEV,         "EVGA GeForce RTX 3080 FTW3 Ultra Hydro Copper"         },
    { NVIDIA_VEN,   NVIDIA_RTX3080TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3080TI_XC3_GAMING_SUB_DEV,          "EVGA GeForce RTX 3080Ti XC3 Gaming"                    },
    { NVIDIA_VEN,   NVIDIA_RTX3080TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3080TI_XC3_ULTRA_GAMING_SUB_DEV,    "EVGA GeForce RTX 3080Ti XC3 Ultra Gaming"              },
    { NVIDIA_VEN,   NVIDIA_RTX3080TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3080TI_XC3_GAMING_HYBRID_SUB_DEV,   "EVGA GeForce RTX 3080Ti XC3 Gaming Hybrid"             },
    { NVIDIA_VEN,   NVIDIA_RTX3080TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3080TI_XC3_GAMING_HC_SUB_DEV,       "EVGA GeForce RTX 3080Ti XC3 Gaming Hydro Copper"       },
    { NVIDIA_VEN,   NVIDIA_RTX3080TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3080TI_FTW3_ULTRA_SUB_DEV,          "EVGA GeForce RTX 3080Ti FTW3 Ultra"                    },
    { NVIDIA_VEN,   NVIDIA_RTX3080TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3080TI_FTW3_ULTRA_HYBRID_SUB_DEV,   "EVGA GeForce RTX 3080Ti FTW3 Ultra Hybrid"             },
    { NVIDIA_VEN,   NVIDIA_RTX3080TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3080TI_FTW3_ULTRA_HC_SUB_DEV,       "EVGA GeForce RTX 3080Ti FTW3 Ultra Hydro Copper"       },
    { NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_XC3_BLACK_SUB_DEV,             "EVGA GeForce RTX 3090 XC3 Black"                       },
    { NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_XC3_GAMING_SUB_DEV,            "EVGA GeForce RTX 3090 XC3 Gaming"                      },
    { NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_XC3_ULTRA_SUB_DEV,             "EVGA GeForce RTX 3090 XC3 Ultra"                       },
    { NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_XC3_ULTRA_HYBRID_SUB_DEV,      "EVGA GeForce RTX 3090 XC3 Ultra Hybrid"                },
    { NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_XC3_ULTRA_HC_SUB_DEV,          "EVGA GeForce RTX 3090 XC3 Ultra Hydro Copper"          },
    { NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_FTW3_ULTRA_SUB_DEV,            "EVGA GeForce RTX 3090 FTW3 Ultra"                      },
    { NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_FTW3_ULTRA_V2_SUB_DEV,         "EVGA GeForce RTX 3090 FTW3 Ultra v2"                   },
    { NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_FTW3_ULTRA_V3_SUB_DEV,         "EVGA GeForce RTX 3090 FTW3 Ultra v3"                   },
    { NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_FTW3_ULTRA_HYBRID_SUB_DEV,     "EVGA GeForce RTX 3090 FTW3 Ultra Hybrid"               },
    { NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_FTW3_ULTRA_HC_SUB_DEV,         "EVGA GeForce RTX 3090 FTW3 Ultra Hydro Copper"         },
    { NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_KINGPIN_HYBRID_SUB_DEV,        "EVGA GeForce RTX 3090 K|NGP|N Hybrid"                  },
    { NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_KINGPIN_HC_SUB_DEV,            "EVGA GeForce RTX 3090 K|NGP|N Hydro Copper"            },
    { NVIDIA_VEN,   NVIDIA_RTX3090TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3090TI_FTW3_GAMING_SUB_DEV,         "EVGA GeForce RTX 3090Ti FTW3 Gaming"                   },
};
/******************************************************************************************\
*                                                                                          *
*   DetectEVGAAmpereGPUControllers                                                         *
*                                                                                          *
*       Detect EVGA Ampere GPU controllers on the enumerated I2C busses at address 0x2D.   *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where EVGA GPU device is connected        *
*           dev - I2C address of EVGA GPU device                                           *
*                                                                                          *
\******************************************************************************************/

void DetectEVGAAmpereGPUControllers(std::vector<i2c_smbus_interface*>& busses)
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
                LOG_DEBUG(GPU_DETECT_MESSAGE, EVGAGPUV3_CONTROLLER_NAME, bus, device_list[dev_idx].pci_device, device_list[dev_idx].pci_subsystem_device, device_list[dev_idx].name );
                EVGAGPUv3Controller*     new_controller;
                RGBController_EVGAGPUv3* new_rgbcontroller;

                new_controller = new EVGAGPUv3Controller(busses[bus], 0x2D);
                new_controller-> evgaGPUName = device_list[dev_idx].name; // Pass name of the card into the controller for logging.

                if(new_controller-> ReadFWVersion() != "")
                {
                    new_rgbcontroller = new RGBController_EVGAGPUv3(new_controller);
                    new_rgbcontroller->name = device_list[dev_idx].name;
                    ResourceManager::get()->RegisterRGBController(new_rgbcontroller);
                }
                else
                {
                    LOG_INFO("[%s] Failed to get a valid FW version, does the i2c interface support `i2c_smbus_read_i2c_block_data`?", new_controller-> evgaGPUName);
                    delete new_controller;
                }
            }
        }
    }
}   /* DetectEVGAAmpereGPUControllers() */

REGISTER_I2C_DETECTOR("EVGA Ampere GPU", DetectEVGAAmpereGPUControllers);
