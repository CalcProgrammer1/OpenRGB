#include "Detector.h"
#include "EVGAGPUCommon.h"
#include "EVGAGP102Controller.h"
#include "EVGAGPUv1Controller.h"
#include "EVGAGPUv2Controller.h"
#include "EVGAGPUv3Controller.h"
#include "LogManager.h"
#include "RGBController.h"
#include "RGBController_EVGAGP102.h"
#include "RGBController_EVGAGPUv1.h"
#include "RGBController_EVGAGPUv2.h"
#include "RGBController_EVGAGPUv3.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

enum
{
    EVGA_RGB_GP102,
    EVGA_RGB_V1,
    EVGA_RGB_V2,
    EVGA_RGB_V3,
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
    { NVIDIA_VEN,   NVIDIA_GTX1070_DEV,         EVGA_SUB_VEN,   EVGA_GTX1070_FTW_DT_GAMING_SUB_DEV,         EVGA_RGB_V1,    "EVGA GeForce GTX 1070 FTW DT Gaming"                   },
    { NVIDIA_VEN,   NVIDIA_GTX1070_DEV,         EVGA_SUB_VEN,   EVGA_GTX1070_FTW_SUB_DEV,                   EVGA_RGB_V1,    "EVGA GeForce GTX 1070 FTW"                             },
    { NVIDIA_VEN,   NVIDIA_GTX1070_DEV,         EVGA_SUB_VEN,   EVGA_GTX1070_FTW_HYBRID_SUB_DEV,            EVGA_RGB_V1,    "EVGA GeForce GTX 1070 FTW HYBRID"                      },
    { NVIDIA_VEN,   NVIDIA_GTX1070TI_DEV,       EVGA_SUB_VEN,   EVGA_GTX1070TI_FTW2_SUB_DEV,                EVGA_RGB_V1,    "EVGA GeForce GTX 1070 Ti FTW2"                         },
    { NVIDIA_VEN,   NVIDIA_GTX1080_DEV,         EVGA_SUB_VEN,   EVGA_GTX1080_FTW_SUB_DEV,                   EVGA_RGB_V1,    "EVGA GeForce GTX 1080 FTW"                             },
    { NVIDIA_VEN,   NVIDIA_GTX1080_DEV,         EVGA_SUB_VEN,   EVGA_GTX1080_FTW2_SUB_DEV,                  EVGA_RGB_GP102, "EVGA GeForce GTX 1080 FTW2"                            },
    { NVIDIA_VEN,   NVIDIA_GTX1080TI_DEV,       EVGA_SUB_VEN,   EVGA_GTX1080TI_FTW3_SUB_DEV,                EVGA_RGB_GP102, "EVGA GTX 1080 Ti FTW3"                                 },
    { NVIDIA_VEN,   NVIDIA_GTX1080TI_DEV,       EVGA_SUB_VEN,   EVGA_GTX1080TI_FTW3_HYBRID_SUB_DEV,         EVGA_RGB_GP102, "EVGA 1080Ti FTW3 Hybrid"                               },
    { NVIDIA_VEN,   NVIDIA_GTX1080TI_DEV,       EVGA_SUB_VEN,   EVGA_GTX1080TI_KINGPIN_SUB_DEV,             EVGA_RGB_GP102, "EVGA GTX 1080 Ti K|NGP|N"                              },
    { NVIDIA_VEN,   NVIDIA_RTX2070_OC_DEV,      EVGA_SUB_VEN,   EVGA_RTX2070_XC_GAMING_SUB_DEV,             EVGA_RGB_V2,    "EVGA GeForce RTX 2070 XC Gaming"                       },
    { NVIDIA_VEN,   NVIDIA_RTX2070_OC_DEV,      EVGA_SUB_VEN,   EVGA_RTX2070_XC_OC_SUB_DEV,                 EVGA_RGB_V2,    "EVGA GeForce RTX 2070 XC OC"                           },
    { NVIDIA_VEN,   NVIDIA_RTX2070S_OC_DEV,     EVGA_SUB_VEN,   EVGA_RTX2070S_BLACK_SUB_DEV,                EVGA_RGB_V2,    "EVGA GeForce RTX 2070 SUPER Black"                     },
    { NVIDIA_VEN,   NVIDIA_RTX2070S_OC_DEV,     EVGA_SUB_VEN,   EVGA_RTX2070S_XC_ULTRA_SUB_DEV,             EVGA_RGB_V2,    "EVGA GeForce RTX 2070 SUPER XC Ultra"                  },
    { NVIDIA_VEN,   NVIDIA_RTX2070S_OC_DEV,     EVGA_SUB_VEN,   EVGA_RTX2070S_XC_ULTRA_PLUS_SUB_DEV,        EVGA_RGB_V2,    "EVGA GeForce RTX 2070 SUPER XC Ultra+"                 },
    { NVIDIA_VEN,   NVIDIA_RTX2080_DEV,         EVGA_SUB_VEN,   EVGA_RTX2080_BLACK_SUB_DEV,                 EVGA_RGB_V2,    "EVGA GeForce RTX 2080 Black"                           },
    { NVIDIA_VEN,   NVIDIA_RTX2080_A_DEV,       EVGA_SUB_VEN,   EVGA_RTX2080_XC_BLACK_SUB_DEV,              EVGA_RGB_V2,    "EVGA GeForce RTX 2080 XC Black"                        },
    { NVIDIA_VEN,   NVIDIA_RTX2080_A_DEV,       EVGA_SUB_VEN,   EVGA_RTX2080_XC_GAMING_SUB_DEV,             EVGA_RGB_V2,    "EVGA GeForce RTX 2080 XC Gaming"                       },
    { NVIDIA_VEN,   NVIDIA_RTX2080_A_DEV,       EVGA_SUB_VEN,   EVGA_RTX2080_XC_ULTRA_GAMING_SUB_DEV,       EVGA_RGB_V2,    "EVGA GeForce RTX 2080 XC Ultra Gaming"                 },
    { NVIDIA_VEN,   NVIDIA_RTX2080S_DEV,        EVGA_SUB_VEN,   EVGA_RTX2080S_XC_GAMING_SUB_DEV,            EVGA_RGB_V2,    "EVGA GeForce RTX 2080 SUPER XC Gaming"                 },
    { NVIDIA_VEN,   NVIDIA_RTX2080S_DEV,        EVGA_SUB_VEN,   EVGA_RTX2080S_FTW3_ULTRA_SUB_DEV,           EVGA_RGB_V2,    "EVGA GeForce RTX 2080 SUPER FTW3 Ultra"                },
    { NVIDIA_VEN,   NVIDIA_RTX2080S_DEV,        EVGA_SUB_VEN,   EVGA_RTX2080S_FTW3_ULTRA_HC_SUB_DEV,        EVGA_RGB_V2,    "EVGA GeForce RTX 2080 SUPER FTW3 Ultra Hydro Copper"   },
    { NVIDIA_VEN,   NVIDIA_RTX2080TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX2080TI_BLACK_SUB_DEV,               EVGA_RGB_V2,    "EVGA GeForce RTX 2080Ti Black"                         },
    { NVIDIA_VEN,   NVIDIA_RTX2080TI_A_DEV,     EVGA_SUB_VEN,   EVGA_RTX2080TI_XC_ULTRA_GAMING_SUB_DEV,     EVGA_RGB_V2,    "EVGA GeForce RTX 2080Ti XC Ultra"                      },
    { NVIDIA_VEN,   NVIDIA_RTX2080TI_A_DEV,     EVGA_SUB_VEN,   EVGA_RTX2080TI_XC_HYBRID_GAMING_SUB_DEV,    EVGA_RGB_V2,    "EVGA GeForce RTX 2080Ti XC HYBRID GAMING"              },
    { NVIDIA_VEN,   NVIDIA_RTX2080TI_A_DEV,     EVGA_SUB_VEN,   EVGA_RTX2080TI_FTW3_ULTRA_SUB_DEV,          EVGA_RGB_V2,    "EVGA GeForce RTX 2080Ti FTW3 Ultra"                    },
    { NVIDIA_VEN,   NVIDIA_RTX3060TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3060TI_FTW3_GAMING_SUB_DEV,         EVGA_RGB_V3,    "EVGA GeForce RTX 3060TI FTW3 Gaming"                   },
    { NVIDIA_VEN,   NVIDIA_RTX3060TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3060TI_FTW3_ULTRA_KL_SUB_DEV,       EVGA_RGB_V3,    "EVGA GeForce RTX 3060TI FTW3 Ultra LHR"                },
    { NVIDIA_VEN,   NVIDIA_RTX3060TI_LHR_DEV,   EVGA_SUB_VEN,   EVGA_RTX3060TI_FTW3_ULTRA_KL_SUB_DEV,       EVGA_RGB_V3,    "EVGA GeForce RTX 3060TI FTW3 Ultra LHR"                },
    { NVIDIA_VEN,   NVIDIA_RTX3070_DEV,         EVGA_SUB_VEN,   EVGA_RTX3070_XC3_BLACK_SUB_DEV,             EVGA_RGB_V3,    "EVGA GeForce RTX 3070 Black Gaming"                    },
    { NVIDIA_VEN,   NVIDIA_RTX3070_DEV,         EVGA_SUB_VEN,   EVGA_RTX3070_XC3_GAMING_SUB_DEV,            EVGA_RGB_V3,    "EVGA GeForce RTX 3070 XC3 Gaming"                      },
    { NVIDIA_VEN,   NVIDIA_RTX3070_DEV,         EVGA_SUB_VEN,   EVGA_RTX3070_XC3_ULTRA_SUB_DEV,             EVGA_RGB_V3,    "EVGA GeForce RTX 3070 XC3 Ultra"                       },
    { NVIDIA_VEN,   NVIDIA_RTX3070_LHR_DEV,     EVGA_SUB_VEN,   EVGA_RTX3070_XC3_ULTRA_LHR_SUB_DEV,         EVGA_RGB_V3,    "EVGA GeForce RTX 3070 XC3 Ultra LHR"                   },
    { NVIDIA_VEN,   NVIDIA_RTX3070_DEV,         EVGA_SUB_VEN,   EVGA_RTX3070_FTW3_ULTRA_SUB_DEV,            EVGA_RGB_V3,    "EVGA GeForce RTX 3070 FTW3 Ultra"                      },
    { NVIDIA_VEN,   NVIDIA_RTX3070_LHR_DEV,     EVGA_SUB_VEN,   EVGA_RTX3070_FTW3_ULTRA_LHR_SUB_DEV,        EVGA_RGB_V3,    "EVGA GeForce RTX 3070 FTW3 Ultra LHR"                  },
    { NVIDIA_VEN,   NVIDIA_RTX3070TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3070TI_XC3_GAMING_SUB_DEV,          EVGA_RGB_V3,    "EVGA GeForce RTX 3070Ti XC3 Gaming"                    },
    { NVIDIA_VEN,   NVIDIA_RTX3070TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3070TI_XC3_ULTRA_SUB_DEV,           EVGA_RGB_V3,    "EVGA GeForce RTX 3070Ti XC3 Ultra"                     },
    { NVIDIA_VEN,   NVIDIA_RTX3070TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3070TI_FTW3_ULTRA_SUB_DEV,          EVGA_RGB_V3,    "EVGA GeForce RTX 3070Ti FTW3 Ultra"                    },
    { NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         EVGA_SUB_VEN,   EVGA_RTX3080_XC3_BLACK_SUB_DEV,             EVGA_RGB_V3,    "EVGA GeForce RTX 3080 XC3 Black"                       },
    { NVIDIA_VEN,   NVIDIA_RTX3080_LHR_DEV,     EVGA_SUB_VEN,   EVGA_RTX3080_XC3_BLACK_LHR_SUB_DEV,         EVGA_RGB_V3,    "EVGA GeForce RTX 3080 XC3 Black LHR"                   },
    { NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         EVGA_SUB_VEN,   EVGA_RTX3080_XC3_GAMING_SUB_DEV,            EVGA_RGB_V3,    "EVGA GeForce RTX 3080 XC3 Gaming"                      },
    { NVIDIA_VEN,   NVIDIA_RTX3080_LHR_DEV,     EVGA_SUB_VEN,   EVGA_RTX3080_XC3_GAMING_LHR_SUB_DEV,        EVGA_RGB_V3,    "EVGA GeForce RTX 3080 XC3 Gaming LHR"                  },
    { NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         EVGA_SUB_VEN,   EVGA_RTX3080_XC3_ULTRA_SUB_DEV,             EVGA_RGB_V3,    "EVGA GeForce RTX 3080 XC3 Ultra"                       },
    { NVIDIA_VEN,   NVIDIA_RTX3080_LHR_DEV,     EVGA_SUB_VEN,   EVGA_RTX3080_XC3_ULTRA_LHR_SUB_DEV,         EVGA_RGB_V3,    "EVGA GeForce RTX 3080 XC3 Ultra LHR"                   },
    { NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         EVGA_SUB_VEN,   EVGA_RTX3080_XC3_ULTRA_HYBRID_SUB_DEV,      EVGA_RGB_V3,    "EVGA GeForce RTX 3080 XC3 Ultra Hybrid"                },
    { NVIDIA_VEN,   NVIDIA_RTX3080_LHR_DEV,     EVGA_SUB_VEN,   EVGA_RTX3080_XC3_ULTRA_HYBRID_LHR_SUB_DEV,  EVGA_RGB_V3,    "EVGA GeForce RTX 3080 XC3 Ultra Hybrid LHR"            },
    { NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         EVGA_SUB_VEN,   EVGA_RTX3080_XC3_ULTRA_HC_SUB_DEV,          EVGA_RGB_V3,    "EVGA GeForce RTX 3080 XC3 Ultra Hydro Copper"          },
    { NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         EVGA_SUB_VEN,   EVGA_RTX3080_FTW3_GAMING_SUB_DEV,           EVGA_RGB_V3,    "EVGA GeForce RTX 3080 FTW3 Gaming"                     },
    { NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         EVGA_SUB_VEN,   EVGA_RTX3080_FTW3_ULTRA_SUB_DEV,            EVGA_RGB_V3,    "EVGA GeForce RTX 3080 FTW3 Ultra"                      },
    { NVIDIA_VEN,   NVIDIA_RTX3080_LHR_DEV,     EVGA_SUB_VEN,   EVGA_RTX3080_FTW3_ULTRA_LHR_SUB_DEV,        EVGA_RGB_V3,    "EVGA GeForce RTX 3080 FTW3 Ultra LHR"                  },
    { NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         EVGA_SUB_VEN,   EVGA_RTX3080_FTW3_ULTRA_HYBRID_SUB_DEV,     EVGA_RGB_V3,    "EVGA GeForce RTX 3080 FTW3 Ultra Hybrid"               },
    { NVIDIA_VEN,   NVIDIA_RTX3080_LHR_DEV,     EVGA_SUB_VEN,   EVGA_RTX3080_FTW3_ULTRA_HYBRID_LHR_SUB_DEV, EVGA_RGB_V3,    "EVGA GeForce RTX 3080 FTW3 Ultra Hybrid LHR"           },
    { NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         EVGA_SUB_VEN,   EVGA_RTX3080_FTW3_ULTRA_HC_SUB_DEV,         EVGA_RGB_V3,    "EVGA GeForce RTX 3080 FTW3 Ultra Hydro Copper"         },
    { NVIDIA_VEN,   NVIDIA_RTX3080TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3080TI_XC3_GAMING_SUB_DEV,          EVGA_RGB_V3,    "EVGA GeForce RTX 3080Ti XC3 Gaming"                    },
    { NVIDIA_VEN,   NVIDIA_RTX3080TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3080TI_XC3_GAMING_HYBRID_SUB_DEV,   EVGA_RGB_V3,    "EVGA GeForce RTX 3080Ti XC3 Gaming Hybrid"             },
    { NVIDIA_VEN,   NVIDIA_RTX3080TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3080TI_XC3_GAMING_HC_SUB_DEV,       EVGA_RGB_V3,    "EVGA GeForce RTX 3080Ti XC3 Gaming Hydro Copper"       },
    { NVIDIA_VEN,   NVIDIA_RTX3080TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3080TI_FTW3_ULTRA_SUB_DEV,          EVGA_RGB_V3,    "EVGA GeForce RTX 3080Ti FTW3 Ultra"                    },
    { NVIDIA_VEN,   NVIDIA_RTX3080TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3080TI_FTW3_ULTRA_HYBRID_SUB_DEV,   EVGA_RGB_V3,    "EVGA GeForce RTX 3080Ti FTW3 Ultra Hybrid"             },
    { NVIDIA_VEN,   NVIDIA_RTX3080TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3080TI_FTW3_ULTRA_HC_SUB_DEV,       EVGA_RGB_V3,    "EVGA GeForce RTX 3080Ti FTW3 Ultra Hydro Copper"       },
    { NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_XC3_BLACK_SUB_DEV,             EVGA_RGB_V3,    "EVGA GeForce RTX 3090 XC3 Black"                       },
    { NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_XC3_GAMING_SUB_DEV,            EVGA_RGB_V3,    "EVGA GeForce RTX 3090 XC3 Gaming"                      },
    { NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_XC3_ULTRA_SUB_DEV,             EVGA_RGB_V3,    "EVGA GeForce RTX 3090 XC3 Ultra"                       },
    { NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_XC3_ULTRA_HYBRID_SUB_DEV,      EVGA_RGB_V3,    "EVGA GeForce RTX 3090 XC3 Ultra Hybrid"                },
    { NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_XC3_ULTRA_HC_SUB_DEV,          EVGA_RGB_V3,    "EVGA GeForce RTX 3090 XC3 Ultra Hydro Copper"          },
    { NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_FTW3_ULTRA_SUB_DEV,            EVGA_RGB_V3,    "EVGA GeForce RTX 3090 FTW3 Ultra"                      },
    { NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_FTW3_ULTRA_V2_SUB_DEV,         EVGA_RGB_V3,    "EVGA GeForce RTX 3090 FTW3 Ultra v2"                   },
    { NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_FTW3_ULTRA_HYBRID_SUB_DEV,     EVGA_RGB_V3,    "EVGA GeForce RTX 3090 FTW3 Ultra Hybrid"               },
    { NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_FTW3_ULTRA_HC_SUB_DEV,         EVGA_RGB_V3,    "EVGA GeForce RTX 3090 FTW3 Ultra Hydro Copper"         },
    { NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_KINGPIN_HYBRID_SUB_DEV,        EVGA_RGB_V3,    "EVGA GeForce RTX 3090 K|NGP|N Hybrid"                  },
    { NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_KINGPIN_HC_SUB_DEV,            EVGA_RGB_V3,    "EVGA GeForce RTX 3090 K|NGP|N Hydro Copper"            },
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
                            LOG_DEBUG(GPU_DETECT_MESSAGE, EVGAGPUV1_CONTROLLER_NAME, bus, device_list[dev_idx].pci_device, device_list[dev_idx].pci_subsystem_device, device_list[dev_idx].name );
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
                            LOG_DEBUG(GPU_DETECT_MESSAGE, EVGAGPUV2_CONTROLLER_NAME, bus, device_list[dev_idx].pci_device, device_list[dev_idx].pci_subsystem_device, device_list[dev_idx].name );
                            EVGAGPUv2Controller*     new_controller;
                            RGBController_EVGAGPUv2* new_rgbcontroller;

                            new_controller          = new EVGAGPUv2Controller(busses[bus], 0x49);
                            new_rgbcontroller       = new RGBController_EVGAGPUv2(new_controller);
                            new_rgbcontroller->name = device_list[dev_idx].name;
                            ResourceManager::get()->RegisterRGBController(new_rgbcontroller);
                        }
                        break;

                    case EVGA_RGB_V3:
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
                        break;
                    case EVGA_RGB_GP102:
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
                        break;
                }
            }
        }
    }
}   /* DetectEVGAGPUControllers() */

REGISTER_I2C_DETECTOR("EVGA GPU", DetectEVGAGPUControllers);
