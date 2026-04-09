/*---------------------------------------------------------*\
| ZotacBlackwellGPUControllerDetect.cpp                     |
|                                                           |
|   Detector for ZOTAC Blackwell (RTX 50 series) GPU        |
|                                                           |
|   Eder Sánchez                                27 Mar 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "ZotacBlackwellGPUController.h"
#include "RGBController_ZotacBlackwellGPU.h"
#include "i2c_smbus.h"
#include "pci_ids.h"

/******************************************************************************************\
*                                                                                          *
*   DetectZotacBlackwellGPUControllersPCI                                                  *
*                                                                                          *
*       Detect ZOTAC Blackwell (RTX 50 series) RGB controllers on the enumerated           *
*       I2C busses at address 0x4B.                                                        *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where RGB device is connected             *
*           dev - I2C address of RGB device                                                *
*                                                                                          *
\******************************************************************************************/

void DetectZotacBlackwellGPUControllersPCI(i2c_smbus_interface* bus, u8 i2c_addr, const std::string& name)
{
    s32 result = bus->i2c_smbus_read_byte_data(i2c_addr, 0x10);

    if(result >= 0)
    {
        ZotacBlackwellGPUController*     controller     = new ZotacBlackwellGPUController(bus, i2c_addr, name);
        RGBController_ZotacBlackwellGPU* rgb_controller = new RGBController_ZotacBlackwellGPU(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_I2C_PCI_DETECTOR("ZOTAC GAMING GeForce RTX 5080 AMP Extreme INFINITY",  DetectZotacBlackwellGPUControllersPCI, NVIDIA_VEN, NVIDIA_RTX5080_DEV, ZOTAC_SUB_VEN, ZOTAC_RTX5080_AMP_EXTREME_SUB_DEV, 0x4B);
