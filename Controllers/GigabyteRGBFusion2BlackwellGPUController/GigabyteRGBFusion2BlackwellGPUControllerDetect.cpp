/*---------------------------------------------------------*\
| GigabyteRGBFusion2BlackwellGPUControllerDetect.cpp        |
|                                                           |
|   Detector for Gigabyte RGB Fusion 2 Blackwell GPU        |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <stdio.h>
#include "DetectionManager.h"
#include "GigabyteRGBFusion2BlackwellGPUController.h"
#include "i2c_amd_gpu.h"
#include "i2c_smbus.h"
#include "LogManager.h"
#include "pci_ids.h"
#include "RGBController_GigabyteRGBFusion2BlackwellGPU.h"

#define GIGABYTEGPU_CONTROLLER_NAME3    "Gigabyte RGB Fusion2 Blackwell GPU"

bool TestForGigabyteRGBFusion2BlackwellGPUController(i2c_smbus_interface* bus, unsigned char address)
{
    if(bus->pci_vendor == AMD_GPU_VEN && !is_amd_gpu_i2c_bus(bus))
    {
        return false;
    }

    bool pass = false;
    int res, pktsz;
    const int read_sz = 4;
    const int write_sz = 64; //0x40
    uint8_t data_pkt[write_sz] = { 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    uint8_t data_readpkt[read_sz] = {};

    res = bus->i2c_write_block(address, write_sz, data_pkt);

    pass = true;

    pktsz = read_sz;
    res = bus->i2c_read_block(address, &pktsz, data_readpkt);

    //What we have seen returned so far...
    //GeForce RTX 5070 Ti Eagle OC 16G                          0x01 0x01 0x01 0x00
    //GeForce RTX 5070 Ti Gaming OC 16G                         0x01 0x01 0x01 0x00
    //GeForce RTX 5070 Gaming OC 12G                            0x01 0x01 0x01 0x00

    if(res < 0 || data_readpkt[0] != 0x01 || data_readpkt[1] != 0x01 || data_readpkt[2] != 0x01)
    {
        // Assemble C-string with respons for debugging
        std::string text = "";

        for(int idx = 0; idx < read_sz; ++idx)
        {
            char str[6];
            snprintf(str, 6, " 0x%02X", data_readpkt[idx]);
            text.append(str);
        }

        LOG_DEBUG("[%s] at address 0x%02X invalid. Expected 0x01 0x01 0x01 [0x*] but received:%s", GIGABYTEGPU_CONTROLLER_NAME3, address, text.c_str());
        pass = false;
    }

    return(pass);
}

DetectedControllers DetectGigabyteRGBFusion2BlackwellGPUControllers(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name, uint8_t led_zones)
{
    DetectedControllers detected_controllers;

    // Check for RGB Fusion2 controller
    if(TestForGigabyteRGBFusion2BlackwellGPUController(bus, i2c_addr))
    {
        RGBFusion2BlackwellGPUController*     controller     = new RGBFusion2BlackwellGPUController(bus, i2c_addr, name);
        RGBController_RGBFusion2BlackwellGPU* rgb_controller = new RGBController_RGBFusion2BlackwellGPU(controller, led_zones);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

DetectedControllers DetectGigabyteRGBFusion2BlackwellSingleZoneGPUControllers(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name)
{
    return(DetectGigabyteRGBFusion2BlackwellGPUControllers(bus, i2c_addr, name, RGB_FUSION2_BLACKWELL_GPU_SINGLE_ZONE));
}

DetectedControllers DetectGigabyteRGBFusion2BlackwellGamingLayoutGPUControllers(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name)
{
    return(DetectGigabyteRGBFusion2BlackwellGPUControllers(bus, i2c_addr, name, RGB_FUSION2_BLACKWELL_GPU_GAMING_LAYOUT));
}

DetectedControllers DetectGigabyteRGBFusion2BlackwellWaterforceLayoutGPUControllers(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name)
{
    return(DetectGigabyteRGBFusion2BlackwellGPUControllers(bus, i2c_addr, name, RGB_FUSION2_BLACKWELL_GPU_WATERFORCE_LAYOUT));
}

DetectedControllers DetectGigabyteRGBFusion2BlackwellAorusWaterforceLayoutGPUControllers(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name)
{
    return(DetectGigabyteRGBFusion2BlackwellGPUControllers(bus, i2c_addr, name, RGB_FUSION2_BLACKWELL_GPU_AORUS_WATERFORCE_LAYOUT));
}

/*---------------------------------------------------------*\
|  Nvidia GPUs                                              |
\*---------------------------------------------------------*/
REGISTER_I2C_PCI_DETECTOR("Gigabyte GeForce RTX 5060 Ti Gaming OC",                 DetectGigabyteRGBFusion2BlackwellSingleZoneGPUControllers,          NVIDIA_VEN, NVIDIA_RTX5060TI_DEV,   GIGABYTE_SUB_VEN, GIGABYTE_RTX5060TI_GAMING_OC_16G_SUB_DEV,         0x75);
REGISTER_I2C_PCI_DETECTOR("Gigabyte GeForce RTX 5070 Aero OC",                      DetectGigabyteRGBFusion2BlackwellSingleZoneGPUControllers,          NVIDIA_VEN, NVIDIA_RTX5070_DEV,     GIGABYTE_SUB_VEN, GIGABYTE_RTX5070_AERO_OC_12G_SUB_DEV,             0x75);
REGISTER_I2C_PCI_DETECTOR("Gigabyte GeForce RTX 5070 Eagle OC",                     DetectGigabyteRGBFusion2BlackwellSingleZoneGPUControllers,          NVIDIA_VEN, NVIDIA_RTX5070_DEV,     GIGABYTE_SUB_VEN, GIGABYTE_RTX5070_EAGLE_OC_12G_SUB_DEV,            0x75);
REGISTER_I2C_PCI_DETECTOR("Gigabyte GeForce RTX 5070 Gaming OC",                    DetectGigabyteRGBFusion2BlackwellGamingLayoutGPUControllers,        NVIDIA_VEN, NVIDIA_RTX5070_DEV,     GIGABYTE_SUB_VEN, GIGABYTE_RTX5070_GAMING_OC_12G_SUB_DEV,           0x75);
REGISTER_I2C_PCI_DETECTOR("Gigabyte GeForce RTX 5070 Ti Eagle OC",                  DetectGigabyteRGBFusion2BlackwellSingleZoneGPUControllers,          NVIDIA_VEN, NVIDIA_RTX5070TI_DEV,   GIGABYTE_SUB_VEN, GIGABYTE_RTX5070TI_EAGLE_OC_16G_SUB_DEV,          0x75);
REGISTER_I2C_PCI_DETECTOR("Gigabyte GeForce RTX 5070 Ti Eagle OC ICE",              DetectGigabyteRGBFusion2BlackwellSingleZoneGPUControllers,          NVIDIA_VEN, NVIDIA_RTX5070TI_DEV,   GIGABYTE_SUB_VEN, GIGABYTE_RTX5070TI_EAGLE_OC_ICE_16G_SUB_DEV,      0x75);
REGISTER_I2C_PCI_DETECTOR("Gigabyte GeForce RTX 5070 Ti Aero OC",                   DetectGigabyteRGBFusion2BlackwellSingleZoneGPUControllers,          NVIDIA_VEN, NVIDIA_RTX5070TI_DEV,   GIGABYTE_SUB_VEN, GIGABYTE_RTX5070TI_AERO_OC_16G_SUB_DEV,           0x75);
REGISTER_I2C_PCI_DETECTOR("Gigabyte GeForce RTX 5070 Ti Gaming OC",                 DetectGigabyteRGBFusion2BlackwellGamingLayoutGPUControllers,        NVIDIA_VEN, NVIDIA_RTX5070TI_DEV,   GIGABYTE_SUB_VEN, GIGABYTE_RTX5070TI_GAMING_OC_16G_SUB_DEV,         0x75);
REGISTER_I2C_PCI_DETECTOR("Gigabyte GeForce RTX 5080 Aero OC",                      DetectGigabyteRGBFusion2BlackwellSingleZoneGPUControllers,          NVIDIA_VEN, NVIDIA_RTX5080_DEV,     GIGABYTE_SUB_VEN, GIGABYTE_RTX5080_AERO_OC_16G_SUB_DEV,             0x75);
REGISTER_I2C_PCI_DETECTOR("Gigabyte GeForce RTX 5080 Gaming OC",                    DetectGigabyteRGBFusion2BlackwellGamingLayoutGPUControllers,             NVIDIA_VEN, NVIDIA_RTX5080_DEV,     GIGABYTE_SUB_VEN, GIGABYTE_RTX5080_GAMING_OC_16G_SUB_DEV,                0x75);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS GeForce RTX 5080 XTREME WATERFORCE",      DetectGigabyteRGBFusion2BlackwellAorusWaterforceLayoutGPUControllers,    NVIDIA_VEN, NVIDIA_RTX5080_DEV,     GIGABYTE_SUB_VEN, GIGABYTE_AORUS_RTX5080_XTREME_WATERFORCE_16G_SUB_DEV,  0x75);
REGISTER_I2C_PCI_DETECTOR("Gigabyte GeForce RTX 5080 XTREME WATERFORCE",            DetectGigabyteRGBFusion2BlackwellWaterforceLayoutGPUControllers,    NVIDIA_VEN, NVIDIA_RTX5080_DEV,     GIGABYTE_SUB_VEN, GIGABYTE_RTX5080_XTREME_WATERFORCE_16G_SUB_DEV,        0x75);
REGISTER_I2C_PCI_DETECTOR("Gigabyte GeForce RTX 5090 Gaming OC",                    DetectGigabyteRGBFusion2BlackwellGamingLayoutGPUControllers,        NVIDIA_VEN, NVIDIA_RTX5090_DEV,     GIGABYTE_SUB_VEN, GIGABYTE_RTX5090_GAMING_OC_32G_SUB_DEV,                0x75);
REGISTER_I2C_PCI_DETECTOR("Gigabyte GeForce RTX 5090 XTREME WATERFORCE",            DetectGigabyteRGBFusion2BlackwellWaterforceLayoutGPUControllers,    NVIDIA_VEN, NVIDIA_RTX5090_DEV,     GIGABYTE_SUB_VEN, GIGABYTE_RTX5090_XTREME_WATERFORCE_32G_SUB_DEV1,  0x75);
REGISTER_I2C_PCI_DETECTOR("Gigabyte GeForce RTX 5090 XTREME WATERFORCE",            DetectGigabyteRGBFusion2BlackwellWaterforceLayoutGPUControllers,    NVIDIA_VEN, NVIDIA_RTX5090_DEV,     GIGABYTE_SUB_VEN, GIGABYTE_RTX5090_XTREME_WATERFORCE_32G_SUB_DEV2,  0x75);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS GeForce RTX 5090 MASTER",                 DetectGigabyteRGBFusion2BlackwellGamingLayoutGPUControllers,        NVIDIA_VEN, NVIDIA_RTX5090_DEV,     GIGABYTE_SUB_VEN, GIGABYTE_AORUS_RTX5090_MASTER_32G_SUB_DEV,        0x75);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS GeForce RTX 5090 MASTER ICE",             DetectGigabyteRGBFusion2BlackwellGamingLayoutGPUControllers,        NVIDIA_VEN, NVIDIA_RTX5090_DEV,     GIGABYTE_SUB_VEN, GIGABYTE_AORUS_RTX5090_MASTER_ICE_32G_SUB_DEV,    0x75);

/*---------------------------------------------------------*\
|  AMD GPUs                                                 |
\*---------------------------------------------------------*/
REGISTER_I2C_PCI_DETECTOR("Gigabyte Radeon RX 9060 XT GAMING",                      DetectGigabyteRGBFusion2BlackwellGamingLayoutGPUControllers,        AMD_GPU_VEN, AMD_NAVI44_DEV,        GIGABYTE_SUB_VEN, GIGABYTE_RX9060XT_GAMING_16G_SUB_DEV,             0x75);
REGISTER_I2C_PCI_DETECTOR("Gigabyte Radeon RX 9060 XT GAMING OC",                   DetectGigabyteRGBFusion2BlackwellGamingLayoutGPUControllers,        AMD_GPU_VEN, AMD_NAVI44_DEV,        GIGABYTE_SUB_VEN, GIGABYTE_RX9060XT_GAMING_OC_16G_SUB_DEV,          0x75);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS Radeon RX 9070 XT Elite",                 DetectGigabyteRGBFusion2BlackwellGamingLayoutGPUControllers,        AMD_GPU_VEN, AMD_NAVI48_DEV,        GIGABYTE_SUB_VEN, GIGABYTE_AORUS_RX9070XT_ELITE_16G_SUB_DEV,        0x75);
REGISTER_I2C_PCI_DETECTOR("Gigabyte Radeon RX 9070 XT GAMING OC",                   DetectGigabyteRGBFusion2BlackwellGamingLayoutGPUControllers,        AMD_GPU_VEN, AMD_NAVI48_DEV,        GIGABYTE_SUB_VEN, GIGABYTE_RX9070XT_GAMING_OC_16G_SUB_DEV,          0x75);
REGISTER_I2C_PCI_DETECTOR("Gigabyte Radeon RX 9070 XT GAMING",                      DetectGigabyteRGBFusion2BlackwellGamingLayoutGPUControllers,        AMD_GPU_VEN, AMD_NAVI48_DEV,        GIGABYTE_SUB_VEN, GIGABYTE_RX9070XT_GAMING_16G_SUB_DEV,             0x75);
