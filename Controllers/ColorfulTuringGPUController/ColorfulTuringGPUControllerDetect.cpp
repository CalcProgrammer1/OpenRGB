﻿#include "Detector.h"
#include "ColorfulTuringGPUController.h"
#include "RGBController.h"
#include "RGBController_ColorfulTuringGPU.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

void DetectColorfulTuringGPUControllers(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name)
{
    if(bus->port_id == 1)
    {
        ColorfulTuringGPUController* controller         = new ColorfulTuringGPUController(bus, i2c_addr);
        RGBController_ColorfulTuringGPU* rgb_controller = new RGBController_ColorfulTuringGPU(controller);
        rgb_controller->name                            = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_I2C_PCI_DETECTOR("iGame GeForce RTX 2070 SUPER Advanced OC-V", DetectColorfulTuringGPUControllers,   NVIDIA_VEN, NVIDIA_RTX2070S_OC_DEV,   COLORFUL_SUB_VEN,   COLORFUL_IGAME_RTX_2070_SUPER_ADVANCED_OCV, 0x50);
