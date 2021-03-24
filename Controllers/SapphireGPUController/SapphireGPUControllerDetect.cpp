#include "Detector.h"
#include "RGBController.h"
#include "SapphireNitroGlowV1Controller.h"
#include "SapphireNitroGlowV3Controller.h"
#include "RGBController_SapphireNitroGlowV1.h"
#include "RGBController_SapphireNitroGlowV3.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include <vector>

/*-----------------------------------------------------*\
| I2C Addresses for Sapphire Nitro Glow RGB             |
\*-----------------------------------------------------*/
enum
{
    SAPPHIRE_NITRO_GLOW_V1_ADDR = 0x55,
    SAPPHIRE_NITRO_GLOW_V3_ADDR = 0x28,
};

/*-----------------------------------------------------*\
| Nitro Glow Versions                                   |
\*-----------------------------------------------------*/
enum
{
    SAPPHIRE_NITRO_GLOW_V1,
    SAPPHIRE_NITRO_GLOW_V2,
    SAPPHIRE_NITRO_GLOW_V3,
};

typedef struct
{
    int             pci_vendor;
    int             pci_device;
    int             pci_subsystem_vendor;
    int             pci_subsystem_device;
    int             version;
    const char *    name;
} gpu_pci_device;

#define GPU_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[ 0 ]))

static const gpu_pci_device device_list[] =
{
    { AMD_GPU_VEN,  AMD_POLARIS_DEV,        SAPPHIRE_LEGACY_SUB_VEN,    SAPPHIRE_LEGACY_POLARIS_NITRO_PLUS_SUB_DEV, SAPPHIRE_NITRO_GLOW_V1, "RX 470/480 Nitro+"             },
    { AMD_GPU_VEN,  AMD_POLARIS_DEV,        SAPPHIRE_SUB_VEN,           SAPPHIRE_POLARIS_NITRO_PLUS_SUB_DEV1,       SAPPHIRE_NITRO_GLOW_V1, "RX 570/580/590 Nitro+"         },
    { AMD_GPU_VEN,  AMD_POLARIS_DEV,        SAPPHIRE_SUB_VEN,           SAPPHIRE_POLARIS_NITRO_PLUS_SUB_DEV2,       SAPPHIRE_NITRO_GLOW_V1, "RX 570/580/590 Nitro+"         },
    { AMD_GPU_VEN,  AMD_POLARIS20XL_DEV,    SAPPHIRE_SUB_VEN,           SAPPHIRE_POLARIS_NITRO_PLUS_SUB_DEV1,       SAPPHIRE_NITRO_GLOW_V1, "RX 580 Nitro+ (2048SP)"        },
    { AMD_GPU_VEN,  AMD_VEGA10_DEV,         SAPPHIRE_SUB_VEN,           SAPPHIRE_VEGA10_NITRO_PLUS_SUB_DEV,         SAPPHIRE_NITRO_GLOW_V1, "RX Vega 56/64 Nitro+"          },
    { AMD_GPU_VEN,  AMD_NAVI10_DEV,         SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI10_NITRO_PLUS_SUB_DEV1,        SAPPHIRE_NITRO_GLOW_V3, "RX 5700 (XT) Nitro+"           },
    { AMD_GPU_VEN,  AMD_NAVI10_DEV,         SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI10_NITRO_PLUS_SUB_DEV2,        SAPPHIRE_NITRO_GLOW_V3, "RX 5700 XT Nitro+"             },
    { AMD_GPU_VEN,  AMD_NAVI10_DEV,         SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI10_NITRO_PLUS_SUB_DEV3,        SAPPHIRE_NITRO_GLOW_V3, "RX 5700 XT Nitro+"             },
    { AMD_GPU_VEN,  AMD_NAVI14_DEV,         SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI14_NITRO_PLUS_SUB_DEV,         SAPPHIRE_NITRO_GLOW_V3, "RX 5500 XT Nitro+"             },
    { AMD_GPU_VEN,  AMD_NAVI21_DEV,         SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI21_NITRO_PLUS_SUB_DEV1,        SAPPHIRE_NITRO_GLOW_V3, "RX 6800 XT Nitro+ SE"          },
    { AMD_GPU_VEN,  AMD_NAVI21_DEV,         SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI21_NITRO_PLUS_SUB_DEV2,        SAPPHIRE_NITRO_GLOW_V3, "RX 6800 XT/6900 XT Nitro+"     },
    { AMD_GPU_VEN,  AMD_NAVI21_DEV,         SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI21_NITRO_PLUS_SUB_DEV3,        SAPPHIRE_NITRO_GLOW_V3, "RX 6800 Nitro+"                },
};

/******************************************************************************************\
*                                                                                          *
*   TestForSapphireGPUController                                                           *
*                                                                                          *
*       Tests the given address to see if an Sapphire controller exists there.  First      *
*       does a byte read to test for a response                                            *
*                                                                                          *
\******************************************************************************************/

bool TestForSapphireGPUController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;
    int res;

    //Read a byte to test for presence
    res = bus->i2c_smbus_read_byte(address);

    if (res >= 0)
    {
        pass = true;
    }

    return(pass);

}   /* TestForSapphireGPUController() */

/******************************************************************************************\
*                                                                                          *
*   DetectSapphireGPUControllers                                                           *
*                                                                                          *
*       Detect Sapphire GPU controllers on the enumerated I2C buses.                       *
*                                                                                          *
\******************************************************************************************/

void DetectSapphireGPUControllers(std::vector<i2c_smbus_interface *> &busses)
{
    for(unsigned int bus = 0; bus < busses.size(); bus++)
    {
        for(unsigned int dev_idx = 0; dev_idx < GPU_NUM_DEVICES; dev_idx++)
        {
            if(busses[bus]->pci_vendor           == device_list[dev_idx].pci_vendor           &&
               busses[bus]->pci_device           == device_list[dev_idx].pci_device           &&
               busses[bus]->pci_subsystem_vendor == device_list[dev_idx].pci_subsystem_vendor &&
               busses[bus]->pci_subsystem_device == device_list[dev_idx].pci_subsystem_device)
            {
                switch(device_list[dev_idx].version)
                {
                    case SAPPHIRE_NITRO_GLOW_V1:
                        if(TestForSapphireGPUController(busses[bus], SAPPHIRE_NITRO_GLOW_V1_ADDR))
                        {
                            SapphireNitroGlowV1Controller*     new_sapphire_gpu = new SapphireNitroGlowV1Controller(busses[bus], SAPPHIRE_NITRO_GLOW_V1_ADDR);
                            RGBController_SapphireNitroGlowV1* new_controller   = new RGBController_SapphireNitroGlowV1(new_sapphire_gpu);
                            new_controller->name = device_list[dev_idx].name;
                            ResourceManager::get()->RegisterRGBController(new_controller);
                        }
                        break;

                    case SAPPHIRE_NITRO_GLOW_V3:
                        if(TestForSapphireGPUController(busses[bus], SAPPHIRE_NITRO_GLOW_V3_ADDR))
                        {
                            SapphireNitroGlowV3Controller*     new_sapphire_gpu = new SapphireNitroGlowV3Controller(busses[bus], SAPPHIRE_NITRO_GLOW_V3_ADDR);
                            RGBController_SapphireNitroGlowV3* new_controller   = new RGBController_SapphireNitroGlowV3(new_sapphire_gpu);
                            new_controller->name = device_list[dev_idx].name;
                            ResourceManager::get()->RegisterRGBController(new_controller);
                        }
                        break;
                }
            }
        }
    }
}   /* DetectSapphireGPUControllers() */

REGISTER_I2C_DETECTOR("Sapphire GPU", DetectSapphireGPUControllers);
