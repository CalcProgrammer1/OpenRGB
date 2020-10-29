#include "Detector.h"
#include "RGBFusionGPUController.h"
#include "RGBController.h"
#include "RGBController_RGBFusionGPU.h"
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
    { NVIDIA_VEN,   NVIDIA_GTX1080TI_DEV,   GIGABYTE_SUB_VEN,   GIGABYTE_GTX1080TI_XTREME_SUB_DEV,  "Gigabyte GTX1080Ti Xtreme Edition" },
};

/******************************************************************************************\
*                                                                                          *
*   TestForRGBFusionGPUController                                                          *
*                                                                                          *
*       Tests the given address to see if an RGB Fusion controller exists there.  First    *
*       does a quick write to test for a response                                          *
*                                                                                          *
\******************************************************************************************/

bool TestForRGBFusionGPUController(i2c_smbus_interface* bus, unsigned char address)
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

        pass = true;

        res = bus->i2c_smbus_read_byte(address);

        if (res != 0xAB)
        {
            pass = false;
        }

        res = bus->i2c_smbus_read_byte(address);

        if(res != 0x14)
        {
            pass = false;
        }

        bus->i2c_smbus_read_byte(address);
        bus->i2c_smbus_read_byte(address);
    }

    return(pass);

}   /* TestForRGBFusionGPUController() */

/******************************************************************************************\
*                                                                                          *
*   DetectRGBFusionGPUControllers                                                          *
*                                                                                          *
*       Detect RGB Fusion controllers on the enumerated I2C busses at address 0x47.        *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where RGB Fusion device is connected      *
*           dev - I2C address of RGB Fusion device                                         *
*                                                                                          *
\******************************************************************************************/

void DetectRGBFusionGPUControllers(std::vector<i2c_smbus_interface*>& busses, std::vector<RGBController*>& rgb_controllers)
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
                // Check for RGB Fusion controller at 0x47
                if (TestForRGBFusionGPUController(busses[bus], 0x47))
                {
                    new_rgb_fusion = new RGBFusionGPUController(busses[bus], 0x47);
                    new_controller = new RGBController_RGBFusionGPU(new_rgb_fusion);
                    new_controller->name = device_list[dev_idx].name;
                    rgb_controllers.push_back(new_controller);
                }
            }
        }
    }

}   /* DetectRGBFusionGPUControllers() */

REGISTER_I2C_DETECTOR("Gigabyte RGB Fusion GPU", DetectRGBFusionGPUControllers);
