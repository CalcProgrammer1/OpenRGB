/*---------------------------------------------------------*\
| NVIDIAIlluminationControllerDetect_Windows_Linux.cpp      |
|                                                           |
|   Detector for NVIDIA Illumination GPU                    |
|                                                           |
|   Carter Miller (GingerRunner)                04 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "LogManager.h"
#include "RGBController_NVIDIAIllumination_Windows_Linux.h"
#include "pci_ids.h"

enum
{
    NVIDIA_ILLUMINATION_V1
};

typedef struct
{
    int             pci_vendor;
    int             pci_device;
    int             pci_subsystem_vendor;
    int             pci_subsystem_device;
    int             gpu_rgb_version;
    bool            treats_rgbw_as_rgb;
    const char *    name;
} nv_gpu_pci_device;


#define GPU_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[ 0 ]))

/*-----------------------------------------------------------------------------------------------------*\
| Certain devices seem to ignore the white value entirely, despite the zone being reported back by the  |
| API as RGBW, so this boolean is passed at detection time via constructor inform the controller logic. |
\*-----------------------------------------------------------------------------------------------------*/
#define TREATS_RGBW_AS_RGB  true
#define TREATS_RGBW_AS_RGBW false

static const nv_gpu_pci_device device_list[] =
{
    {NVIDIA_VEN,    NVIDIA_RTX2060_TU104_DEV,       NVIDIA_SUB_VEN,   NVIDIA_RTX2060_TU104_DEV,                       NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "Palit GeForce RTX 2060"                      },
    {NVIDIA_VEN,    NVIDIA_RTX2060_TU106_DEV,       NVIDIA_SUB_VEN,   NVIDIA_RTX2060_FE_SUB_DEV,                      NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "NVIDIA GeForce RTX 2060 FE"                  },
    {NVIDIA_VEN,    NVIDIA_RTX2060S_OC_DEV,         NVIDIA_SUB_VEN,   NVIDIA_RTX2060S_FE_SUB_DEV,                     NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "NVIDIA GeForce RTX 2060 SUPER FE"            },
    {NVIDIA_VEN,    NVIDIA_RTX2070_OC_DEV,          NVIDIA_SUB_VEN,   NVIDIA_RTX2070_FE_SUB_DEV,                      NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "NVIDIA GeForce RTX 2070 FE"                  },
    {NVIDIA_VEN,    NVIDIA_RTX2070S_DEV,            NVIDIA_SUB_VEN,   NVIDIA_RTX2070_FE_SUPER_SUB_DEV,                NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "NVIDIA GeForce RTX 2070 SUPER FE"            },
    {NVIDIA_VEN,    NVIDIA_RTX2080_A_DEV,           NVIDIA_SUB_VEN,   NVIDIA_RTX2080_FE_SUB_DEV,                      NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "NVIDIA GeForce RTX 2080 FE"                  },
    {NVIDIA_VEN,    NVIDIA_RTX2080S_DEV,            NVIDIA_SUB_VEN,   NVIDIA_RTX2080S_FE_SUB_DEV,                     NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "NVIDIA GeForce RTX 2080 SUPER FE"            },
    {NVIDIA_VEN,    NVIDIA_RTX2080TI_A_DEV,         NVIDIA_SUB_VEN,   NVIDIA_RTX2080TI_FE_SUB_DEV,                    NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "NVIDIA GeForce RTX 2080 Ti FE"               },
    {NVIDIA_VEN,    NVIDIA_TITANRTX_DEV,            NVIDIA_SUB_VEN,   NVIDIA_TITANRTX_FE_SUB_DEV,                     NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "NVIDIA TITAN RTX"                            },
    {NVIDIA_VEN,    NVIDIA_RTX3050_DEV,             NVIDIA_SUB_VEN,   GAINWARD_RTX3050_SUB_DEV,                       NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGB,     "Gainward GeForce RTX 3050 LHR"               },
    {NVIDIA_VEN,    NVIDIA_RTX3060_8G_DEV,          NVIDIA_SUB_VEN,   NVIDIA_RTX3060_8G_DEV,                          NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGB,     "Gainward GeForce RTX 3060 Pegasus"           },
    {NVIDIA_VEN,    NVIDIA_RTX3060_LHR_DEV,         NVIDIA_SUB_VEN,   NVIDIA_RTX3060_LHR_DEV,                         NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGB,     "Palit GeForce RTX 3060 LHR"                  },
    {NVIDIA_VEN,    NVIDIA_RTX3060_LHR_DEV,         PNY_SUB_VEN,      PNY_RTX_3060_XLR8_REVEL_EPIC_X_SUB_DEV,         NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGB,     "PNY GeForce RTX 3060 XLR8 REVEL EPIC-X"      },
    {NVIDIA_VEN,    NVIDIA_RTX3060_GA104_DEV,       PNY_SUB_VEN,      PNY_RTX_3060_XLR8_REVEL_EPIC_X_SUB_DEV,         NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGB,     "PNY GeForce RTX 3060 XLR8 REVEL EPIC-X"      },
    {NVIDIA_VEN,    NVIDIA_RTX3060_GA104_DEV,       NVIDIA_SUB_VEN,   NVIDIA_RTX3060_GA104_DEV,                       NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGB,     "Palit GeForce RTX 3060 LHR"                  },
    {NVIDIA_VEN,    NVIDIA_RTX3060TI_LHR_DEV,       PNY_SUB_VEN,      PNY_RTX_3060TI_XLR8_REVEL_EPIC_X_SUB_DEV,       NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGB,     "PNY GeForce RTX 3060 Ti XLR8 REVEL EPIC-X"   },
    {NVIDIA_VEN,    NVIDIA_RTX3060TI_LHR_DEV,       NVIDIA_SUB_VEN,   PNY_RTX_3060TI_XLR8_REVEL_EPIC_X_SUB_DEV,       NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGB,     "PNY GeForce RTX 3060 Ti XLR8 REVEL EPIC-X"   },
    {NVIDIA_VEN,    NVIDIA_RTX3060TI_V1_LHR_DEV,    NVIDIA_SUB_VEN,   NVIDIA_RTX3060TI_V1_LHR_DEV,                    NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGB,     "NVIDIA GeForce RTX 3060 Ti V1 LHR"           },
    {NVIDIA_VEN,    NVIDIA_RTX3060TI_LHR_DEV,       NVIDIA_SUB_VEN,   NVIDIA_RTX3060TI_LHR_DEV,                       NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGB,     "NVIDIA GeForce RTX 3060 Ti LHR"              },
    {NVIDIA_VEN,    NVIDIA_RTX3070TI_DEV,           PNY_SUB_VEN,      PNY_RTX_3070TI_XLR8_UPRISING_EPIC_X_SUB_DEV,    NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "PNY GeForce RTX 3070 Ti XLR8 Uprising EPIC-X"},
    {NVIDIA_VEN,    NVIDIA_RTX3080_DEV,             NVIDIA_SUB_VEN,   NVIDIA_RTX3080_FE_SUB_DEV,                      NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "NVIDIA GeForce RTX 3080 FE"                  },
    {NVIDIA_VEN,    NVIDIA_RTX3080TI_DEV,           NVIDIA_SUB_VEN,   NVIDIA_RTX3080TI_FE_SUB_DEV,                    NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "NVIDIA GeForce RTX 3080 Ti FE"               },
    {NVIDIA_VEN,    NVIDIA_RTX3080TI_DEV,           NVIDIA_SUB_VEN,   MANLI_RTX3080TI_GALLARDO_SUB_DEV,               NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGB,     "MANLI GeForce RTX 3080 Ti GALLARDO"          },
    {NVIDIA_VEN,    NVIDIA_RTX3090_DEV,             NVIDIA_SUB_VEN,   NVIDIA_RTX3090_FE_SUB_DEV,                      NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "NVIDIA GeForce RTX 3090 FE"                  },
    {NVIDIA_VEN,    NVIDIA_RTX3090TI_DEV,           NVIDIA_SUB_VEN,   NVIDIA_RTX3090TI_FE_SUB_DEV,                    NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "NVIDIA GeForce RTX 3090 Ti FE"               },
    {NVIDIA_VEN,    NVIDIA_RTX4060_DEV,             PALIT_SUB_VEN,    PALIT_RTX4060_SUB_DEV,                          NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGB,     "Palit GeForce RTX 4060 Dual"                 },
    {NVIDIA_VEN,    NVIDIA_RTX4070_DEV,             PALIT_SUB_VEN,    PALIT_RTX4070_SUB_DEV,                          NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGB,     "Palit GeForce RTX 4070"                      },
    {NVIDIA_VEN,    NVIDIA_RTX4070S_DEV,            PALIT_SUB_VEN,    PALIT_RTX4070S_SUB_DEV,                         NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGB,     "Palit GeForce RTX 4070 SUPER Dual"           },
    {NVIDIA_VEN,    NVIDIA_RTX4070_DEV,             GAINWARD_SUB_VEN, GAINWARD_RTX_4070_GHOST_SUB_DEV,                NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "Gainward GeForce RTX 4070 Ghost"             },
    {NVIDIA_VEN,    NVIDIA_RTX4080_DEV,             NVIDIA_SUB_VEN,   NVIDIA_RTX4080_FE_SUB_DEV,                      NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "NVIDIA GeForce RTX 4080 FE"                  },
    {NVIDIA_VEN,    NVIDIA_RTX4080_DEV,             NVIDIA_SUB_VEN,   NVIDIA_RTX4080_FE_SUB_DEV2,                     NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "NVIDIA GeForce RTX 4080 FE"                  },
    {NVIDIA_VEN,    NVIDIA_RTX4080S_DEV,            NVIDIA_SUB_VEN,   NVIDIA_RTX4080S_FE_SUB_DEV,                     NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "NVIDIA GeForce RTX 4080 SUPER FE"            },
    {NVIDIA_VEN,    NVIDIA_RTX4090_DEV,             NVIDIA_SUB_VEN,   NVIDIA_RTX4090_FE_SUB_DEV,                      NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "NVIDIA GeForce RTX 4090 FE"                  },
    {NVIDIA_VEN,    NVIDIA_RTX4090_DEV,             NVIDIA_SUB_VEN,   NVIDIA_RTX4090_FE_SUB_DEV2,                     NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "NVIDIA GeForce RTX 4090 FE"                  },
    {NVIDIA_VEN,    NVIDIA_RTX5080_DEV,             NVIDIA_SUB_VEN,   NVIDIA_RTX5080_FE_SUB_DEV,                      NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "NVIDIA GeForce RTX 5080 FE"                  },
    {NVIDIA_VEN,    NVIDIA_RTX5090_DEV,             NVIDIA_SUB_VEN,   NVIDIA_RTX5090_FE_SUB_DEV,                      NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "NVIDIA GeForce RTX 5090 FE"                  },
};

void DetectNVIDIAIllumGPUs()
{
    static NV_PHYSICAL_GPU_HANDLE   gpu_handles[64];
    static NV_S32                   gpu_count = 0;
    NV_U32                          device_id;
    NV_U32                          ext_device_id;
    NV_STATUS                       res;
    NV_U32                          revision_id;
    NV_U32                          sub_system_id;

    NvAPI_Initialize();

    NvAPI_EnumPhysicalGPUs(gpu_handles, &gpu_count);

    for(NV_S32 gpu_idx = 0; gpu_idx < gpu_count; gpu_idx++)
    {
        res = NvAPI_GPU_GetPCIIdentifiers(gpu_handles[gpu_idx], &device_id, &sub_system_id, &revision_id, &ext_device_id);
        if (res == 0)
        {
            uint16_t pci_device           = device_id >> 16;
            uint16_t pci_vendor           = device_id & 0xffff;
            uint16_t pci_subsystem_device = sub_system_id >> 16;
            uint16_t pci_subsystem_vendor = sub_system_id & 0xffff;
            for(unsigned int dev_idx = 0; dev_idx < GPU_NUM_DEVICES; dev_idx++)
            {
                if(pci_vendor           == device_list[dev_idx].pci_vendor           &&
                   pci_device           == device_list[dev_idx].pci_device           &&
                   pci_subsystem_vendor == device_list[dev_idx].pci_subsystem_vendor &&
                   pci_subsystem_device == device_list[dev_idx].pci_subsystem_device)
                {
                    LOG_DEBUG("[%s] Nvidia NvAPI Illumination GPU found", device_list[dev_idx].name);
                    switch(device_list[dev_idx].gpu_rgb_version)
                    {
                        case NVIDIA_ILLUMINATION_V1:
                            {
                                nvapi_accessor*                     new_nvapi      = new nvapi_accessor(gpu_handles[gpu_idx]);
                                NVIDIAIlluminationV1Controller*     controller     = new NVIDIAIlluminationV1Controller(new_nvapi, device_list[dev_idx].treats_rgbw_as_rgb, device_list[dev_idx].name);
                                RGBController_NVIDIAIlluminationV1* rgb_controller = new RGBController_NVIDIAIlluminationV1(controller);

                                ResourceManager::get()->RegisterRGBController(rgb_controller);
                            }
                            break;
                    }
                }
            }
        }
    }
}

REGISTER_DETECTOR("Nvidia NvAPI Illumination", DetectNVIDIAIllumGPUs);
