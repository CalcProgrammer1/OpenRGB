#include "Detector.h"
#include "GigabyteRGBFusion2GPUController.h"
#include "LogManager.h"
#include "RGBController.h"
#include "RGBController_GigabyteRGBFusion2GPU.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

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
    //GeForce RTX 3070 AORUS MASTER 8G                  0xAB 0x11 0x52 0x03
    //GeForce RTX 3060 Ti GAMING OC PRO 8G              0xAB 0x10 0x01 0x02
    //GeForce RTX 3070 AORUS ELITE 12G                  0xAB 0x11 0x52 0x03
    //GeForce RTX 3080 Ti AORUS XTREME WATERFORCE 12G   0xAB 0x11 0x01 0x00
    //GeForce RTX 3080 AORUS XTREME WATERFORCE WB 10G   0xAB 0x10 0x01 0x00
    if(res < 0)
    {
        LOG_DEBUG("[%s] at 0x%02X address expected 0x04 but received: 0x%02X", GIGABYTEGPU_CONTROLLER_NAME2, address, res);
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

REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX2060 SUPER 8G V1",                     DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX2060S_OC_DEV,     GIGABYTE_SUB_VEN,   GIGABYTE_AORUS_RTX2060S_V1_SUB_DEV,             0x50);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX2070 XTREME 8G",                       DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX2070_OC_DEV,      GIGABYTE_SUB_VEN,   GIGABYTE_AORUS_RTX2070_XTREME_SUB_DEV,          0x50);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX2070 SUPER 8G",                        DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX2070S_OC_DEV,     GIGABYTE_SUB_VEN,   GIGABYTE_RTX2070S_GAMING_OC_SUB_DEV2,           0x50);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX2070 SUPER 8G",                        DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX2070S_OC_DEV,     GIGABYTE_SUB_VEN,   GIGABYTE_AORUS_RTX2070S_8G_SUB_DEV,             0x50);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX2080 8G",                              DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX2080_A_DEV,       GIGABYTE_SUB_VEN,   GIGABYTE_AORUS_RTX2080_SUB_DEV,                 0x50);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX2080 SUPER 8G",                        DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX2080S_DEV,        GIGABYTE_SUB_VEN,   GIGABYTE_AORUS_RTX2080S_SUB_DEV,                0x50);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX2080 SUPER Waterforce WB 8G",          DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX2080S_DEV,        GIGABYTE_SUB_VEN,   GIGABYTE_AORUS_RTX2080S_WATERFORCE_WB_SUB_DEV,  0x51);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX3060 ELITE 12G",                       DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3060_LHR_DEV,     GIGABYTE_SUB_VEN,   GIGABYTE_RTX3060_ELITE_12GB_SUB_DEV,            0x70);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX3060 Ti ELITE 8G LHR",                 DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3060TI_LHR_DEV,   GIGABYTE_SUB_VEN,   GIGABYTE_RTX3060TI_ELITE_8GB_SUB_DEV,           0x70);
REGISTER_I2C_PCI_DETECTOR("Gigabyte RTX3060 Ti GAMING OC PRO 8G",                   DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3060TI_DEV,       GIGABYTE_SUB_VEN,   GIGABYTE_RTX3060TI_GAMING_OC_PRO_SUB_DEV,       0x62);
REGISTER_I2C_PCI_DETECTOR("Gigabyte RTX3060 Ti Gaming OC PRO 8G LHR",               DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3060TI_LHR_DEV,   GIGABYTE_SUB_VEN,   GIGABYTE_RTX3060TI_GAMING_OC_PRO_SUB_DEV,       0x62);
REGISTER_I2C_PCI_DETECTOR("Gigabyte RTX3070 MASTER 8G",                             DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3070_DEV,         GIGABYTE_SUB_VEN,   GIGABYTE_RTX3070_MASTER_OC_SUB_DEV,             0x66);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX3080 XTREME WATERFORCE WB 10G",        DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3080_DEV,         GIGABYTE_SUB_VEN,   GIGABYTE_RTX3080_XTREME_WATERFORCE_SUB_DEV,     0x64);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX3080 XTREME WATERFORCE WB 10G",        DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3080_LHR_DEV,     GIGABYTE_SUB_VEN,   GIGABYTE_RTX3080_XTREME_WATERFORCE_SUB_DEV,     0x64);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX3080 XTREME WATERFORCE 10G Rev 2.0",   DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3080_LHR_DEV,     GIGABYTE_SUB_VEN,   GIGABYTE_RTX3080_XTREME_WATERFORCE_V2_SUB_DEV,  0x65);
REGISTER_I2C_PCI_DETECTOR("Gigabyte RTX3080 Ti Vision OC 12G",                      DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3080TI_DEV,       GIGABYTE_SUB_VEN,   GIGABYTE_RTX3080TI_VISION_OC_SUB_DEV,           0x63);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX3080 Ti XTREME WATERFORCE 12G",        DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3080TI_DEV,       GIGABYTE_SUB_VEN,   GIGABYTE_RTX3080TI_XTREME_WATERFORCE_SUB_DEV,   0x65);
REGISTER_I2C_PCI_DETECTOR("Gigabyte AORUS RTX3090 XTREME WATERFORCE WB 24G",        DetectGigabyteRGBFusion2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3090_DEV,         GIGABYTE_SUB_VEN,   GIGABYTE_RTX3090_XTREME_WATERFORCE_SUB_DEV,     0x64);