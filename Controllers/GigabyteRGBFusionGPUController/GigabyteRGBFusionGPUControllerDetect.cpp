#include "Detector.h"
#include "GigabyteRGBFusionGPUController.h"
#include "RGBController.h"
#include "RGBController_GigabyteRGBFusionGPU.h"
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
    unsigned char   controller_address;
    const char *    name;
} gpu_pci_device;

#define GPU_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[ 0 ]))

static const gpu_pci_device device_list[] =
{
    { NVIDIA_VEN,   NVIDIA_GTX1050TI_DEV,       GIGABYTE_SUB_VEN,   GIGABYTE_GTX1050TI_G1_GAMING_SUB_DEV,           0x48,   "Gigabyte GTX1050 Ti G1 Gaming"                 },
    { NVIDIA_VEN,   NVIDIA_GTX1060_DEV,         GIGABYTE_SUB_VEN,   GIGABYTE_GTX1060_XTREME_SUB_DEV,                0x47,   "Gigabyte GTX1060 Xtreme Gaming"                },
    { NVIDIA_VEN,   NVIDIA_GTX1070_DEV,         GIGABYTE_SUB_VEN,   GIGABYTE_GTX1070_XTREME_SUB_DEV,                0x47,   "Gigabyte GTX1070 Xtreme Gaming"                },
    { NVIDIA_VEN,   NVIDIA_GTX1080_DEV,         GIGABYTE_SUB_VEN,   GIGABYTE_GTX1080_G1_GAMING_SUB_DEV,             0x48,   "Gigabyte GTX1080 G1 Gaming"                    },
    { NVIDIA_VEN,   NVIDIA_GTX1080TI_DEV,       GIGABYTE_SUB_VEN,   GIGABYTE_GTX1080TI_SUB_DEV,                     0x47,   "Gigabyte GTX1080 Ti 11G"                       },
    { NVIDIA_VEN,   NVIDIA_GTX1080TI_DEV,       GIGABYTE_SUB_VEN,   GIGABYTE_GTX1080TI_GAMING_OC_SUB_DEV,           0x47,   "Gigabyte GTX1080 Ti Gaming OC 11G"             },
    { NVIDIA_VEN,   NVIDIA_GTX1080TI_DEV,       GIGABYTE_SUB_VEN,   GIGABYTE_GTX1080TI_GAMING_OC_BLACK_SUB_DEV,     0x47,   "Gigabyte GTX1080 Ti Gaming OC BLACK 11G"       },
    { NVIDIA_VEN,   NVIDIA_GTX1080TI_DEV,       GIGABYTE_SUB_VEN,   GIGABYTE_GTX1080TI_XTREME_SUB_DEV,              0x47,   "Gigabyte GTX1080 Ti Xtreme Edition"            },
    { NVIDIA_VEN,   NVIDIA_GTX1080TI_DEV,       GIGABYTE_SUB_VEN,   GIGABYTE_GTX1080TI_XTREME_WATERFORCE_SUB_DEV,   0x47,   "Gigabyte GTX1080 Ti Xtreme Waterforce Edition" },
    { NVIDIA_VEN,   NVIDIA_GTX1650_DEV,         GIGABYTE_SUB_VEN,   GIGABYTE_GTX1650_GAMING_OC_SUB_DEV,             0x55,   "Gigabyte GTX1650 Gaming OC"                    },
    { NVIDIA_VEN,   NVIDIA_GTX1660S_DEV,        GIGABYTE_SUB_VEN,   GIGABYTE_GTX1660S_GAMING_OC_SUB_DEV,            0x47,   "Gigabyte GTX1660 SUPER Gaming OC"              },
    { NVIDIA_VEN,   NVIDIA_RTX2060_TU106_DEV,   GIGABYTE_SUB_VEN,   GIGABYTE_RTX2060_GAMING_OC_SUB_DEV,             0x47,   "Gigabyte RTX2060 Gaming OC"                    },
    { NVIDIA_VEN,   NVIDIA_RTX2060_TU106_DEV,   GIGABYTE_SUB_VEN,   GIGABYTE_RTX2060_GAMING_OC_PRO_SUB_DEV,         0x47,   "Gigabyte RTX2060 Gaming OC PRO"                },
    { NVIDIA_VEN,   NVIDIA_RTX2060S_OC_DEV,     GIGABYTE_SUB_VEN,   GIGABYTE_RTX2060S_GAMING_SUB_DEV,               0x47,   "Gigabyte RTX2060 SUPER Gaming"                 },
    { NVIDIA_VEN,   NVIDIA_RTX2070_DEV,         GIGABYTE_SUB_VEN,   GIGABYTE_RTX2070_GAMING_OC_SUB_DEV,             0x47,   "Gigabyte RTX2070 Gaming OC 8G"                 },
    { NVIDIA_VEN,   NVIDIA_RTX2070_DEV,         GIGABYTE_SUB_VEN,   GIGABYTE_RTX2070_WINDFORCE_SUB_DEV,             0x47,   "Gigabyte RTX2070 Windforce 8G"                 },
    { NVIDIA_VEN,   NVIDIA_RTX2070S_DEV,        GIGABYTE_SUB_VEN,   GIGABYTE_RTX2070S_GAMING_OC_SUB_DEV,            0x47,   "Gigabyte RTX2070S Gaming OC"                   },
    { NVIDIA_VEN,   NVIDIA_RTX2070S_DEV,        GIGABYTE_SUB_VEN,   GIGABYTE_RTX2070S_GAMING_OC_3X_SUB_DEV,         0x55,   "Gigabyte RTX2070S Gaming OC 3X"                },
    { NVIDIA_VEN,   NVIDIA_RTX2080_DEV,         GIGABYTE_SUB_VEN,   GIGABYTE_RTX2080_GAMING_OC_SUB_DEV,             0x47,   "Gigabyte RTX2080 Gaming OC 8G"                 },
    { NVIDIA_VEN,   NVIDIA_RTX2080_A_DEV,       GIGABYTE_SUB_VEN,   GIGABYTE_RTX2080_A_GAMING_OC_SUB_DEV,           0x47,   "Gigabyte RTX2080 Gaming OC 8G"                 },
    { NVIDIA_VEN,   NVIDIA_RTX3060_DEV,         GIGABYTE_SUB_VEN,   GIGABYTE_RTX3060_GAMING_OC_12GB_SUB_DEV,        0x62,   "Gigabyte RTX3060 Gaming OC 12G"                },
    { NVIDIA_VEN,   NVIDIA_RTX3060TI_DEV,       GIGABYTE_SUB_VEN,   GIGABYTE_RTX3060TI_EAGLE_OC_SUB_DEV,            0x63,   "Gigabyte RTX3060 Ti EAGLE OC 8G"               },
    { NVIDIA_VEN,   NVIDIA_RTX3070_DEV,         GIGABYTE_SUB_VEN,   GIGABYTE_RTX3070_VISION_OC_SUB_DEV,             0x63,   "Gigabyte RTX3070 Vision 8G"                    },
};

/******************************************************************************************\
*                                                                                          *
*   TestForGigabyteRGBFusionGPUController                                                  *
*                                                                                          *
*       Tests the given address to see if an RGB Fusion controller exists there.  First    *
*       does a quick write to test for a response                                          *
*                                                                                          *
\******************************************************************************************/

bool TestForGigabyteRGBFusionGPUController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;
    int res;

    //Write out 0xAB 0x00 0x00 0x00 sequence
    res = bus->i2c_smbus_write_byte(address, 0xAB);

    if (res >= 0)
    {
        bus->i2c_smbus_write_byte(address, 0x00);
        bus->i2c_smbus_write_byte(address, 0x00);
        bus->i2c_smbus_write_byte(address, 0x00);
        
        // NVIDIA_RTX3060_DEV requires additional bytes to initialise
        if (address == 0x62)
        {
            bus->i2c_smbus_write_byte(address, 0x00);
            bus->i2c_smbus_write_byte(address, 0x00);
            bus->i2c_smbus_write_byte(address, 0x00);
            bus->i2c_smbus_write_byte(address, 0x00);
        }

        pass = true;

        res = bus->i2c_smbus_read_byte(address);

        if (res != 0xAB)
        {
            pass = false;
        }

        res = bus->i2c_smbus_read_byte(address);

        if ((res != 0x14) && (res != 0x10))
        {
            pass = false;
        }

        bus->i2c_smbus_read_byte(address);
        bus->i2c_smbus_read_byte(address);

        //We don't know what the 0x48 controller returns, so for now just assume it exists
        if(address == 0x48)
        {
            pass = true;
        }
    }

    return(pass);

}   /* TestForRGBFusionGPUController() */

/******************************************************************************************\
*                                                                                          *
*   DetectRGBFusionGPUControllers                                                          *
*                                                                                          *
*       Detect GigabyteRGB Fusion controllers on the enumerated I2C busses at address 0x47.*
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where RGB Fusion device is connected      *
*           dev - I2C address of RGB Fusion device                                         *
*                                                                                          *
\******************************************************************************************/

void DetectGigabyteRGBFusionGPUControllers(std::vector<i2c_smbus_interface*>& busses)
{
    RGBFusionGPUController* new_rgb_fusion;
    RGBController_RGBFusionGPU* new_controller;

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        for(unsigned int dev_idx = 0; dev_idx < GPU_NUM_DEVICES; dev_idx++)
        {
            if(busses[bus]->pci_vendor           == device_list[dev_idx].pci_vendor           &&
               busses[bus]->pci_device           == device_list[dev_idx].pci_device           &&
               busses[bus]->pci_subsystem_vendor == device_list[dev_idx].pci_subsystem_vendor &&
               busses[bus]->pci_subsystem_device == device_list[dev_idx].pci_subsystem_device)
            {
                // Check for RGB Fusion controller
                if (TestForGigabyteRGBFusionGPUController(busses[bus], device_list[dev_idx].controller_address))
                {
                    new_rgb_fusion = new RGBFusionGPUController(busses[bus], device_list[dev_idx].controller_address);
                    new_controller = new RGBController_RGBFusionGPU(new_rgb_fusion);
                    new_controller->name = device_list[dev_idx].name;
                    ResourceManager::get()->RegisterRGBController(new_controller);
                }
            }
        }
    }

}   /* DetectGigabyteRGBFusionGPUControllers() */

REGISTER_I2C_DETECTOR("Gigabyte RGB Fusion GPU", DetectGigabyteRGBFusionGPUControllers);
