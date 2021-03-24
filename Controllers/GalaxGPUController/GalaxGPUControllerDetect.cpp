/*-----------------------------------------*\
|  GalaxGPUControllerDetect.cpp             |
|                                           |
|  Driver for Galax / KFA2 RGB on GPUs      |
|                                           |
|  Niels Westphal (crashniels)  12.07.2020  |
\*-----------------------------------------*/

#include "Detector.h"
#include "GalaxGPUController.h"
#include "RGBController.h"
#include "RGBController_GalaxGPU.h"
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
    const char *    name;
} gpu_pci_device;

#define GPU_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[ 0 ]))

static const gpu_pci_device device_list[] =
{
    { NVIDIA_VEN,   NVIDIA_RTX2070_DEV,     NVIDIA_SUB_VEN,     KFA2_RTX_2070_EX_SUB_DEV,               "KFA2 RTX 2070 EX"                      },
    { NVIDIA_VEN,   NVIDIA_RTX2070S_DEV,    NVIDIA_SUB_VEN,     GALAX_RTX_2070S_EX_GAMER_BLACK_SUB_DEV, "GALAX RTX 2070 Super EX Gamer Black"   },
    { NVIDIA_VEN,   NVIDIA_RTX2080_DEV,     NVIDIA_SUB_VEN,     KFA2_RTX_2080_EX_OC_SUB_DEV,            "KFA2 RTX 2080 EX OC"                   },
};
/******************************************************************************************\
*                                                                                          *
*   TestForGalaxGPUController                                                              *
*                                                                                          *
*       Tests the given address to see if a Galax GPU controller exists there.             *
*                                                                                          *
\******************************************************************************************/

bool TestForGalaxGPUController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    unsigned char res  = bus->i2c_smbus_read_byte_data(address, 0x00);
    unsigned char res2 = bus->i2c_smbus_read_byte_data(address, 0x01);
    if(res == 0x27 && res2 == 0x10)
    {
        pass = true;
    }

    return(pass);

}   /* TestForGalaxGPUController() */


/******************************************************************************************\
*                                                                                          *
*   DetectGalaxGPUControllers                                                              *
*                                                                                          *
*       Detect Galax GPU controllers on the enumerated I2C busses.                         *
*                                                                                          *
\******************************************************************************************/

void DetectGalaxGPUControllers(std::vector<i2c_smbus_interface*> &busses)
{
    GalaxGPUController* new_GalaxGPU;
    RGBController_GalaxGPU* new_controller;

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        // Check for GALAX controller at 0x23
        for(unsigned int dev_idx = 0; dev_idx < GPU_NUM_DEVICES; dev_idx++)
        {
            if(busses[bus]->pci_vendor           == device_list[dev_idx].pci_vendor           &&
               busses[bus]->pci_device           == device_list[dev_idx].pci_device           &&
               busses[bus]->pci_subsystem_vendor == device_list[dev_idx].pci_subsystem_vendor &&
               busses[bus]->pci_subsystem_device == device_list[dev_idx].pci_subsystem_device)
            {
                if (TestForGalaxGPUController(busses[bus], 0x23))
                {
                    new_GalaxGPU         = new GalaxGPUController(busses[bus], 0x23);
                    new_controller       = new RGBController_GalaxGPU(new_GalaxGPU);
                    new_controller->name = device_list[dev_idx].name;
                    ResourceManager::get()->RegisterRGBController(new_controller);
                }
            }
        }
    }

} /* DetectGalaxGPUControllers() */

REGISTER_I2C_DETECTOR("Galax GPU", DetectGalaxGPUControllers);
