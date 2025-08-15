/*---------------------------------------------------------*\
| PowerColorRedDevilGPUControllerDetect.cpp                 |
|                                                           |
|   Driver for PowerColor Red Devil GPU                     |
|                                                           |
|   Jana Rettig (SapphicKitten)                 14 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "pci_ids.h"
#include "RGBController_PowerColorRedDevilGPU.h"
#include "PowerColorRedDevilGPUController.h"

static const unsigned char indicator[3] = {0x01, 0x05, 0x00};

void DetectPowerColorRedDevilGPUControllers(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name)
{
    unsigned char data[3] = {0};
    int ret = bus->i2c_smbus_read_i2c_block_data(i2c_addr, 0x90, 3, data);

    if(ret == 3 && memcmp(data, indicator, 3) == 0)
    {
        RedDevilGPUController*     controller     = new RedDevilGPUController(bus, i2c_addr, name);
        RGBController_RedDevilGPU* rgb_controller = new RGBController_RedDevilGPU(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_I2C_PCI_DETECTOR("PowerColor Red Devil RX5700",            DetectPowerColorRedDevilGPUControllers, AMD_GPU_VEN, AMD_NAVI10_DEV, POWERCOLOR_SUB_VEN, POWERCOLOR_RED_DEVIL_RX5700_SUB_DEV,               0x22);
REGISTER_I2C_PCI_DETECTOR("PowerColor Red Devil RX5700XT",          DetectPowerColorRedDevilGPUControllers, AMD_GPU_VEN, AMD_NAVI10_DEV, POWERCOLOR_SUB_VEN, POWERCOLOR_RED_DEVIL_RX5700XT_SUB_DEV,             0x22);
REGISTER_I2C_PCI_DETECTOR("PowerColor Red Devil RX6750XT",          DetectPowerColorRedDevilGPUControllers, AMD_GPU_VEN, AMD_NAVI22_DEV,  POWERCOLOR_SUB_VEN, POWERCOLOR_RED_DEVIL_RX6750XT_SUB_DEV,            0x22);
REGISTER_I2C_PCI_DETECTOR("PowerColor Red Devil RX6800XT",          DetectPowerColorRedDevilGPUControllers, AMD_GPU_VEN, AMD_NAVI21_DEV1, POWERCOLOR_SUB_VEN, POWERCOLOR_RED_DEVIL_RX6800XT_SUB_DEV,            0x22);
REGISTER_I2C_PCI_DETECTOR("PowerColor Red Devil RX6900XT Ultimate", DetectPowerColorRedDevilGPUControllers, AMD_GPU_VEN, AMD_NAVI21_DEV2, POWERCOLOR_SUB_VEN, POWERCOLOR_RED_DEVIL_RX6900XT_ULTIMATE_SUB_DEV,   0x22);
