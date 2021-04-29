/*-----------------------------------------*\
|  GainwardGPUControllerDetect.cpp          |
|                                           |
|  Driver for Gainward RGB on GPUs          |
|                                           |
|  TheRogueZeta 11/05/2020                  |
\*-----------------------------------------*/

#include "Detector.h"
#include "GainwardGPUv1Controller.h"
#include "GainwardGPUv2Controller.h"
#include "RGBController.h"
#include "RGBController_GainwardGPUv1.h"
#include "RGBController_GainwardGPUv2.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std::chrono_literals;

enum
{
    RGB_V1,
    RGB_V2,
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
    { NVIDIA_VEN,   NVIDIA_GTX1080_DEV,     GAINWARD_SUB_VEN,       GAINWARD_GTX_1080_PHOENIX,     RGB_V1,     "Gainward GTX 1080 Phoenix"             },
    { NVIDIA_VEN,   NVIDIA_RTX2070S_DEV,    GAINWARD_SUB_VEN,       NVIDIA_RTX2070S_DEV,           RGB_V2,     "Gainward RTX 2070 Super Phantom"       },
    { NVIDIA_VEN,   NVIDIA_RTX3070_DEV,     GAINWARD_SUB_VEN,       NVIDIA_RTX3070_DEV,            RGB_V2,     "Gainward RTX 3070 Phoenix"             },
};

/******************************************************************************************\
*                                                                                          *
*   TestForGainwardGPUController                                                           *
*                                                                                          *
*       Tests the given address to see if a Gainward GPU controller exists there.          *
*                                                                                          *
\******************************************************************************************/

bool TestForGainwardGPUController(i2c_smbus_interface* bus, int gpu_rgb_version)
{
    bool pass = false;

    switch (gpu_rgb_version)
    {
        case RGB_V1:
            pass = bus->i2c_smbus_write_quick(0x08, I2C_SMBUS_WRITE);
            break;

        case RGB_V2:
            /*-------------------------------------------------------------*\
            | This detection might need some modifications                  |
            | Reading 0x6F*0x73 and comparing to 0x64 might be a possibility|
            \*-------------------------------------------------------------*/
            s32 data        = bus->i2c_smbus_read_byte_data(0x49, 0x0);
            s32 mode_data   = bus->i2c_smbus_read_byte_data(0x49, 0xe0);
            pass            = (data == 0x0) && (mode_data < 0x5);
            break;
    }

    return(pass);

}   /* TestForGainwardGPUController() */


/******************************************************************************************\
*                                                                                          *
*   DetectGainwardGPUControllers                                                           *
*                                                                                          *
*       Detect Gainward GPU controllers on the enumerated I2C busses.                      *
*                                                                                          *
\******************************************************************************************/

void DetectGainwardGPUControllers(std::vector<i2c_smbus_interface*> &busses)
{

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        /*-------------------------------------*\
        | Check for Gainward controller at 0x08 |
        \*-------------------------------------*/
        for(unsigned int dev_idx = 0; dev_idx < GPU_NUM_DEVICES; dev_idx++)
        {
            if(busses[bus]->pci_vendor           == device_list[dev_idx].pci_vendor           &&
               busses[bus]->pci_device           == device_list[dev_idx].pci_device           &&
               busses[bus]->pci_subsystem_vendor == device_list[dev_idx].pci_subsystem_vendor &&
               busses[bus]->pci_subsystem_device == device_list[dev_idx].pci_subsystem_device)
            {

                if (TestForGainwardGPUController(busses[bus], device_list[dev_idx].gpu_rgb_version))
                {
                    switch(device_list[dev_idx].gpu_rgb_version)
                    {
                        case RGB_V1:
                            {
                                GainwardGPUv1Controller*     new_GainwardGPU;
                                RGBController_GainwardGPUv1* new_controller;

                                new_GainwardGPU         = new GainwardGPUv1Controller(busses[bus], 0x08);
                                new_controller          = new RGBController_GainwardGPUv1(new_GainwardGPU);
                                new_controller->name    = device_list[dev_idx].name;
                                ResourceManager::get()->RegisterRGBController(new_controller);
                            }
                            break;

                        case RGB_V2:
                            {
                                GainwardGPUv2Controller*     new_GainwardGPU;
                                RGBController_GainwardGPUv2* new_controller;

                                new_GainwardGPU         = new GainwardGPUv2Controller(busses[bus], 0x49);
                                new_controller          = new RGBController_GainwardGPUv2(new_GainwardGPU);
                                new_controller->name    = device_list[dev_idx].name;
                                ResourceManager::get()->RegisterRGBController(new_controller);
                            }
                            break;
                    }
                }
            }
        }
    }

} /* DetectGainwardGPUControllers() */

REGISTER_I2C_DETECTOR("Gainward GPU", DetectGainwardGPUControllers);
