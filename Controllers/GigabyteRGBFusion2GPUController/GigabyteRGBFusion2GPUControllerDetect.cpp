/*---------------------------------------------------------*\
| GigabyteRGBFusion2GPUControllerDetect.cpp                 |
|                                                           |
|   Detector for Gigabyte Aorus RGB Fusion 2 GPU            |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "Detector.h"
#include "GigabyteRGBFusion2GPUController.h"
#include "LogManager.h"
#include "RGBController.h"
#include "RGBController_GigabyteRGBFusion2GPU.h"
#include "i2c_smbus.h"
#include "pci_ids.h"

#define GIGABYTEGPU_CONTROLLER_NAME2    "Gigabyte RGB Fusion2 GPU"

/******************************************************************************************\
*                                                                                          *
*   TestForGigabyteRGBFusion2GPUController                                                 *
*                                                                                          *
*       Tests the given address to see if an RGB Fusion2 controller exists there.  First   *
*       does a quick write to test for a response                                          *
*                                                                                          *
\******************************************************************************************/

bool TestForGigabyteRGBFusion2GPUController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;
    int res, pktsz;
    const int read_sz = 4;
    const int write_sz = 8;
    uint8_t data_pkt[write_sz] = { 0xAB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    uint8_t data_readpkt[read_sz] = {};

    res = bus->i2c_write_block(address, write_sz, data_pkt);

    pass = true;

    pktsz = read_sz;
    res = bus->i2c_read_block(address, &pktsz, data_readpkt);

    //What we have seen returned so far...
    //GeForce RTX 3070 AORUS MASTER 8G                      0xAB 0x11 0x52 0x03
    //GeForce RTX 3060 Ti GAMING OC PRO 8G                  0xAB 0x10 0x01 0x02
    //GeForce RTX 3070 AORUS ELITE 12G                      0xAB 0x11 0x52 0x03
    //GeForce RTX 3080 Ti AORUS XTREME WATERFORCE 12G       0xAB 0x11 0x01 0x00
    //GeForce RTX 3080 Ti AORUS XTREME WATERFORCE 12G LHS   0xAB 0x11 0x52 0x00
    //GeForce RTX 3080 AORUS XTREME WATERFORCE WB 10G       0xAB 0x10 0x01 0x00
    //GeForce RTX 4080 Gigabyte AORUS MASTER 16G            0xAB 0x10 0x52 0x07
    //Note that GeForce RTX 3080 Ti AORUS XTREME WATERFORCE 12G LHS exposes three i2c buses but only one returns a 0xAB
    //response and controls the RGB lighting. The other buses return 0x00 0x00 0x00 0x00.
    //Note that GeForce RTX 4080 Gigabyte AORUS MASTER 16G exposes two i2c bus with writable address 0x71 but one respond
    //0x00 0x00 0x00 0x00 so it should be the one controlling the LCD screen. So we skip this bus

    //All seen responses start with 0xAB, so we check for this.
    if(res < 0 || data_readpkt[0] != 0xAB)
    {
        // Assemble C-string with respons for debugging
        std::string text = "";

        for(int idx = 0; idx < read_sz; ++idx)
        {
            char str[6];
            snprintf(str, 6, " 0x%02X", data_readpkt[idx]);
            text.append(str);
        }

        LOG_DEBUG("[%s] at address 0x%02X invalid. Expected 0xAB [0x*] but received:%s", GIGABYTEGPU_CONTROLLER_NAME2, address, text.c_str());
        pass = false;
    }

    return(pass);
}   /* TestForRGBFusion2GPUController() */

/*******************************************************************************************\
*                                                                                           *
*   DetectRGBFusion2GPUControllers                                                          *
*                                                                                           *
*       Detect GigabyteRGB Fusion2 controllers on the enumerated I2C busses at address 0x70.*
*                                                                                           *
*           bus - pointer to i2c_smbus_interface where RGB Fusion2 device is connected      *
*           dev - I2C address of RGB Fusion2 device                                         *
*                                                                                           *
\*******************************************************************************************/

void DetectGigabyteRGBFusion2GPUControllers(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name)
{
    // Check for RGB Fusion2 controller
    if(TestForGigabyteRGBFusion2GPUController(bus, i2c_addr))
    {
        RGBFusion2GPUController*     controller     = new RGBFusion2GPUController(bus, i2c_addr);
        RGBController_RGBFusion2GPU* rgb_controller = new RGBController_RGBFusion2GPU(controller);
        rgb_controller->name                        = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectGigabyteRGBFusion2GPUControllers() */

REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX 2060 SUPER 8G V1",                     DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX2060S_OC_DEV,     GIGABYTE_SUB_VEN,   GIGABYTE_AORUS_RTX2060S_V1_SUB_DEV,             0x50);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX 2070 XTREME 8G",                       DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX2070_OC_DEV,      GIGABYTE_SUB_VEN,   GIGABYTE_AORUS_RTX2070_XTREME_SUB_DEV,          0x50);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX 2070 XTREME 8G",                       DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX2070_OC_DEV,      GIGABYTE_SUB_VEN,   GIGABYTE_AORUS_RTX2070_XTREME_SUB_DEV2,         0x50);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX 2070 SUPER 8G",                        DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX2070S_OC_DEV,     GIGABYTE_SUB_VEN,   GIGABYTE_RTX2070S_GAMING_OC_SUB_DEV2,           0x50);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX 2070 SUPER 8G",                        DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX2070S_OC_DEV,     GIGABYTE_SUB_VEN,   GIGABYTE_AORUS_RTX2070S_8G_SUB_DEV,             0x50);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX 2080 8G",                              DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX2080_A_DEV,       GIGABYTE_SUB_VEN,   GIGABYTE_AORUS_RTX2080_SUB_DEV,                 0x50);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX 2080 XTREME 8G",                       DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX2080_A_DEV,       GIGABYTE_SUB_VEN,   GIGABYTE_AORUS_RTX2080_XTREME_SUB_DEV,          0x50);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX 2080 SUPER 8G",                        DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX2080S_DEV,        GIGABYTE_SUB_VEN,   GIGABYTE_AORUS_RTX2080S_SUB_DEV,                0x50);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX 2080 SUPER 8G Rev 1.0",                DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX2080S_DEV,        GIGABYTE_SUB_VEN,   GIGABYTE_AORUS_RTX2080S_SUB_DEV_V1,             0x50);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX 2080 SUPER Waterforce WB 8G",          DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX2080S_DEV,        GIGABYTE_SUB_VEN,   GIGABYTE_AORUS_RTX2080S_WATERFORCE_WB_SUB_DEV,  0x51);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX 2080 SUPER Waterforce WB 8G",          DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX2080S_DEV,        GIGABYTE_SUB_VEN,   GIGABYTE_AORUS_RTX2080S_WATERFORCE_WB_SUB_DEV2, 0x08);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX 2080 Ti XTREME 11G",                   DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX2080TI_A_DEV,     GIGABYTE_SUB_VEN,   GIGABYTE_AORUS_RTX2080TI_EXTREME_SUB_DEV,       0x50);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX 3060 ELITE 12G",                       DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3060_GA104_DEV,   GIGABYTE_SUB_VEN,   GIGABYTE_RTX3060_ELITE_12GB_SUB_DEV,            0x70);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX 3060 ELITE 12G Rev a1",                DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3060_GA106_DEV,   GIGABYTE_SUB_VEN,   GIGABYTE_RTX3060_ELITE_12GB_SUB_DEV,            0x70);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX 3060 ELITE 12G LHR",                   DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3060_LHR_DEV,     GIGABYTE_SUB_VEN,   GIGABYTE_RTX3060_ELITE_12GB_SUB_DEV,            0x70);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX 3060 Ti ELITE 8G LHR",                 DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3060TI_LHR_DEV,   GIGABYTE_SUB_VEN,   GIGABYTE_RTX3060TI_ELITE_8GB_SUB_DEV,           0x70);
REGISTER_I2C_PCI_DETECTOR("Gigabyte RTX 3060 Ti GAMING OC 8G",                       DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3060TI_GDDR6X_DEV, GIGABYTE_SUB_VEN,   GIGABYTE_RTX3060TI_GAMING_OC_8G_SUB_DEV,       0x62);
REGISTER_I2C_PCI_DETECTOR("Gigabyte RTX 3060 Ti GAMING OC 8G",                       DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3060TI_DEV,       GIGABYTE_SUB_VEN,   GIGABYTE_RTX3060TI_GAMING_OC_SUB_DEV,           0x62);
REGISTER_I2C_PCI_DETECTOR("Gigabyte RTX 3060 Ti GAMING OC LHR 8G",                   DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3060TI_LHR_DEV,   GIGABYTE_SUB_VEN,   GIGABYTE_RTX3060TI_GAMING_OC_SUB_DEV,           0x62);
REGISTER_I2C_PCI_DETECTOR("Gigabyte RTX 3060 Ti GAMING OC PRO 8G",                   DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3060TI_DEV,       GIGABYTE_SUB_VEN,   GIGABYTE_RTX3060TI_GAMING_OC_PRO_SUB_DEV,       0x62);
REGISTER_I2C_PCI_DETECTOR("Gigabyte RTX 3060 Ti Gaming OC PRO 8G LHR",               DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3060TI_LHR_DEV,   GIGABYTE_SUB_VEN,   GIGABYTE_RTX3060TI_GAMING_OC_PRO_SUB_DEV,       0x62);
REGISTER_I2C_PCI_DETECTOR("Gigabyte RTX 3070 MASTER 8G",                             DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3070_DEV,         GIGABYTE_SUB_VEN,   GIGABYTE_RTX3070_MASTER_OC_SUB_DEV,             0x66);
REGISTER_I2C_PCI_DETECTOR("Gigabyte RTX 3070 MASTER 8G LHR",                         DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3070_LHR_DEV,     GIGABYTE_SUB_VEN,   GIGABYTE_RTX3070_MASTER_OC_SUB_DEV,             0x66);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX 3070 Ti MASTER 8G",                    DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3070TI_DEV,       GIGABYTE_SUB_VEN,   GIGABYTE_AORUS_RTX3070TI_MASTER_8G_SUB_DEV,     0x70);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX 3080 XTREME WATERFORCE WB 10G",        DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3080_DEV,         GIGABYTE_SUB_VEN,   GIGABYTE_RTX3080_XTREME_WATERFORCE_SUB_DEV,     0x64);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX 3080 XTREME WATERFORCE WB 10G",        DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3080_LHR_DEV,     GIGABYTE_SUB_VEN,   GIGABYTE_RTX3080_XTREME_WATERFORCE_SUB_DEV,     0x64);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX 3080 XTREME WATERFORCE WB 12G LHR",    DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3080_12G_LHR_DEV, GIGABYTE_SUB_VEN,   GIGABYTE_RTX3080_XTREME_WATERFORCE_12G_SUB_DEV, 0x64);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX 3080 XTREME WATERFORCE 10G Rev 2.0",   DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3080_LHR_DEV,     GIGABYTE_SUB_VEN,   GIGABYTE_RTX3080_XTREME_WATERFORCE_V2_SUB_DEV,  0x65);
REGISTER_I2C_PCI_DETECTOR("Gigabyte RTX 3080 Ti Vision OC 12G",                      DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3080TI_DEV,       GIGABYTE_SUB_VEN,   GIGABYTE_RTX3080TI_VISION_OC_SUB_DEV,           0x63);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX 3080 Ti XTREME WATERFORCE 12G",        DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3080TI_DEV,       GIGABYTE_SUB_VEN,   GIGABYTE_RTX3080TI_XTREME_WATERFORCE_SUB_DEV,   0x65);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX 3080 Ti XTREME WATERFORCE 12G",        DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3080TI_DEV,       GIGABYTE_SUB_VEN,   GIGABYTE_RTX3080TI_XTREME_WATERFORCE_SUB_DEV2,  0x64);
REGISTER_I2C_PCI_DETECTOR("Gigabyte RTX 3090 VISION OC 24G ",                        DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3090_DEV,         GIGABYTE_SUB_VEN,   GIGABYTE_RTX3090_VISION_OC_24G_SUB_DEV,         0x63);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX 3090 XTREME WATERFORCE 24G",           DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3090_DEV,         GIGABYTE_SUB_VEN,   GIGABYTE_RTX3090_XTREME_WATERFORCE_SUB_DEV,     0x65);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX 3090 XTREME WATERFORCE WB 24G",        DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3090_DEV,         GIGABYTE_SUB_VEN,   GIGABYTE_RTX3090_XTREME_WATERFORCE_WB_SUB_DEV,  0x64);
REGISTER_I2C_PCI_DETECTOR("Gigabyte RTX 4070 Gaming OC 12G",                         DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX4070_DEV,         GIGABYTE_SUB_VEN,   GIGABYTE_RTX4070_GAMING_OC_12G,                 0x71);
REGISTER_I2C_PCI_DETECTOR("Gigabyte RTX 4070 Gaming OC 12G",                         DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX4070_DEV,         GIGABYTE_SUB_VEN,   GIGABYTE_RTX4070_GAMING_OC_12G_V2,              0x71);
REGISTER_I2C_PCI_DETECTOR("Gigabyte RTX 4070 SUPER Gaming OC 12G",                   DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX4070S_DEV,        GIGABYTE_SUB_VEN,   GIGABYTE_RTX4070S_GAMING_OC_12G,                0x71);
REGISTER_I2C_PCI_DETECTOR("Gigabyte RTX 4070 SUPER Aero OC 12G",                     DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX4070S_DEV,        GIGABYTE_SUB_VEN,   GIGABYTE_RTX4070S_AERO_OC_12G,                  0x71);
REGISTER_I2C_PCI_DETECTOR("Gigabyte RTX 4070 Ti Gaming 12G",                         DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX4070TI_DEV,       GIGABYTE_SUB_VEN,   GIGABYTE_RTX4070TI_GAMING_12G,                  0x71);
REGISTER_I2C_PCI_DETECTOR("Gigabyte RTX 4070 Ti Gaming OC 12G",                      DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX4070TI_DEV,       GIGABYTE_SUB_VEN,   GIGABYTE_RTX4070TI_GAMING_OC_12G,               0x71);
REGISTER_I2C_PCI_DETECTOR("Gigabyte RTX 4070 Ti Gaming OC 12G",                      DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX4070TI_DEV,       GIGABYTE_SUB_VEN,   GIGABYTE_RTX4070TI_GAMING_OC_12G_SUB_DEV2,      0x71);
REGISTER_I2C_PCI_DETECTOR("Gigabyte RTX 4070 Ti Eagle OC 12G",                       DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX4070TI_DEV,       GIGABYTE_SUB_VEN,   GIGABYTE_RTX4070TI_EAGLE_OC_SUB_DEV,            0x71);
REGISTER_I2C_PCI_DETECTOR("Gigabyte RTX 4070 Ti Super Gaming OC 16G",                DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX4070TIS_DEV,      GIGABYTE_SUB_VEN,   GIGABYTE_RTX4070TIS_GAMING_OC_16G_SUB_DEV,      0x71);
REGISTER_I2C_PCI_DETECTOR("Gigabyte RTX 4080 AERO OC 16G",                           DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX4080_DEV,         GIGABYTE_SUB_VEN,   GIGABYTE_RTX4080_AERO_OC_SUB_DEV,               0x71);
REGISTER_I2C_PCI_DETECTOR("Gigabyte RTX 4080 Eagle OC 16G",                          DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX4080_DEV,         GIGABYTE_SUB_VEN,   GIGABYTE_RTX4080_EAGLE_OC_SUB_DEV,              0x71);
REGISTER_I2C_PCI_DETECTOR("Gigabyte RTX 4080 SUPER Gaming OC 16G",                   DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX4080S_DEV,        GIGABYTE_SUB_VEN,   GIGABYTE_RTX4080S_GAMING_OC_16GB_SUB_DEV,       0x72);
REGISTER_I2C_PCI_DETECTOR("Gigabyte RTX 4080 Gaming OC 16G",                         DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX4080_DEV,         GIGABYTE_SUB_VEN,   GIGABYTE_RTX4080_GAMING_OC_SUB_DEV,             0x71);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX 4080 MASTER 16G",                      DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX4080_DEV,         GIGABYTE_SUB_VEN,   GIGABYTE_AORUS_RTX4080_MASTER_16G_SUB_DEV,      0x71);
REGISTER_I2C_PCI_DETECTOR("Gigabyte RTX 4090 AERO OC 24G",                           DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX4090_DEV,         GIGABYTE_SUB_VEN,   GIGABYTE_RTX4090_AERO_OC_24G_SUB_DEV,           0x71);
REGISTER_I2C_PCI_DETECTOR("Gigabyte RTX 4090 GAMING OC 24G",                         DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX4090_DEV,         GIGABYTE_SUB_VEN,   GIGABYTE_RTX4090_GAMING_OC_24G_SUB_DEV,         0x71);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX 4090 MASTER 24G",                      DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX4090_DEV,         GIGABYTE_SUB_VEN,   GIGABYTE_AORUS_RTX4090_MASTER_24G_SUB_DEV,      0x71);
