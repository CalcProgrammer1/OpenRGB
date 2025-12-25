/*---------------------------------------------------------*\
| ManliGPUControllerDetect.cpp                              |
|                                                           |
|   Detector for Manli GPU                                  |
|                                                           |
|   Based on ZotacV2GPUControllerDetect                      |
|   Adapted for Manli RTX 4090 Gallardo                     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "ManliGPUController.h"
#include "RGBController_ManliGPU.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include "LogManager.h"

/******************************************************************************************\
*                                                                                          *
*   DetectManliGPUControllers                                                              *
*                                                                                          *
*       Detect Manli GPU RGB controllers on the enumerated I2C busses                     *
*       at address 0x49.                                                                   *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where RGB device is connected             *
*           dev - I2C address of RGB device                                                *
*                                                                                          *
\******************************************************************************************/

void DetectManliGPUControllers(i2c_smbus_interface* bus, u8 i2c_addr, const std::string& name)
{
    u8 data_pkt[] = { MANLI_GPU_REG_RGB, 0xF1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    if(bus->i2c_write_block(i2c_addr, sizeof(data_pkt), data_pkt) < 0)
    {
        return;
    }

    u8  rdata_pkt[I2C_SMBUS_BLOCK_MAX]  = { 0x00 };
    int rdata_len                       = sizeof(rdata_pkt);

    if(bus->i2c_read_block(i2c_addr, &rdata_len, rdata_pkt) >= 0)
    {
        ManliGPUController*     controller     = new ManliGPUController(bus, i2c_addr, name);
        RGBController_ManliGPU* rgb_controller = new RGBController_ManliGPU(controller);

        if(rgb_controller->config.numberOfZones > 0)
        {
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
        else
        {
            LOG_ERROR("[%s] RGB controller not registered - invalid zone count: %d", name.c_str(), rgb_controller->config.numberOfZones);
            delete rgb_controller;
        }
    }
}

REGISTER_I2C_PCI_DETECTOR("MANLI GeForce RTX 4090 Gallardo", DetectManliGPUControllers, NVIDIA_VEN, NVIDIA_RTX4090_DEV, NVIDIA_SUB_VEN, MANLI_RTX4090_GALLARDO_SUB_DEV, 0x49);

