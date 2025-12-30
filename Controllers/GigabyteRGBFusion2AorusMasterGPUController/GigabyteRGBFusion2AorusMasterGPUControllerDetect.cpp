/*---------------------------------------------------------*\
| GigabyteRGBFusion2AorusMasterGPUControllerDetect.cpp      |
|                                                           |
|   Detector for Gigabyte AORUS MASTER GPU                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <stdio.h>
#include "Detector.h"
#include "GigabyteRGBFusion2AorusMasterGPUController.h"
#include "i2c_smbus.h"
#include "LogManager.h"
#include "pci_ids.h"
#include "RGBController_GigabyteRGBFusion2AorusMasterGPU.h"


#define GIGABYTEGPU_CONTROLLER_NAME_AORUS_MASTER    "Gigabyte RGB Fusion2 AORUS MASTER GPU"

bool TestForGigabyteRGBFusion2AorusMasterGPUController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;
    int res, pktsz;
    const int read_sz = 4;
    const int write_sz = 8;
    uint8_t data_pkt[write_sz] = { 0xAB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    uint8_t data_readpkt[read_sz] = {};

    res = bus->i2c_write_block(address, write_sz, data_pkt);

    if(res < 0)
    {
        LOG_DEBUG("[%s] Write probe failed at address 0x%02X", GIGABYTEGPU_CONTROLLER_NAME_AORUS_MASTER, address);
        return false;
    }

    pktsz = read_sz;
    res = bus->i2c_read_block(address, &pktsz, data_readpkt);

    /*-----------------------------------------------------*\
    | All RGB Fusion 2 responses start with 0xAB            |
    | RTX 5080 AORUS MASTER response: 0xAB 0x10 0x52 0x0B   |
    \*-----------------------------------------------------*/
    if(res >= 0 && data_readpkt[0] == 0xAB)
    {
        pass = true;
        LOG_DEBUG("[%s] Detected at address 0x%02X with response: 0x%02X 0x%02X 0x%02X 0x%02X",
            GIGABYTEGPU_CONTROLLER_NAME_AORUS_MASTER, address,
            data_readpkt[0], data_readpkt[1], data_readpkt[2], data_readpkt[3]);
    }
    else
    {
        std::string text = "";
        for(int idx = 0; idx < read_sz; ++idx)
        {
            char str[6];
            snprintf(str, 6, " 0x%02X", data_readpkt[idx]);
            text.append(str);
        }
        LOG_DEBUG("[%s] at address 0x%02X invalid. Expected 0xAB [0x*] but received:%s",
            GIGABYTEGPU_CONTROLLER_NAME_AORUS_MASTER, address, text.c_str());
    }

    return(pass);
}

void DetectGigabyteRGBFusion2AorusMasterGPUControllers(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name)
{
    if(TestForGigabyteRGBFusion2AorusMasterGPUController(bus, i2c_addr))
    {
        RGBFusion2AorusMasterGPUController*     controller     = new RGBFusion2AorusMasterGPUController(bus, i2c_addr, name);
        RGBController_RGBFusion2AorusMasterGPU* rgb_controller = new RGBController_RGBFusion2AorusMasterGPU(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

/*---------------------------------------------------------*\
|  Nvidia GPUs                                              |
\*---------------------------------------------------------*/
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS GeForce RTX 5080 MASTER",                    DetectGigabyteRGBFusion2AorusMasterGPUControllers, NVIDIA_VEN, NVIDIA_RTX5080_DEV,     GIGABYTE_SUB_VEN,   GIGABYTE_AORUS_RTX5080_MASTER_16G_SUB_DEV,      0x71);
