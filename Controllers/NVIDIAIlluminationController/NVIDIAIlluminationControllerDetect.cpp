/*----------------------------------------------*\
|  NVIDIAIlluminationControllerDetect.cpp        |
|                                                |
|  Detect GPUS that are controlled by the direct |
|  NVIDIA Illumination controller.               |
|                                                |
|  Carter Miller (GingerRunner) 1/4/2022         |
\*----------------------------------------------*/

#include "Detector.h"

#include "LogManager.h"
#include "RGBController.h"
#include "RGBController_NVIDIAIllumination.h"

#include "pci_ids.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

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
} gpu_pci_device;


#define GPU_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[ 0 ]))

/*-----------------------------------------------------------------------------------------------------*\
| Certain devices seem to ignore the white value entirely, despite the zone being reported back by the  |
| API as RGBW, so this boolean is passed at detection time via constructor inform the controller logic. |
\*-----------------------------------------------------------------------------------------------------*/
#define TREATS_RGBW_AS_RGB  true
#define TREATS_RGBW_AS_RGBW false

static const gpu_pci_device device_list[] =
{
    {NVIDIA_VEN,    NVIDIA_RTX2070S_DEV,            NVIDIA_VEN,     NVIDIA_RTX2070_FE_SUPER_SUB_DEV,            NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "NVIDIA 2070 SUPER FE"          },
    {NVIDIA_VEN,    NVIDIA_RTX3060_LHR_DEV,         PNY_SUB_VEN,    PNY_RTX_3060_XLR8_REVEL_EPIC_X_SUB_DEV,     NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGB,     "PNY 3060 XLR8 REVEL EPIC-X"    },
    {NVIDIA_VEN,    NVIDIA_RTX3060_GA104_DEV,       PNY_SUB_VEN,    PNY_RTX_3060_XLR8_REVEL_EPIC_X_SUB_DEV,     NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGB,     "PNY 3060 XLR8 REVEL EPIC-X"    },
    {NVIDIA_VEN,    NVIDIA_RTX3060TI_LHR_DEV,       PNY_SUB_VEN,    PNY_RTX_3060TI_XLR8_REVEL_EPIC_X_SUB_DEV,   NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGB,     "PNY 3060TI XLR8 REVEL EPIC-X"  },
    {NVIDIA_VEN,    NVIDIA_RTX3060TI_LHR_DEV,       NVIDIA_VEN,     PNY_RTX_3060TI_XLR8_REVEL_EPIC_X_SUB_DEV,   NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGB,     "PNY 3060TI XLR8 REVEL EPIC-X"  },
    {NVIDIA_VEN,    NVIDIA_RTX3060TI_V1_LHR_DEV,    NVIDIA_VEN,     NVIDIA_RTX3060TI_V1_LHR_DEV,                NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGB,     "NVIDIA 3060TI V1 LHR"          },
    {NVIDIA_VEN,    NVIDIA_RTX3060TI_LHR_DEV,       NVIDIA_VEN,     NVIDIA_RTX3060TI_LHR_DEV,                   NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "NVIDIA 3060TI LHR"             },
    {NVIDIA_VEN,    NVIDIA_RTX3080_DEV,             NVIDIA_VEN,     NVIDIA_RTX3080_FE_SUB_DEV,                  NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "NVIDIA 3080 FE"                },
    {NVIDIA_VEN,    NVIDIA_RTX3080TI_DEV,           NVIDIA_VEN,     NVIDIA_RTX3080TI_FE_SUB_DEV,                NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "NVIDIA 3080TI FE"              },
    {NVIDIA_VEN,    NVIDIA_RTX3080TI_DEV,           NVIDIA_VEN,     MANLI_RTX3080TI_GALLARDO_SUB_DEV,           NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGB,     "MANLI 3080TI GALLARDO"         },
    {NVIDIA_VEN,    NVIDIA_RTX3090_DEV,             NVIDIA_VEN,     NVIDIA_RTX3090_FE_SUB_DEV,                  NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "NVIDIA 3090 FE"                },
    {NVIDIA_VEN,    NVIDIA_RTX3090TI_DEV,           NVIDIA_VEN,     NVIDIA_RTX3090TI_FE_SUB_DEV,                NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "NVIDIA 3090TI FE"              },
    {NVIDIA_VEN,    NVIDIA_RTX4080_DEV,             NVIDIA_VEN,     NVIDIA_RTX4080_FE_SUB_DEV,                  NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "NVIDIA 4080 FE"                },
    {NVIDIA_VEN,    NVIDIA_RTX4090_DEV,             NVIDIA_VEN,     NVIDIA_RTX4090_FE_SUB_DEV,                  NVIDIA_ILLUMINATION_V1,     TREATS_RGBW_AS_RGBW,    "NVIDIA 4090 FE"                },
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
                    LOG_DEBUG("[%s] NVidia NvAPI Illumination GPU found", device_list[dev_idx].name);
                    switch(device_list[dev_idx].gpu_rgb_version)
                    {
                        case NVIDIA_ILLUMINATION_V1:
                            {
                                nvapi_accessor*                     new_nvapi      = new nvapi_accessor(gpu_handles[gpu_idx]);
                                NVIDIAIlluminationV1Controller*     controller     = new NVIDIAIlluminationV1Controller(new_nvapi, device_list[dev_idx].treats_rgbw_as_rgb);
                                RGBController_NVIDIAIlluminationV1* rgb_controller = new RGBController_NVIDIAIlluminationV1(controller);
                                rgb_controller->name                               = device_list[dev_idx].name;

                                ResourceManager::get()->RegisterRGBController(rgb_controller);
                            }
                            break;
                    }
                }
            }
        }
    }
}

REGISTER_DETECTOR("NVidia NvAPI Illumination", DetectNVIDIAIllumGPUs);
