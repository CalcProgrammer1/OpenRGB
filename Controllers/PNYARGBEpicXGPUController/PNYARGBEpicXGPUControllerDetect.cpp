/*---------------------------------------------------------*\
| PNYARGBEpicXGPUControllerDetect.cpp                       |
|                                                           |
|   Detector for PNY ARGB Epic-X GPU                        |
|                                                           |
|   Peter Berendi                               27 Apr 2025 |
|   Adam Honse <calcprogrammer1@gmail.com>      01 Aug 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "LogManager.h"
#include "PNYARGBEpicXGPUController.h"
#include "RGBController_PNYARGBEpicXGPU.h"
#include "i2c_smbus.h"
#include "pci_ids.h"

/*-----------------------------------------------------------------------------------------*\
| DetectPNYARGBEpicXGPUControllers                                                          |
|                                                                                           |
|     Detect PNY ARGB Epic X GPU controllers on the enumerated I2C busses at address 0x60.  |
|                                                                                           |
|     bus - pointer to i2c_smbus_interface where PNY GPU device is connected                |
|     dev - I2C address of PNY GPU device                                                   |
\*-----------------------------------------------------------------------------------------*/

void DetectPNYARGBEpicXGPUControllers(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name)
{
    PNYARGBEpicXGPUController*     controller     = new PNYARGBEpicXGPUController(bus, i2c_addr, name);
    RGBController_PNYARGBEpicXGPU* rgb_controller = new RGBController_PNYARGBEpicXGPU(controller);

    ResourceManager::get()->RegisterRGBController(rgb_controller);
} /* DetectPNYARGBEpicXGPUControllers() */

REGISTER_I2C_PCI_DETECTOR("PNY GeForce RTX 5070Ti ARGB Epic-X OC", DetectPNYARGBEpicXGPUControllers, NVIDIA_VEN, NVIDIA_RTX5070TI_DEV, PNY_SUB_VEN, PNY_RTX_5070TI_ARGB_EPIC_X_OC_SUB_DEV, 0x60);
//REGISTER_I2C_PCI_DETECTOR("PNY GeForce RTX 5080 ARGB Epic-X OC",   DetectPNYARGBEpicXGPUControllers, NVIDIA_VEN, NVIDIA_RTX5080_DEV,   PNY_SUB_VEN, PNY_RTX_5080_ARGB_EPIC_X_OC_SUB_DEV,   0x60);
//REGISTER_I2C_PCI_DETECTOR("PNY GeForce RTX 5090 ARGB Epic-X OC",   DetectPNYARGBEpicXGPUControllers, NVIDIA_VEN, NVIDIA_RTX5090_DEV,   PNY_SUB_VEN, PNY_RTX_5090_ARGB_EPIC_X_OC_SUB_DEV,   0x60);
