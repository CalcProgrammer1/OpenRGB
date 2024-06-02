/*---------------------------------------------------------*\
| MSIGPUv2ControllerDetect.cpp                              |
|                                                           |
|   Detector for MSI V2 GPU (ITE9)                          |
|                                                           |
|   Wojciech Lazarski                           03 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "LogManager.h"
#include "i2c_smbus.h"
#include "RGBController_MSIGPUv2.h"
#include "MSIGPUv2Controller.h"

/*-----------------------------------------------------------------------------------------*\
|                                                                                           |
|   DetectMSI GPU V2 Controllers                                                            |
|                                                                                           |
|       Detect MSI GPU v2 controllers on the enumerated I2C busses.                         |
|                                                                                           |
\*-----------------------------------------------------------------------------------------*/

void DetectMSIGPUv2Controllers(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name)
{
    if(bus->pci_vendor == NVIDIA_VEN && bus->port_id != 1)
    {
        return;
    }

        int msi_gpu_id                          = bus->pci_subsystem_device | bus->pci_device << 16;
        MSIGPUv2Controller*     controller      = new MSIGPUv2Controller(bus, i2c_addr);
        RGBController_MSIGPUv2* rgb_controller  = new RGBController_MSIGPUv2(controller, msi_gpu_id);
        rgb_controller->name                    = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
        LOG_TRACE("MSI GPU Controler using ITE9 chip");

} /* DetectMSIGPUv2Controllers() */

REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4060 8GB Gaming X",                      DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4060_DEV,         MSI_SUB_VEN,    MSI_RTX4060_GAMING_X_8G_SUB_DEV,               0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4060 Ti 8GB Gaming X",                   DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4060TI_DEV,       MSI_SUB_VEN,    MSI_RTX4060TI_GAMING_X_8G_SUB_DEV,             0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4060 Ti 16GB Gaming X",                  DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4060TI_16G_DEV,   MSI_SUB_VEN,    MSI_RTX4060TI_GAMING_X_16G_SUB_DEV,            0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4060 Ti 16GB Gaming X Slim White",       DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4060TI_16G_DEV,   MSI_SUB_VEN,    MSI_RTX4060TI_GAMING_X_16G_SLIM_WHITE_SUB_DEV, 0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4070 12GB Gaming X Trio",                DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4070_DEV,         MSI_SUB_VEN,    MSI_RTX4070_GAMING_X_TRIO_SUB_DEV,             0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4070 12GB Gaming X Slim",                DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4070_DEV,         MSI_SUB_VEN,    MSI_RTX4070S_GAMING_X_SLIM_SUB_DEV,            0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4070 SUPER 12GB Gaming X Slim",          DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4070S_DEV,        MSI_SUB_VEN,    MSI_RTX4070S_GAMING_X_SLIM_SUB_DEV,            0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4070 SUPER 12GB Gaming X Slim White",    DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4070S_DEV,        MSI_SUB_VEN,    MSI_RTX4070S_GAMING_X_SLIM_WHITE_SUB_DEV,      0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4070 Ti 12GB Gaming X Trio",             DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4070TI_DEV,       MSI_SUB_VEN,    MSI_RTX4070TI_GAMING_X_TRIO_SUB_DEV,           0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4070 Ti 12GB Gaming X Trio White",       DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4070TI_DEV,       MSI_SUB_VEN,    MSI_RTX4070TI_GAMING_X_TRIO_WHITE_SUB_DEV,     0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4070 Ti 12GB Gaming X Slim White",       DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4070TI_DEV,       MSI_SUB_VEN,    MSI_RTX4070TI_GAMING_X_SLIM_WHITE_SUB_DEV,     0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4070 Ti 12GB Suprim X Trio",             DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4070TI_DEV,       MSI_SUB_VEN,    MSI_RTX4070TI_SUPRIM_X_TRIO_SUB_DEV,           0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4070 Ti SUPER 16GB Gaming X Trio White", DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4070TIS_DEV,      MSI_SUB_VEN,    MSI_RTX4070TI_GAMING_X_TRIO_WHITE_SUB_DEV,     0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4070 Ti SUPER 16GB Gaming Slim",         DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4070TIS_DEV,      MSI_SUB_VEN,    MSI_RTX4070TIS_GAMING_SLIM_SUB_DEV,            0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4080 16GB Gaming X Slim White",          DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4080_DEV,         MSI_SUB_VEN,    MSI_RTX4080S_GAMING_X_SLIM_WHITE_SUB_DEV,      0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4080 16GB Gaming X Trio",                DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4080_DEV,         MSI_SUB_VEN,    MSI_RTX4080_GAMING_X_TRIO_SUB_DEV,             0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4080 16GB Gaming X Trio White",          DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4080_DEV,         MSI_SUB_VEN,    MSI_RTX4080_GAMING_X_TRIO_WHITE_SUB_DEV,       0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4080 16GB Suprim X",                     DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4080_DEV,         MSI_SUB_VEN,    MSI_RTX4080_SUPRIM_X_SUB_DEV,                  0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4080 SUPER 16GB Gaming X Slim",          DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4080S_DEV,        MSI_SUB_VEN,    MSI_RTX4080S_GAMING_X_SLIM_SUB_DEV,            0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4080 SUPER 16GB Gaming X Slim White",    DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4080S_DEV,        MSI_SUB_VEN,    MSI_RTX4080S_GAMING_X_SLIM_WHITE_SUB_DEV,      0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4080 SUPER 16GB Suprim X",               DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4080S_DEV,        MSI_SUB_VEN,    MSI_RTX4080_SUPRIM_X_SUB_DEV,                  0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4090 24GB Gaming X Trio",                DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         MSI_SUB_VEN,    MSI_RTX4090_GAMING_X_TRIO_SUB_DEV,             0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4090 24GB Gaming X Slim",                DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         MSI_SUB_VEN,    MSI_RTX4090_GAMING_X_SLIM_SUB_DEV,             0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4090 24GB Suprim Liquid X",              DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         MSI_SUB_VEN,    MSI_RTX4090_SUPRIM_LIQUID_X_SUB_DEV,           0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4090 24GB Suprim X",                     DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         MSI_SUB_VEN,    MSI_RTX4090_SUPRIM_X_SUB_DEV,                  0x68);
