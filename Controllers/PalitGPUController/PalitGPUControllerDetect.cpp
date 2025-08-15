/*---------------------------------------------------------*\
| PalitGPUControllerDetect.cpp                              |
|                                                           |
|   Detector for Palit GPU                                  |
|                                                           |
|   Manatsawin Hanmongkolchai                   11 Apr 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "Detector.h"
#include "PalitGPUController.h"
#include "RGBController_PalitGPU.h"
#include "i2c_smbus.h"
#include "pci_ids.h"

/******************************************************************************************\
 *                                                                                        *
 *   DetectPalitGPUControllers                                                            *
 *                                                                                        *
 *       Detect Palit GPU controllers on the enumerated I2C busses at address 0x49.       *
 *                                                                                        *
 *       bus - pointer to i2c_smbus_interface where Palit GPU device is connected         *
 *       dev - I2C address of Palit GPU device                                            *
 *                                                                                        *                                                                                       *
 *         Ligolas Neo Malicdem - Added 1060, 1070, 1070ti support                        *
\******************************************************************************************/

void DetectPalitGPUControllers(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name)
{
    if(bus->port_id != 1)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Check for PALIT string                                |
    \*-----------------------------------------------------*/
    const uint8_t palit[] = {'P', 'A', 'L', 'I', 'T'};

    for(size_t i = 0; i < sizeof(palit); i++)
    {
        int32_t letter = bus->i2c_smbus_read_byte_data(i2c_addr, 0x07 + (u8)i);

        if(palit[i] != letter)
        {
            return;
        }
    }

    PalitGPUController*     controller      = new PalitGPUController(bus, i2c_addr, name);
    RGBController_PalitGPU* rgb_controller  = new RGBController_PalitGPU(controller);

    ResourceManager::get()->RegisterRGBController(rgb_controller);
} /* DetectPalitGPUControllers() */

REGISTER_I2C_PCI_DETECTOR("Palit GeForce GTX 1060",    DetectPalitGPUControllers, NVIDIA_VEN, NVIDIA_GTX1060_DEV, NVIDIA_SUB_VEN, NVIDIA_GTX1060_DEV,     0x08);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce GTX 1070",    DetectPalitGPUControllers, NVIDIA_VEN, NVIDIA_GTX1070_DEV, NVIDIA_SUB_VEN, NVIDIA_GTX1070_DEV,     0x08);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce GTX 1070 Ti", DetectPalitGPUControllers, NVIDIA_VEN, NVIDIA_GTX1070TI_DEV, NVIDIA_SUB_VEN, NVIDIA_GTX1070TI_DEV, 0x08);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce GTX 1080",    DetectPalitGPUControllers, NVIDIA_VEN, NVIDIA_GTX1080_DEV, NVIDIA_SUB_VEN, NVIDIA_GTX1080_DEV,     0x08);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce GTX 1080 Ti", DetectPalitGPUControllers, NVIDIA_VEN, NVIDIA_GTX1080TI_DEV, NVIDIA_SUB_VEN, NVIDIA_GTX1080TI_DEV, 0x08);
