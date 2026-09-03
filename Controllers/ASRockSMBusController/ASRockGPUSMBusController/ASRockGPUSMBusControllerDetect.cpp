/*---------------------------------------------------------*\
| ASRockGPUSMBusControllerDetect.cpp                        |
|                                                           |
|   Detector for ASRock GPU RGB controllers                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "ASRockGPUSMBusController.h"
#include "LogManager.h"
#include "RGBController_ASRockGPUSMBus.h"
#include "i2c_amd_gpu.h"
#include "i2c_smbus.h"
#include "pci_ids.h"

DetectedControllers DetectASRockGPUSMBusControllers(i2c_smbus_interface* bus, uint8_t address, const std::string& name)
{
    DetectedControllers detected_controllers;

    if(bus->info.pci_vendor == AMD_GPU_VEN && !is_amd_gpu_i2c_bus(bus))
    {
        return(detected_controllers);
    }

    LOG_DEBUG("[ASRock GPU] Checking GPU I2C bus for RGB controller at 0x%02X", address);

    ASRockGPUSMBusController* controller = new ASRockGPUSMBusController(bus, address, name);

    if(controller->DetectChannels())
    {
        detected_controllers.push_back(new RGBController_ASRockGPUSMBus(controller));
    }
    else
    {
        LOG_WARNING("[ASRock GPU] No active channels found, device not registered");
        delete controller;
    }

    return(detected_controllers);
}

REGISTER_I2C_PCI_DETECTOR("ASRock Radeon RX 9070 XT Steel Legend", DetectASRockGPUSMBusControllers, AMD_GPU_VEN, AMD_NAVI48_DEV, ASROCK_SUB_VEN, 0x5403, ASROCK_GPU_SMBUS_ADDRESS);
REGISTER_I2C_PCI_DETECTOR("ASRock Radeon RX 9060 XT Steel Legend", DetectASRockGPUSMBusControllers, AMD_GPU_VEN, AMD_NAVI44_DEV, ASROCK_SUB_VEN, 0x5407, ASROCK_GPU_SMBUS_ADDRESS);
