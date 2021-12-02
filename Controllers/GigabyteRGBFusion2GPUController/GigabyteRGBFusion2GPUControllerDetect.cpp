#include "Detector.h"
#include "GigabyteRGBFusion2GPUController.h"
#include "LogManager.h"
#include "RGBController.h"
#include "RGBController_GigabyteRGBFusion2GPU.h"
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

#define GIGABYTEGPU_CONTROLLER_NAME2    "Gigabyte RGB Fusion2 GPU"
#define GPU_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[ 0 ]))

static const gpu_pci_device device_list[] =
{
    { NVIDIA_VEN,    NVIDIA_RTX2060S_OC_DEV,    GIGABYTE_SUB_VEN,   GIGABYTE_AORUS_RTX2060S_V1_SUB_DEV,             0x50,   "Gigabyte AORUS RTX2060 SUPER 8G V1"                },
    { NVIDIA_VEN,    NVIDIA_RTX2070_OC_DEV,     GIGABYTE_SUB_VEN,   GIGABYTE_AORUS_RTX2070_XTREME_SUB_DEV,          0x50,   "Gigabyte AORUS RTX2070 XTREME 8G"                  },
    { NVIDIA_VEN,    NVIDIA_RTX2070S_DEV,       GIGABYTE_SUB_VEN,   GIGABYTE_RTX2070S_GAMING_OC_SUB_DEV2,           0x50,   "Gigabyte AORUS RTX2070 SUPER 8G"                   },
    { NVIDIA_VEN,    NVIDIA_RTX3060_LHR_DEV,    GIGABYTE_SUB_VEN,   GIGABYTE_RTX3060_ELITE_12GB_SUB_DEV,            0x70,   "Gigabyte AORUS RTX3060 ELITE 12G"                  },
    { NVIDIA_VEN,    NVIDIA_RTX3060TI_DEV,      GIGABYTE_SUB_VEN,   GIGABYTE_RTX3060TI_GAMING_OC_PRO_SUB_DEV,       0x62,   "Gigabyte AORUS RTX3060 Ti GAMING OC PRO 8G"        },
    { NVIDIA_VEN,    NVIDIA_RTX3070_DEV,        GIGABYTE_SUB_VEN,   GIGABYTE_RTX3070_MASTER_OC_SUB_DEV,             0x66,   "Gigabyte RTX3070 MASTER 8G"                        },
    { NVIDIA_VEN,    NVIDIA_RTX3080_LHR_DEV,    GIGABYTE_SUB_VEN,   GIGABYTE_RTX3080_XTREME_WATERFORCE_SUB_DEV,     0x64,   "Gigabyte AORUS RTX3080 XTREME WATERFORCE WB 10G"   },
    { NVIDIA_VEN,    NVIDIA_RTX3080TI_DEV,      GIGABYTE_SUB_VEN,   GIGABYTE_RTX3080TI_XTREME_WATERFORCE_SUB_DEV,   0x65,   "Gigabyte AORUS RTX3080 Ti XTREME WATERFORCE 12G"   },
    { NVIDIA_VEN,    NVIDIA_RTX3090_DEV,        GIGABYTE_SUB_VEN,   GIGABYTE_RTX3090_XTREME_WATERFORCE_SUB_DEV,     0x64,   "Gigabyte AORUS RTX3090 XTREME WATERFORCE WB 24G"   },
};

/******************************************************************************************\
*                                                                                          *
*   TestForGigabyteRGBFusion2GPUController                                                 *
*                                                                                          *
*       Tests the given address to see if an RGB Fusion2 controller exists there.  First   *
*       does a quick write to test for a response                                          *
*                                                                                          *
\******************************************************************************************/

bool TestForGigabyteRGBFusion2GPUController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;
    int res, pktsz;
    const int read_sz = 4;
    const int write_sz = 8;
    uint8_t data_pkt[write_sz] = { 0xAB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    uint8_t data_readpkt[read_sz] = {};

    res = bus->i2c_write_block(address, write_sz, data_pkt);

    pass = true;

    pktsz = read_sz;
    res = bus->i2c_read_block(address, &pktsz, data_readpkt);

    //What we have seen returned so far...
    //GeForce RTX 3070 AORUS MASTER 8G                  0xAB 0x11 0x52 0x03
    //GeForce RTX 3060 Ti GAMING OC PRO 8G              0xAB 0x10 0x01 0x02
    //GeForce RTX 3070 AORUS ELITE 12G                  0xAB 0x11 0x52 0x03
    //GeForce RTX 3080 Ti AORUS XTREME WATERFORCE 12G   0xAB 0x11 0x01 0x00
    //GeForce RTX 3080 AORUS XTREME WATERFORCE WB 10G   0xAB 0x10 0x01 0x00
    if(res < 0)
    {
        LOG_DEBUG("[%s] at 0x%02X address expected 0x04 but received: 0x%02X", GIGABYTEGPU_CONTROLLER_NAME2, address, res);
        pass = false;
    }

    return(pass);
}   /* TestForRGBFusion2GPUController() */

/*******************************************************************************************\
*                                                                                           *
*   DetectRGBFusion2GPUControllers                                                          *
*                                                                                           *
*       Detect GigabyteRGB Fusion2 controllers on the enumerated I2C busses at address 0x70.*
*                                                                                           *
*           bus - pointer to i2c_smbus_interface where RGB Fusion2 device is connected      *
*           dev - I2C address of RGB Fusion2 device                                         *
*                                                                                           *
\*******************************************************************************************/

void DetectGigabyteRGBFusion2GPUControllers(std::vector<i2c_smbus_interface*>& busses)
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
                LOG_DEBUG(GPU_DETECT_MESSAGE, GIGABYTEGPU_CONTROLLER_NAME2, bus, device_list[dev_idx].pci_device, device_list[dev_idx].pci_subsystem_device, device_list[dev_idx].name );
                // Check for RGB Fusion2 controller
                if(TestForGigabyteRGBFusion2GPUController(busses[bus], device_list[dev_idx].controller_address))
                {
                    RGBFusion2GPUController*     controller     = new RGBFusion2GPUController(busses[bus], device_list[dev_idx].controller_address);
                    RGBController_RGBFusion2GPU* rgb_controller = new RGBController_RGBFusion2GPU(controller);

                    rgb_controller->name = device_list[dev_idx].name;

                    ResourceManager::get()->RegisterRGBController(rgb_controller);
                }
            }
        }
    }
}   /* DetectGigabyteRGBFusion2GPUControllers() */

REGISTER_I2C_DETECTOR("Gigabyte RGB Fusion2 GPU", DetectGigabyteRGBFusion2GPUControllers);
