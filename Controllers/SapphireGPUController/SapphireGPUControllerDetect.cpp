/*---------------------------------------------------------*\
| SapphireGPUControllerDetect.cpp                           |
|                                                           |
|   Detector for Sapphire Nitro Glow                        |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "SapphireNitroGlowV1Controller.h"
#include "SapphireNitroGlowV3Controller.h"
#include "RGBController_SapphireNitroGlowV1.h"
#include "RGBController_SapphireNitroGlowV3.h"
#include "i2c_smbus.h"
#include "pci_ids.h"

/*-----------------------------------------------------*\
| I2C Addresses for Sapphire Nitro Glow RGB             |
\*-----------------------------------------------------*/
enum
{
    SAPPHIRE_NITRO_GLOW_V1_ADDR = 0x55,
    SAPPHIRE_NITRO_GLOW_V3_ADDR = 0x28,
};

/******************************************************************************************\
*                                                                                          *
*   TestForSapphireGPUController                                                           *
*                                                                                          *
*       Tests the given address to see if an Sapphire controller exists there.  First      *
*       does a byte read to test for a response                                            *
*                                                                                          *
\******************************************************************************************/

bool TestForSapphireGPUController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;
    int res;

    //Read a byte to test for presence
    res = bus->i2c_smbus_read_byte(address);

    if (res >= 0)
    {
        pass = true;
    }

    return(pass);

}   /* TestForSapphireGPUController() */

/******************************************************************************************\
*                                                                                          *
*   DetectSapphireGPUControllers                                                           *
*                                                                                          *
*       Detect Sapphire GPU controllers on the enumerated I2C buses.                       *
*                                                                                          *
\******************************************************************************************/

void DetectSapphireV1Controllers(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name)
{
    if(TestForSapphireGPUController(bus, i2c_addr))
    {
        SapphireNitroGlowV1Controller*     new_sapphire_gpu = new SapphireNitroGlowV1Controller(bus, i2c_addr, name);
        RGBController_SapphireNitroGlowV1* new_controller   = new RGBController_SapphireNitroGlowV1(new_sapphire_gpu);

        ResourceManager::get()->RegisterRGBController(new_controller);
    }
}   /* DetectSapphireV1Controllers() */

void DetectSapphireV3Controllers(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name)
{
    if(TestForSapphireGPUController(bus, i2c_addr))
    {
        SapphireNitroGlowV3Controller*     new_sapphire_gpu = new SapphireNitroGlowV3Controller(bus, i2c_addr, name);
        RGBController_SapphireNitroGlowV3* new_controller   = new RGBController_SapphireNitroGlowV3(new_sapphire_gpu);

        ResourceManager::get()->RegisterRGBController(new_controller);
    }
}   /* DetectSapphireV3Controllers() */

REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX 470/480 Nitro+",                     DetectSapphireV1Controllers,    AMD_GPU_VEN,    AMD_POLARIS_DEV,        SAPPHIRE_LEGACY_SUB_VEN,    SAPPHIRE_LEGACY_POLARIS_NITRO_PLUS_SUB_DEV,     SAPPHIRE_NITRO_GLOW_V1_ADDR);
REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX 570/580/590 Nitro+",                 DetectSapphireV1Controllers,    AMD_GPU_VEN,    AMD_POLARIS_DEV,        SAPPHIRE_SUB_VEN,           SAPPHIRE_POLARIS_NITRO_PLUS_SUB_DEV1,           SAPPHIRE_NITRO_GLOW_V1_ADDR);
REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX 570/580/590 Nitro+",                 DetectSapphireV1Controllers,    AMD_GPU_VEN,    AMD_POLARIS_DEV,        SAPPHIRE_SUB_VEN,           SAPPHIRE_POLARIS_NITRO_PLUS_SUB_DEV2,           SAPPHIRE_NITRO_GLOW_V1_ADDR);
REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX 580 Nitro+ (2048SP)",                DetectSapphireV1Controllers,    AMD_GPU_VEN,    AMD_POLARIS20XL_DEV,    SAPPHIRE_SUB_VEN,           SAPPHIRE_POLARIS_NITRO_PLUS_SUB_DEV1,           SAPPHIRE_NITRO_GLOW_V1_ADDR);
REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX Vega 56/64 Nitro+",                  DetectSapphireV1Controllers,    AMD_GPU_VEN,    AMD_VEGA10_DEV,         SAPPHIRE_SUB_VEN,           SAPPHIRE_VEGA10_NITRO_PLUS_SUB_DEV,             SAPPHIRE_NITRO_GLOW_V1_ADDR);
REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX 5500 XT Nitro+",                     DetectSapphireV3Controllers,    AMD_GPU_VEN,    AMD_NAVI14_DEV,         SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI14_NITRO_PLUS_SUB_DEV,             SAPPHIRE_NITRO_GLOW_V3_ADDR);
REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX 5700 XT Nitro+",                     DetectSapphireV3Controllers,    AMD_GPU_VEN,    AMD_NAVI10_DEV,         SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI10_NITRO_PLUS_SUB_DEV1,            SAPPHIRE_NITRO_GLOW_V3_ADDR);
REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX 5700 XT Nitro+",                     DetectSapphireV3Controllers,    AMD_GPU_VEN,    AMD_NAVI10_DEV,         SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI10_NITRO_PLUS_SUB_DEV2,            SAPPHIRE_NITRO_GLOW_V3_ADDR);
REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX 5700 XT Nitro+",                     DetectSapphireV3Controllers,    AMD_GPU_VEN,    AMD_NAVI10_DEV,         SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI10_NITRO_PLUS_SUB_DEV3,            SAPPHIRE_NITRO_GLOW_V3_ADDR);
REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX 6600 XT Nitro+",                     DetectSapphireV3Controllers,    AMD_GPU_VEN,    AMD_NAVI23_DEV,         SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI23_NITRO_PLUS_SUB_DEV,             SAPPHIRE_NITRO_GLOW_V3_ADDR);
REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX 6650 XT Nitro+",                     DetectSapphireV3Controllers,    AMD_GPU_VEN,    AMD_NAVI23_DEV1,        SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI23_NITRO_PLUS_SUB_DEV,             SAPPHIRE_NITRO_GLOW_V3_ADDR);
REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX 6700 XT Nitro+",                     DetectSapphireV3Controllers,    AMD_GPU_VEN,    AMD_NAVI22_DEV,         SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI22_NITRO_PLUS_SUB_DEV,             SAPPHIRE_NITRO_GLOW_V3_ADDR);
REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX 6750 XT Nitro+",                     DetectSapphireV3Controllers,    AMD_GPU_VEN,    AMD_NAVI22_DEV,         SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI22_NITRO_PLUS_SUB_DEV1,            SAPPHIRE_NITRO_GLOW_V3_ADDR);
REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX 6800 Nitro+",                        DetectSapphireV3Controllers,    AMD_GPU_VEN,    AMD_NAVI21_DEV1,        SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI21_NITRO_PLUS_SUB_DEV3,            SAPPHIRE_NITRO_GLOW_V3_ADDR);
REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX 6800 XT Nitro+ SE",                  DetectSapphireV3Controllers,    AMD_GPU_VEN,    AMD_NAVI21_DEV1,        SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI21_NITRO_PLUS_SUB_DEV1,            SAPPHIRE_NITRO_GLOW_V3_ADDR);
REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX 6800 XT/6900 XT Nitro+",             DetectSapphireV3Controllers,    AMD_GPU_VEN,    AMD_NAVI21_DEV1,        SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI21_NITRO_PLUS_SUB_DEV2,            SAPPHIRE_NITRO_GLOW_V3_ADDR);
REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX 6900 XT Nitro+ SE",                  DetectSapphireV3Controllers,    AMD_GPU_VEN,    AMD_NAVI21_DEV1,        SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI21_6900XT_NITRO_PLUS_SUB_DEV,      SAPPHIRE_NITRO_GLOW_V3_ADDR);
REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX 6900 XT Toxic",                      DetectSapphireV3Controllers,    AMD_GPU_VEN,    AMD_NAVI21_DEV2,        SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI21_TOXIC_SUB_DEV,                  SAPPHIRE_NITRO_GLOW_V3_ADDR);
REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX 6900 XT Toxic",                      DetectSapphireV3Controllers,    AMD_GPU_VEN,    AMD_NAVI21_DEV1,        SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI21_6900XT_TOXIC_AC_SUB_DEV,        SAPPHIRE_NITRO_GLOW_V3_ADDR);
REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX 6900 XT Toxic Limited Edition",      DetectSapphireV3Controllers,    AMD_GPU_VEN,    AMD_NAVI21_DEV1,        SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI21_TOXIC_SUB_DEV1,                 SAPPHIRE_NITRO_GLOW_V3_ADDR);
REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX 6950 XT Toxic",                      DetectSapphireV3Controllers,    AMD_GPU_VEN,    AMD_NAVI21_DEV3,        SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI21_6950XT_TOXIC_AC_SUB_DEV,        SAPPHIRE_NITRO_GLOW_V3_ADDR);
REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX 6950 XT Nitro+",                     DetectSapphireV3Controllers,    AMD_GPU_VEN,    AMD_NAVI21_DEV3,        SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI21_6950XT_NITRO_PLUS_SUB_DEV,      SAPPHIRE_NITRO_GLOW_V3_ADDR);
REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX 6950 XT Nitro+ Pure",                DetectSapphireV3Controllers,    AMD_GPU_VEN,    AMD_NAVI21_DEV3,        SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI21_6950XT_NITRO_PLUS_PURE_SUB_DEV, SAPPHIRE_NITRO_GLOW_V3_ADDR);
REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX 7700 XT Nitro+",                     DetectSapphireV3Controllers,    AMD_GPU_VEN,    AMD_NAVI32_DEV,         SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI32_7700XT_NITRO_PLUS_SUB_DEV,      SAPPHIRE_NITRO_GLOW_V3_ADDR);
REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX 7800 XT Nitro+",                     DetectSapphireV3Controllers,    AMD_GPU_VEN,    AMD_NAVI32_DEV,         SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI32_7800XT_NITRO_PLUS_SUB_DEV,      SAPPHIRE_NITRO_GLOW_V3_ADDR);
REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX 7900 GRE Nitro+",                    DetectSapphireV3Controllers,    AMD_GPU_VEN,    AMD_NAVI31_DEV,         SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI31_GRE_NITRO_PLUS_SUB_DEV,         SAPPHIRE_NITRO_GLOW_V3_ADDR);
REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX 7900 XTX Nitro+",                    DetectSapphireV3Controllers,    AMD_GPU_VEN,    AMD_NAVI31_DEV,         SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI31_NITRO_PLUS_SUB_DEV,             SAPPHIRE_NITRO_GLOW_V3_ADDR);
REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX 9070 Pure",                          DetectSapphireV3Controllers,    AMD_GPU_VEN,    AMD_NAVI48_DEV,         SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI48_PURE_SUB_DEV,                   SAPPHIRE_NITRO_GLOW_V3_ADDR);
REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX 9070 XT Nitro+",                     DetectSapphireV3Controllers,    AMD_GPU_VEN,    AMD_NAVI48_DEV,         SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI48_NITRO_PLUS_SUB_DEV,             SAPPHIRE_NITRO_GLOW_V3_ADDR);
REGISTER_I2C_PCI_DETECTOR("Sapphire Radeon RX 9070 XT Pure",                     DetectSapphireV3Controllers,    AMD_GPU_VEN,    AMD_NAVI48_DEV,         SAPPHIRE_SUB_VEN,           SAPPHIRE_NAVI48_PURE_XT_SUB_DEV,             SAPPHIRE_NITRO_GLOW_V3_ADDR);
