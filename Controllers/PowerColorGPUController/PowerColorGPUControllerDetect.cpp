/*---------------------------------------------------------*\
| PowerColorGPUControllerDetect.cpp                         |
|                                                           |
|   Driver for PowerColor GPUs                              |
|                                                           |
|   Nexrem                                      15 Aug 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "pci_ids.h"
#include "i2c_amd_gpu.h"
#include "PowerColorRedDevilV1Controller/RGBController_PowerColorRedDevilV1.h"
#include "PowerColorRedDevilV1Controller/PowerColorRedDevilV1Controller.h"
#include "PowerColorRedDevilV2Controller/RGBController_PowerColorRedDevilV2.h"
#include "PowerColorRedDevilV2Controller/PowerColorRedDevilV2Controller.h"

static const unsigned char magic_v1[3] = {0x01, 0x05, 0x00};
static const unsigned char magic_v2[3] = {0x01, 0x32, 0x00};

/*---------------------------------------------------------*\
| The controller reports a unique identifier for V1 and V2. |
| Unfortunately they are on different addresses. Read it    |
| for good measure anyways.                                 |
\*---------------------------------------------------------*/

void DetectPowerColorRedDevilGPUControllersV1(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name)
{
    if(!is_amd_gpu_i2c_bus(bus))
    {
        return;
    }

    unsigned char data[3];
    int ret = bus->i2c_smbus_read_i2c_block_data(i2c_addr, RED_DEVIL_V1_REG_MAGIC, 3, data);
    if(ret == 3 && memcmp(data, magic_v1, 3) == 0)
    {
        PowerColorRedDevilV1Controller* controller         = new PowerColorRedDevilV1Controller(bus, i2c_addr, name);
        RGBController_PowerColorRedDevilV1* rgb_controller = new RGBController_PowerColorRedDevilV1(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}


void DetectPowerColorRedDevilGPUControllersV2(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name)
{
    if(!is_amd_gpu_i2c_bus(bus))
    {
        return;
    }

    unsigned char data[3];
    int ret = bus->i2c_smbus_read_i2c_block_data(i2c_addr, RED_DEVIL_V2_READ_REG_MAGIC, 3, data);
    if(ret == 3 && memcmp(data, magic_v2, 3) == 0)
    {
        PowerColorRedDevilV2Controller* controller         = new PowerColorRedDevilV2Controller(bus, i2c_addr, name);
        RGBController_PowerColorRedDevilV2* rgb_controller = new RGBController_PowerColorRedDevilV2(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_I2C_PCI_DETECTOR("PowerColor Red Devil RX5700",            DetectPowerColorRedDevilGPUControllersV1, AMD_GPU_VEN, AMD_NAVI10_DEV,  POWERCOLOR_SUB_VEN, POWERCOLOR_RED_DEVIL_RX5700_SUB_DEV,                0x22);
REGISTER_I2C_PCI_DETECTOR("PowerColor Red Devil RX5700XT",          DetectPowerColorRedDevilGPUControllersV1, AMD_GPU_VEN, AMD_NAVI10_DEV,  POWERCOLOR_SUB_VEN, POWERCOLOR_RED_DEVIL_RX5700XT_SUB_DEV,              0x22);
REGISTER_I2C_PCI_DETECTOR("PowerColor Red Devil RX6750XT",          DetectPowerColorRedDevilGPUControllersV1, AMD_GPU_VEN, AMD_NAVI22_DEV,  POWERCOLOR_SUB_VEN, POWERCOLOR_RED_DEVIL_RX6750XT_SUB_DEV,              0x22);
REGISTER_I2C_PCI_DETECTOR("PowerColor Red Devil RX6800XT",          DetectPowerColorRedDevilGPUControllersV1, AMD_GPU_VEN, AMD_NAVI21_DEV1, POWERCOLOR_SUB_VEN, POWERCOLOR_RED_DEVIL_RX6800XT_SUB_DEV,              0x22);
REGISTER_I2C_PCI_DETECTOR("PowerColor Red Devil RX6900XT",          DetectPowerColorRedDevilGPUControllersV1, AMD_GPU_VEN, AMD_NAVI21_DEV1, POWERCOLOR_SUB_VEN, POWERCOLOR_RED_DEVIL_RX6900XT_SUB_DEV,              0x22);
REGISTER_I2C_PCI_DETECTOR("PowerColor Red Devil RX6900XT",          DetectPowerColorRedDevilGPUControllersV1, AMD_GPU_VEN, AMD_NAVI21_DEV2, POWERCOLOR_SUB_VEN, POWERCOLOR_RED_DEVIL_RX6900XT_SUB_DEV,              0x22);
REGISTER_I2C_PCI_DETECTOR("PowerColor Red Devil RX6900XT Ultimate", DetectPowerColorRedDevilGPUControllersV1, AMD_GPU_VEN, AMD_NAVI21_DEV2, POWERCOLOR_SUB_VEN, POWERCOLOR_RED_DEVIL_RX6900XT_ULTIMATE_SUB_DEV,     0x22);

REGISTER_I2C_PCI_DETECTOR("PowerColor Red Devil RX9070XT",          DetectPowerColorRedDevilGPUControllersV2, AMD_GPU_VEN, AMD_NAVI48_DEV,  POWERCOLOR_SUB_VEN, POWERCOLOR_RED_DEVIL_RX9070XT_SUB_DEV,              0x22);
