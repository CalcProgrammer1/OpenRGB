/*---------------------------------------------------------*\
| PNYLovelaceGPUControllerDetect.cpp                        |
|                                                           |
|   Detector for PNY Lovelace GPU                           |
|                                                           |
|   yufan                                       01 Oct 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "PNYLovelaceGPUController.h"
#include "RGBController_PNYLovelaceGPU.h"
#include "i2c_smbus.h"
#include "pci_ids.h"

/*-----------------------------------------------------------------------------------------*\
| DetectPNYLovelaceGPUControllers                                                           |
|                                                                                           |
|     Detect PNY 40xx GPU controllers on the enumerated I2C busses at address 0x60.         |
|                                                                                           |
|     bus - pointer to i2c_smbus_interface where PNY GPU device is connected                |
|     dev - I2C address of PNY GPU device                                                   |
\*-----------------------------------------------------------------------------------------*/

void DetectPNYLovelaceGPUControllers(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name)
{
    if(bus->port_id != 1)
    {
        return;
    }

    PNYLovelaceGPUController*     controller        = new PNYLovelaceGPUController(bus, i2c_addr, name);
    RGBController_PNYLovelaceGPU* rgb_controller    = new RGBController_PNYLovelaceGPU(controller);

    ResourceManager::get()->RegisterRGBController(rgb_controller);
} /* DetectPNYLovelaceGPUControllers() */

REGISTER_I2C_PCI_DETECTOR("PNY GeForce RTX 4070 Ti XLR8 VERTO Epic-X",      DetectPNYLovelaceGPUControllers,    NVIDIA_VEN, NVIDIA_RTX4070TI_DEV,   PNY_SUB_VEN,  PNY_RTX_4070TI_XLR8_VERTO_EPIC_X_SUB_DEV, 0x60);
REGISTER_I2C_PCI_DETECTOR("PNY GeForce RTX 4070 Ti XLR8 VERTO REV1",        DetectPNYLovelaceGPUControllers,    NVIDIA_VEN, NVIDIA_RTX4070TI_DEV,   PNY_SUB_VEN,  PNY_RTX_4070TI_XLR8_VERTO_REV1_SUB_DEV,   0x60);
REGISTER_I2C_PCI_DETECTOR("PNY GeForce RTX 4070 Ti XLR8 VERTO REV2",        DetectPNYLovelaceGPUControllers,    NVIDIA_VEN, NVIDIA_RTX4070TI_DEV,   PNY_SUB_VEN,  PNY_RTX_4070TI_XLR8_VERTO_REV2_SUB_DEV,   0x60);
REGISTER_I2C_PCI_DETECTOR("PNY GeForce RTX 4070 Ti XLR8 VERTO OC",          DetectPNYLovelaceGPUControllers,    NVIDIA_VEN, NVIDIA_RTX4070TI_DEV,   PNY_SUB_VEN,  PNY_RTX_4070TI_XLR8_VERTO_OC_SUB_DEV,     0x60);
REGISTER_I2C_PCI_DETECTOR("PNY GeForce RTX 4070 Ti Super XLR8 VERTO OC",    DetectPNYLovelaceGPUControllers,    NVIDIA_VEN, NVIDIA_RTX4070TIS_DEV,   PNY_SUB_VEN,  PNY_RTX_4070TIS_XLR8_VERTO_OC_SUB_DEV,     0x60);
REGISTER_I2C_PCI_DETECTOR("PNY GeForce RTX 4080 XLR8 UPRISING",             DetectPNYLovelaceGPUControllers,    NVIDIA_VEN, NVIDIA_RTX4080_DEV,     PNY_SUB_VEN,  PNY_RTX_4080_XLR8_UPRISING_SUB_DEV,       0x60);
REGISTER_I2C_PCI_DETECTOR("PNY GeForce RTX 4080 XLR8 VERTO",                DetectPNYLovelaceGPUControllers,    NVIDIA_VEN, NVIDIA_RTX4080_DEV,     PNY_SUB_VEN,  PNY_RTX_4080_XLR8_VERTO_SUB_DEV,          0x60);
REGISTER_I2C_PCI_DETECTOR("PNY GeForce RTX 4080 SUPER XLR8 VERTO",          DetectPNYLovelaceGPUControllers,    NVIDIA_VEN, NVIDIA_RTX4080S_DEV,    PNY_SUB_VEN,  PNY_RTX_4080S_XLR8_VERTO_SUB_DEV,         0x60);
REGISTER_I2C_PCI_DETECTOR("PNY GeForce RTX 4080 XLR8 Verto Epic-X",         DetectPNYLovelaceGPUControllers,    NVIDIA_VEN, NVIDIA_RTX4080_DEV,     PNY_SUB_VEN,  PNY_RTX_4080_XLR8_VERTO_EPIC_X_SUB_DEV,   0x60);
REGISTER_I2C_PCI_DETECTOR("PNY GeForce RTX 4090 XLR8 VERTO",                DetectPNYLovelaceGPUControllers,    NVIDIA_VEN, NVIDIA_RTX4090_DEV,     PNY_SUB_VEN,  PNY_RTX_4090_XLR8_VERTO_SUB_DEV,          0x60);
REGISTER_I2C_PCI_DETECTOR("PNY GeForce RTX 4090 XLR8 Verto Epic-X",         DetectPNYLovelaceGPUControllers,    NVIDIA_VEN, NVIDIA_RTX4090_DEV,     PNY_SUB_VEN,  PNY_RTX_4090_VERTO_EPIC_X_SUB_DEV,        0x60);
REGISTER_I2C_PCI_DETECTOR("PNY GeForce RTX 4090 XLR8 Verto Epic-X OC",      DetectPNYLovelaceGPUControllers,    NVIDIA_VEN, NVIDIA_RTX4090_DEV,     PNY_SUB_VEN,  PNY_RTX_4090_VERTO_EPIC_X_OC_SUB_DEV,     0x60);
