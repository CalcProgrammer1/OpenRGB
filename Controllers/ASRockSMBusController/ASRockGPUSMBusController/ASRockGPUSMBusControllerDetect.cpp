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
#include "i2c_intel_gpu.h"
#include "i2c_smbus.h"
#include "pci_ids.h"

DetectedControllers DetectASRockGPUSMBusControllers(i2c_smbus_interface* bus, uint8_t address, const std::string& name)
{
    DetectedControllers detected_controllers;

    /*-----------------------------------------------------*\
    | GPUs register several i2c adapters under the same PCI |
    | device (e.g. gmbus and DisplayPort AUX for the        |
    | outputs on Intel), and all of them carry the card's   |
    | PCI IDs.  Only probe the GPU's own RGB bus so the     |
    | probe packet is never written to a monitor's DDC bus. |
    \*-----------------------------------------------------*/
    if(!is_intel_gpu_i2c_bus(bus) && !is_amd_gpu_i2c_bus(bus))
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

REGISTER_I2C_PCI_DETECTOR("ASRock Radeon RX 7900 XTX Phantom Gaming", DetectASRockGPUSMBusControllers, AMD_GPU_VEN, AMD_NAVI31_DEV, ASROCK_SUB_VEN, 0x5304, ASROCK_GPU_SMBUS_ADDRESS);
REGISTER_I2C_PCI_DETECTOR("ASRock Radeon RX 7900 XTX Taichi 24GB OC", DetectASRockGPUSMBusControllers, AMD_GPU_VEN, AMD_NAVI31_DEV, ASROCK_SUB_VEN, 0x5305, ASROCK_GPU_SMBUS_ADDRESS);
REGISTER_I2C_PCI_DETECTOR("ASRock Radeon RX 7900 XTX Taichi White 24GB OC", DetectASRockGPUSMBusControllers, AMD_GPU_VEN, AMD_NAVI31_DEV, ASROCK_SUB_VEN, 0x5302, ASROCK_GPU_SMBUS_ADDRESS);
REGISTER_I2C_PCI_DETECTOR("ASRock Radeon RX 7900 XT Phantom Gaming 20GB OC", DetectASRockGPUSMBusControllers, AMD_GPU_VEN, AMD_NAVI31_DEV, ASROCK_SUB_VEN, 0x5308, ASROCK_GPU_SMBUS_ADDRESS);
REGISTER_I2C_PCI_DETECTOR("ASRock Radeon RX 9070 XT Steel Legend",   DetectASRockGPUSMBusControllers, AMD_GPU_VEN, AMD_NAVI48_DEV, ASROCK_SUB_VEN, 0x5403, ASROCK_GPU_SMBUS_ADDRESS);
REGISTER_I2C_PCI_DETECTOR("ASRock Radeon RX 9070 XT Steel Legend Dark", DetectASRockGPUSMBusControllers, AMD_GPU_VEN, AMD_NAVI48_DEV, ASROCK_SUB_VEN, 0x5414, ASROCK_GPU_SMBUS_ADDRESS);
REGISTER_I2C_PCI_DETECTOR("ASRock Radeon RX 9070 XT Taichi 16GB OC", DetectASRockGPUSMBusControllers, AMD_GPU_VEN, AMD_NAVI48_DEV, ASROCK_SUB_VEN, 0x5401, ASROCK_GPU_SMBUS_ADDRESS);
REGISTER_I2C_PCI_DETECTOR("ASRock Radeon RX 9060 XT Steel Legend",   DetectASRockGPUSMBusControllers, AMD_GPU_VEN, AMD_NAVI44_DEV, ASROCK_SUB_VEN, 0x5407, ASROCK_GPU_SMBUS_ADDRESS);
REGISTER_I2C_PCI_DETECTOR("ASRock Intel Arc B580 Steel Legend 12GB OC", DetectASRockGPUSMBusControllers, INTEL_VEN, INTEL_ARC_B580_DEV, ASROCK_SUB_VEN, 0x6020, ASROCK_GPU_SMBUS_ADDRESS);
