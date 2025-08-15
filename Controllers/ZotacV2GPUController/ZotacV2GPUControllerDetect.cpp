/*---------------------------------------------------------*\
| ZotacV2GPUControllerDetect.cpp                            |
|                                                           |
|   Detector for Zotac V2 GPU                               |
|                                                           |
|   Krzysztof Haładyn (krzys_h)                 16 Mar 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "ZotacV2GPUController.h"
#include "RGBController_ZotacV2GPU.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include "LogManager.h"

/******************************************************************************************\
*                                                                                          *
*   DetectZotacV2GPUControllers                                                            *
*                                                                                          *
*       Detect ZOTAC 30/40 series RGB controllers on the enumerated I2C busses             *
*       at address 0x49.                                                                   *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where RGB device is connected             *
*           dev - I2C address of RGB device                                                *
*                                                                                          *
\******************************************************************************************/

void DetectZotacV2GPUControllers(i2c_smbus_interface* bus, u8 i2c_addr, const std::string& name)
{
    u8  rdata_pkt[I2C_SMBUS_BLOCK_MAX]  = { 0x00 };
    int rdata_len                       = sizeof(rdata_pkt);

    if(bus->i2c_read_block(i2c_addr, &rdata_len, rdata_pkt) >= 0)
    {
        ZotacV2GPUController*     controller     = new ZotacV2GPUController(bus, i2c_addr, name);
        RGBController_ZotacV2GPU* rgb_controller = new RGBController_ZotacV2GPU(controller);

        if(rgb_controller->config.numberOfZones > 0)
        {
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
        else
        {
            LOG_ERROR("[%s] RGB controller not registered.", name.c_str());
        }
    }
}

REGISTER_I2C_PCI_DETECTOR("ZOTAC GAMING GeForce RTX 3070 AMP Holo LHR",           DetectZotacV2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3070_LHR_DEV,     ZOTAC_SUB_VEN, ZOTAC_RTX3070_AMP_SUB_DEV,             0x49);
REGISTER_I2C_PCI_DETECTOR("ZOTAC GAMING GeForce RTX 3070 Ti",                     DetectZotacV2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3070TI_GA102_DEV, ZOTAC_SUB_VEN, ZOTAC_RTX3070TI_SUB_DEV,               0x49);
REGISTER_I2C_PCI_DETECTOR("ZOTAC GAMING GeForce RTX 3070 Ti Trinity OC/AMP Holo", DetectZotacV2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3070TI_DEV,       ZOTAC_SUB_VEN, ZOTAC_RTX3070TI_TRINITY_SUB_DEV,       0x49);
REGISTER_I2C_PCI_DETECTOR("ZOTAC GAMING GeForce RTX 3080 Trinity OC",             DetectZotacV2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3080_DEV,         ZOTAC_SUB_VEN, ZOTAC_RTX3080_TRINITY_SUB_DEV,         0x49);
REGISTER_I2C_PCI_DETECTOR("ZOTAC GAMING GeForce RTX 3080 Trinity LHR",            DetectZotacV2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3080_LHR_DEV,     ZOTAC_SUB_VEN, ZOTAC_RTX3080_TRINITY_SUB_DEV,         0x49);
REGISTER_I2C_PCI_DETECTOR("ZOTAC GAMING GeForce RTX 3080 AMP Holo",               DetectZotacV2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3080_DEV,         ZOTAC_SUB_VEN, ZOTAC_RTX3080_AMP_SUB_DEV,             0x49);
REGISTER_I2C_PCI_DETECTOR("ZOTAC GAMING GeForce RTX 3080 AMP Holo LHR",           DetectZotacV2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3080_LHR_DEV,     ZOTAC_SUB_VEN, ZOTAC_RTX3080_AMP_SUB_DEV,             0x49);
REGISTER_I2C_PCI_DETECTOR("ZOTAC GAMING GeForce RTX 3080 12GB Trinity OC LHR",    DetectZotacV2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3080_12G_LHR_DEV, ZOTAC_SUB_VEN, ZOTAC_RTX3080_12G_LHR_TRINITY_SUB_DEV, 0x49);
REGISTER_I2C_PCI_DETECTOR("ZOTAC GAMING GeForce RTX 3080 Ti AMP Holo",            DetectZotacV2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3080TI_DEV,       ZOTAC_SUB_VEN, ZOTAC_RTX3080TI_AMP_SUB_DEV,           0x49);
REGISTER_I2C_PCI_DETECTOR("ZOTAC GAMING GeForce RTX 3090 AMP Extreme Holo",       DetectZotacV2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3090_DEV,         ZOTAC_SUB_VEN, ZOTAC_RTX3090_AMP_SUB_DEV,             0x49);
REGISTER_I2C_PCI_DETECTOR("ZOTAC GAMING GeForce RTX 3090 Trinity",                DetectZotacV2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3090_DEV,         ZOTAC_SUB_VEN, ZOTAC_RTX3090_TRINITY_SUB_DEV,         0x49);
REGISTER_I2C_PCI_DETECTOR("ZOTAC GAMING GeForce RTX 4070 Ti Trinity OC",          DetectZotacV2GPUControllers, NVIDIA_VEN, NVIDIA_RTX4070TI_DEV,       ZOTAC_SUB_VEN, ZOTAC_RTX4070TI_TRINITY_SUB_DEV,       0x49);
REGISTER_I2C_PCI_DETECTOR("ZOTAC GAMING GeForce RTX 4080 AMP Extreme AIRO",       DetectZotacV2GPUControllers, NVIDIA_VEN, NVIDIA_RTX4080_DEV,         ZOTAC_SUB_VEN, ZOTAC_RTX4080_AMP_SUB_DEV,             0x49);
REGISTER_I2C_PCI_DETECTOR("ZOTAC GAMING GeForce RTX 4080 AMP Extreme AIRO",       DetectZotacV2GPUControllers, NVIDIA_VEN, NVIDIA_RTX4080_DEV,         ZOTAC_SUB_VEN, ZOTAC_RTX4080_AMP_ALT_SUB_DEV,         0x49);
REGISTER_I2C_PCI_DETECTOR("ZOTAC GAMING GeForce RTX 4090 Trinity OC",             DetectZotacV2GPUControllers, NVIDIA_VEN, NVIDIA_RTX4090_DEV,         ZOTAC_SUB_VEN, ZOTAC_RTX4090_TRINITY_SUB_DEV,         0x49);
REGISTER_I2C_PCI_DETECTOR("ZOTAC GAMING GeForce RTX 4090 AMP Extreme AIRO",       DetectZotacV2GPUControllers, NVIDIA_VEN, NVIDIA_RTX4090_DEV,         ZOTAC_SUB_VEN, ZOTAC_RTX4090_AMP_SUB_DEV,             0x49);
