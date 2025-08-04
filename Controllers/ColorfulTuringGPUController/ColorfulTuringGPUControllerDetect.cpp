/*---------------------------------------------------------*\
| ColorfulTuringGPUControllerDetect.cpp                     |
|                                                           |
|   Driver for Colorful Turing GPU                          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "ColorfulTuringGPUController.h"
#include "RGBController_ColorfulTuringGPU.h"
#include "i2c_smbus.h"
#include "pci_ids.h"

void DetectColorfulTuringGPUControllers(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name)
{
    if(bus->port_id == 1)
    {
        ColorfulTuringGPUController* controller         = new ColorfulTuringGPUController(bus, i2c_addr, name);
        RGBController_ColorfulTuringGPU* rgb_controller = new RGBController_ColorfulTuringGPU(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_I2C_PCI_DETECTOR("iGame GeForce RTX 2070 SUPER Advanced OC-V", DetectColorfulTuringGPUControllers,   NVIDIA_VEN, NVIDIA_RTX2070S_OC_DEV,   COLORFUL_SUB_VEN,   COLORFUL_IGAME_RTX_2070_SUPER_ADVANCED_OCV,  0x50);
REGISTER_I2C_PCI_DETECTOR("iGame GeForce RTX 2070 SUPER Advanced OC-V", DetectColorfulTuringGPUControllers,   NVIDIA_VEN, NVIDIA_RTX2070S_OC_DEV,   COLORFUL_SUB_VEN,   COLORFUL_IGAME_RTX_2070_SUPER_ADVANCED_OCV2, 0x50);
