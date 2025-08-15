/*---------------------------------------------------------*\
| ZotacTuringGPUControllerDetect.cpp                        |
|                                                           |
|   Detector for Zotac Turing GPU                           |
|                                                           |
|   David Henry                                 07 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "ZotacTuringGPUController.h"
#include "RGBController_ZotacTuringGPU.h"
#include "i2c_smbus.h"
#include "pci_ids.h"

/******************************************************************************************\
*                                                                                          *
*   TestForZotacTuringGPUController                                                        *
*                                                                                          *
*       Tests the given address to see if an RGB controller exists there.                  *
*                                                                                          *
\******************************************************************************************/

bool TestForZotacTuringGPUController(i2c_smbus_interface* bus, u8 i2c_addr)
{
    /*---------------------------------------------------------*\
    | This command seems to enable the RGB controller (0xF1,    |
    | 0x00 disables it).                                        |
    | Not really sure it's mandatory, but we can still use it   |
    | for testing the device: if the command succeeds, assume   |
    | it's a valid device.                                      |
    \*---------------------------------------------------------*/
    u8 data_pkt[] = { ZOTAC_TURING_GPU_REG_COLOR_AND_MODE, 0xF1, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 };
    return (bus->i2c_write_block(i2c_addr, sizeof(data_pkt), data_pkt) >= 0);
}

/******************************************************************************************\
*                                                                                          *
*   DetectZotacTuringGPUControllers                                                        *
*                                                                                          *
*       Detect ZOTAC Turing RGB controllers on the enumerated I2C busses at address 0x49.  *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where RGB device is connected             *
*           dev - I2C address of RGB device                                                *
*                                                                                          *
\******************************************************************************************/

void DetectZotacTuringGPUControllers(i2c_smbus_interface* bus, u8 i2c_addr, const std::string& name)
{
    if(TestForZotacTuringGPUController(bus, i2c_addr))
    {
        ZotacTuringGPUController*     controller     = new ZotacTuringGPUController(bus, i2c_addr, name);
        RGBController_ZotacTuringGPU* rgb_controller = new RGBController_ZotacTuringGPU(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_I2C_PCI_DETECTOR("ZOTAC GAMING GeForce RTX 2070 SUPER Twin Fan", DetectZotacTuringGPUControllers, NVIDIA_VEN, NVIDIA_RTX2070S_OC_DEV, ZOTAC_SUB_VEN, ZOTAC_RTX2070S_GAMING_SUB_DEV, 0x49);
REGISTER_I2C_PCI_DETECTOR("ZOTAC GAMING GeForce RTX 2080 SUPER Twin Fan", DetectZotacTuringGPUControllers, NVIDIA_VEN, NVIDIA_RTX2080S_DEV, ZOTAC_SUB_VEN, ZOTAC_RTX2080S_SUB_DEV, 0x49);
REGISTER_I2C_PCI_DETECTOR("ZOTAC GAMING GeForce RTX 2080 AMP", DetectZotacTuringGPUControllers, NVIDIA_VEN, NVIDIA_RTX2080_A_DEV, ZOTAC_SUB_VEN, ZOTAC_RTX2080_AMP_SUB_DEV, 0x49);
REGISTER_I2C_PCI_DETECTOR("ZOTAC GAMING GeForce RTX 2080 Ti AMP", DetectZotacTuringGPUControllers, NVIDIA_VEN, NVIDIA_RTX2080TI_A_DEV, ZOTAC_SUB_VEN, ZOTAC_RTX2080_AMP_TI_SUB_DEV, 0x49);
