/*---------------------------------------------------------*\
| GigabyteRGBFusion2BlackwellGPUControllerDetect.cpp        |
|                                                           |
|   Detector for Gigabyte RGB Fusion 2 Blackwell GPU        |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <stdio.h>
#include "Detector.h"
#include "GigabyteRGBFusion2BlackwellGPUController.h"
#include "LogManager.h"
#include "RGBController_GigabyteRGBFusion2BlackwellGPU.h"
#include "i2c_smbus.h"
#include "pci_ids.h"

#define GIGABYTEGPU_CONTROLLER_NAME3    "Gigabyte RGB Fusion2 Blackwell GPU"

/******************************************************************************************\
*                                                                                          *
*   TestForGigabyteRGBFusion2BlackwellGPUController                                        *
*                                                                                          *
*       Tests the given address to see if an RGB Fusion2 controller exists there.  First   *
*       does a quick write to test for a response                                          *
*                                                                                          *
\******************************************************************************************/

bool TestForGigabyteRGBFusion2BlackwellGPUController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;
    int res, pktsz;
    const int read_sz = 4;
    const int write_sz = 64; //0x40
    uint8_t data_pkt[write_sz] = { 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    uint8_t data_readpkt[read_sz] = {};

    res = bus->i2c_write_block(address, write_sz, data_pkt);

    pass = true;

    pktsz = read_sz;
    res = bus->i2c_read_block(address, &pktsz, data_readpkt);

    //What we have seen returned so far...
    //GeForce RTX 5070 Ti EAGLE OC 16G                          0x01 0x01 0x01 0x00

    if(res < 0 || data_readpkt[0] != 0x01 || data_readpkt[1] != 0x01 || data_readpkt[2] != 0x01)
    {
        // Assemble C-string with respons for debugging
        std::string text = "";

        for(int idx = 0; idx < read_sz; ++idx)
        {
            char str[6];
            snprintf(str, 6, " 0x%02X", data_readpkt[idx]);
            text.append(str);
        }

        LOG_DEBUG("[%s] at address 0x%02X invalid. Expected 0x01 0x01 0x01 [0x*] but received:%s", GIGABYTEGPU_CONTROLLER_NAME3, address, text.c_str());
        pass = false;
    }

    return(pass);
}   /* TestForRGBFusion2BlackwellGPUController() */

/*******************************************************************************************\
*                                                                                           *
*   DetectRGBFusion2BlackwellGPUControllers                                                 *
*                                                                                           *
*       Detect GigabyteRGB Fusion2 controllers on the enumerated I2C busses.                *
*                                                                                           *
*           bus - pointer to i2c_smbus_interface where RGB Fusion2 device is connected      *
*           dev - I2C address of RGB Fusion2 device                                         *
*                                                                                           *
\*******************************************************************************************/

void DetectGigabyteRGBFusion2BlackwellGPUControllers(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name)
{
    // Check for RGB Fusion2 controller
    if(TestForGigabyteRGBFusion2BlackwellGPUController(bus, i2c_addr))
    {
        RGBFusion2BlackwellGPUController*     controller     = new RGBFusion2BlackwellGPUController(bus, i2c_addr);
        RGBController_RGBFusion2BlackwellGPU* rgb_controller = new RGBController_RGBFusion2BlackwellGPU(controller);
        rgb_controller->name                        = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectGigabyteRGBFusion2BlackwellGPUControllers() */

/*-----------------------------------------*\
|  Nvidia GPUs                              |
\*-----------------------------------------*/

REGISTER_I2C_PCI_DETECTOR("Gigabyte GeForce RTX 5070 Ti Eagle OC",                     DetectGigabyteRGBFusion2BlackwellGPUControllers, NVIDIA_VEN, NVIDIA_RTX5070TI_DEV,       GIGABYTE_SUB_VEN,   GIGABYTE_RTX5070TI_EAGLE_OC_16G_SUB_DEV,        0x75);
REGISTER_I2C_PCI_DETECTOR("Gigabyte GeForce RTX 5070 Ti Aero OC",                      DetectGigabyteRGBFusion2BlackwellGPUControllers, NVIDIA_VEN, NVIDIA_RTX5070TI_DEV,       GIGABYTE_SUB_VEN,   GIGABYTE_RTX5070TI_AERO_OC_16G_SUB_DEV,         0x75);

/*-----------------------------------------*\
|  AMD GPUs                                 |
\*-----------------------------------------*/

