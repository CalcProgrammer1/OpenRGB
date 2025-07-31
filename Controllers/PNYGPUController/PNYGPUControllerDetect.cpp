/*---------------------------------------------------------*\
| PNYGPUControllerDetect.cpp                                |
|                                                           |
|   Detector for PNY Turing GPU                             |
|                                                           |
|   KendallMorgan                               17 Oct 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "PNYGPUController.h"
#include "RGBController_PNYGPU.h"
#include "i2c_smbus.h"
#include "pci_ids.h"

/******************************************************************************************\
 *                                                                                        *
 *   DetectPNYGPUControllers                                                              *
 *                                                                                        *
 *       Detect PNY GPU controllers on the enumerated I2C busses at address 0x49.         *
 *                                                                                        *
 *       bus - pointer to i2c_smbus_interface where PNY GPU device is connected           *
 *       dev - I2C address of PNY GPU device                                              *
 *                                                                                        *
\******************************************************************************************/

void DetectPNYGPUControllers(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name)
{
    if(bus->port_id != 1)
    {
        return;
    }

    PNYGPUController*     controller        = new PNYGPUController(bus, i2c_addr, name);
    RGBController_PNYGPU* rgb_controller    = new RGBController_PNYGPU(controller);

    ResourceManager::get()->RegisterRGBController(rgb_controller);
} /* DetectPNYGPUControllers() */

REGISTER_I2C_PCI_DETECTOR("PNY GeForce RTX 2060 XLR8 OC EDITION",       DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX2060_TU104_DEV,   PNY_SUB_VEN,    PNY_RTX_2060_XLR8_OC_SUB_DEV,               0x49);
REGISTER_I2C_PCI_DETECTOR("PNY GeForce RTX 3060 XLR8 Revel EPIC-X",     DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3060_DEV,         PNY_SUB_VEN,    PNY_RTX_3060_XLR8_REVEL_EPIC_X_SUB_DEV,     0x49);
REGISTER_I2C_PCI_DETECTOR("PNY GeForce RTX 3070 XLR8 Revel EPIC-X",     DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3070_DEV,         PNY_SUB_VEN,    PNY_RTX_3070_XLR8_REVEL_EPIC_X_SUB_DEV,     0x49);
REGISTER_I2C_PCI_DETECTOR("PNY GeForce RTX 3070 XLR8 Revel EPIC-X LHR", DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3070_LHR_DEV,     PNY_SUB_VEN,    PNY_RTX_3070_XLR8_REVEL_EPIC_X_LHR_SUB_DEV, 0x49);
REGISTER_I2C_PCI_DETECTOR("PNY GeForce RTX 3080 XLR8 Revel EPIC-X",     DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3080_DEV,         PNY_SUB_VEN,    PNY_RTX_3080_XLR8_REVEL_EPIC_X_SUB_DEV,     0x49);
REGISTER_I2C_PCI_DETECTOR("PNY GeForce RTX 3080 Ti XLR8 Revel EPIC-X",  DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3080TI_DEV,       PNY_SUB_VEN,    PNY_RTX_3080TI_XLR8_REVEL_EPIC_X_SUB_DEV,   0x49);
REGISTER_I2C_PCI_DETECTOR("PNY GeForce RTX 3090 XLR8 Revel EPIC-X",     DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3090_DEV,         PNY_SUB_VEN,    PNY_RTX_3090_XLR8_REVEL_EPIC_X_SUB_DEV,     0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 2070 SUPER Jetstream",     DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX2070S_OC_DEV,     NVIDIA_SUB_VEN, NVIDIA_RTX2070S_OC_DEV,                     0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3060",                     DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3060_DEV,         PALIT_SUB_VEN,  PALIT_RTX3060_SUB_DEV,                      0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3060 LHR",                 DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3060_LHR_DEV,     PALIT_SUB_VEN,  PALIT_RTX3060_LHR_SUB_DEV,                  0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3060 Ti",                  DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3060TI_DEV,       PALIT_SUB_VEN,  PALIT_RTX3060TI_SUB_DEV,                    0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3060 Ti LHR",              DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3060TI_LHR_DEV,   PALIT_SUB_VEN,  NVIDIA_RTX3060TI_LHR_DEV,                   0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3060 Ti Dual",             DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3060TI_DEV,       NVIDIA_SUB_VEN, PALIT_RTX3060TI_SUB_DEV,                    0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3070",                     DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3070_DEV,         PALIT_SUB_VEN,  PALIT_RTX3070_SUB_DEV,                      0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3070 LHR",                 DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3070_LHR_DEV,     PALIT_SUB_VEN,  PALIT_RTX3070_LHR_SUB_DEV,                  0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3070 Gamerock LHR",        DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3070_LHR_DEV,     PALIT_SUB_VEN,  PALIT_RTX3070_GAMEROCK_SUB_DEV,             0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3070 Ti",                  DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3070TI_DEV,       PALIT_SUB_VEN,  PALIT_RTX3070TI_SUB_DEV,                    0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3070 Ti GamingPro",        DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3070TI_DEV,       PALIT_SUB_VEN,  PALIT_RTX3070TI_GAMING_PRO_SUB_DEV,         0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3080",                     DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3080_DEV,         PALIT_SUB_VEN,  PALIT_RTX3080_SUB_DEV,                      0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3080 LHR",                 DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3080_LHR_DEV,     PALIT_SUB_VEN,  PALIT_RTX3080_LHR_SUB_DEV,                  0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3080 Gamerock",            DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3080_DEV,         PALIT_SUB_VEN,  PALIT_RTX3080_GAMEROCK_SUB_DEV,             0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3080 Gamerock LHR",        DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3080_LHR_DEV,     PALIT_SUB_VEN,  PALIT_RTX3080_GAMEROCK_SUB_DEV,             0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3080 12GB GamingPro",      DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3080_12G_LHR_DEV, PALIT_SUB_VEN,  PALIT_RTX3080_GAMINGPRO_12G_SUB_DEV,        0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3080 Ti",                  DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3080TI_DEV,       PALIT_SUB_VEN,  PALIT_RTX3080TI_SUB_DEV,                    0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3080 Ti Gamerock",         DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3080TI_DEV,       PALIT_SUB_VEN,  PALIT_RTX3080TI_GAMEROCK_SUB_DEV,           0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3090",                     DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3090_DEV,         PALIT_SUB_VEN,  PALIT_RTX3090_SUB_DEV,                      0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3090 Gamerock",            DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3090_DEV,         PALIT_SUB_VEN,  PALIT_RTX3090_GAMEROCK_SUB_DEV,             0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 4070 Ti",                  DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX4070TI_DEV,       PALIT_SUB_VEN,  PALIT_RTX4070TI_SUB_DEV,                    0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 4070 Ti Gamerock",         DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX4070TI_DEV,       PALIT_SUB_VEN,  PALIT_RTX4070TI_GAMEROCK_SUB_DEV,           0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 4070 Ti SUPER GamingPro",  DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX4070TIS_DEV,      PALIT_SUB_VEN,  PALIT_RTX4080_GAMINGPRO_SUB_DEV,            0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 4080 GamingPro",           DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX4080_DEV,         PALIT_SUB_VEN,  PALIT_RTX4080_GAMINGPRO_SUB_DEV,            0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 4080 SUPER GamingPro",     DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX4080S_DEV,        PALIT_SUB_VEN,  PALIT_RTX4080_GAMINGPRO_SUB_DEV,            0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 4090 Gamerock",            DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX4090_DEV,         PALIT_SUB_VEN,  PALIT_RTX4090_GAMEROCK_SUB_DEV,             0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 5070 GamingPro",           DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX5070_DEV,         PALIT_SUB_VEN,  PALIT_RTX5070_GAMINGPRO_SUB_DEV,            0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 5090 Gamerock",            DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX5090_DEV,         PALIT_SUB_VEN,  PALIT_RTX5090_GAMEROCK_SUB_DEV,             0x49);
REGISTER_I2C_PCI_DETECTOR("NVIDIA GeForce RTX 2060 SUPER",              DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX2060_TU106_DEV,   NVIDIA_SUB_VEN, NVIDIA_RTX2060_TU106_DEV,                   0x49);
REGISTER_I2C_PCI_DETECTOR("NVIDIA GeForce RTX 2080 SUPER",              DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX2080S_DEV,        NVIDIA_SUB_VEN, NVIDIA_RTX2080S_DEV,                        0x49);
