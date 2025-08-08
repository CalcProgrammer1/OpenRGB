/*---------------------------------------------------------*\
| EVGAGPUv1ControllerDetect.cpp                             |
|                                                           |
|   Detector for EVGA V1 (Pascal) GPU                       |
|                                                           |
|   Adam Honse (CalcProgrammer1)                11 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "EVGAGPUv1Controller.h"
#include "LogManager.h"
#include "RGBController_EVGAGPUv1.h"
#include "i2c_smbus.h"
#include "pci_ids.h"

/******************************************************************************************\
*                                                                                          *
*   DetectEVGAGPUControllers                                                               *
*                                                                                          *
*       Detect EVGA Pascal GPU controllers on the enumerated I2C busses at address 0x49.   *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where EVGA GPU device is connected        *
*           dev - I2C address of EVGA GPU device                                           *
*                                                                                          *
\******************************************************************************************/

void DetectEVGAPascalGPUControllers(i2c_smbus_interface* bus, uint8_t address, const std::string& name)
{
    if(bus->port_id == 1)
    {
        EVGAGPUv1Controller*     controller     = new EVGAGPUv1Controller(bus, address, name);
        RGBController_EVGAGPUv1* rgb_controller = new RGBController_EVGAGPUv1(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectEVGAPascalGPUControllers() */

REGISTER_I2C_PCI_DETECTOR("EVGA GeForce GTX 1070 FTW DT Gaming",    DetectEVGAPascalGPUControllers, NVIDIA_VEN, NVIDIA_GTX1070_DEV,     EVGA_SUB_VEN,   EVGA_GTX1070_FTW_DT_GAMING_SUB_DEV, 0x49);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce GTX 1070 FTW",              DetectEVGAPascalGPUControllers, NVIDIA_VEN, NVIDIA_GTX1070_DEV,     EVGA_SUB_VEN,   EVGA_GTX1070_FTW_SUB_DEV,           0x49);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce GTX 1070 FTW HYBRID",       DetectEVGAPascalGPUControllers, NVIDIA_VEN, NVIDIA_GTX1070_DEV,     EVGA_SUB_VEN,   EVGA_GTX1070_FTW_HYBRID_SUB_DEV,    0x49);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce GTX 1070 Ti FTW2",          DetectEVGAPascalGPUControllers, NVIDIA_VEN, NVIDIA_GTX1070TI_DEV,   EVGA_SUB_VEN,   EVGA_GTX1070TI_FTW2_SUB_DEV,        0x49);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce GTX 1080 FTW",              DetectEVGAPascalGPUControllers, NVIDIA_VEN, NVIDIA_GTX1080_DEV,     EVGA_SUB_VEN,   EVGA_GTX1080_FTW_SUB_DEV,           0x49);
