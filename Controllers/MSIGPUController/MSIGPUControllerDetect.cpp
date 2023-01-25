/*-----------------------------------------*\
|  MSIGPUControllerDetect.cpp               |
|                                           |
|  Driver for MSI GPUs                      |
|                                           |
\*-----------------------------------------*/

#include "Detector.h"
#include "MSIGPUController.h"
#include "LogManager.h"
#include "RGBController.h"
#include "RGBController_MSIGPU.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************************\
*                                                                                          *
*   DetectMSIGPUControllers                                                               *
*                                                                                          *
*       Detect MSI GPU controllers on the enumerated I2C busses.                          *
*                                                                                          *
\******************************************************************************************/

void DetectMSIGPUControllers(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name)
{
    if(bus->pci_vendor == NVIDIA_VEN && bus->port_id != 1)
    {
        return;
    }

    MSIGPUController*     controller     = new MSIGPUController(bus, i2c_addr);
    RGBController_MSIGPU* rgb_controller = new RGBController_MSIGPU(controller);
    rgb_controller->name                 = name;

    ResourceManager::get()->RegisterRGBController(rgb_controller);

} /* DetectMSIGPUControllers() */

/*-----------------------------------------*\
|  NVidia GPUs                              |
\*-----------------------------------------*/

REGISTER_I2C_PCI_DETECTOR("MSI GeForce GTX 1070 Gaming X",                  DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_GTX1070_DEV,         MSI_SUB_VEN,    MSI_GTX1070_GAMING_X_SUB_DEV,           0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce GTX 1660 Gaming X 6G",               DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_GTX1660_DEV,         MSI_SUB_VEN,    MSI_GTX1660_GAMING_X_6G_SUB_DEV,        0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce GTX 1660Ti Gaming 6G",               DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_GTX1660TI_DEV,       MSI_SUB_VEN,    MSI_GTX1660TI_GAMING_6G_SUB_DEV,        0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce GTX 1660Ti Gaming X 6G",             DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_GTX1660TI_DEV,       MSI_SUB_VEN,    MSI_GTX1660TI_GAMING_X_6G_SUB_DEV,      0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce GTX 1660 Super Gaming 6G",           DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_GTX1660S_DEV,        MSI_SUB_VEN,    MSI_GTX1660S_GAMING_6G_SUB_DEV,         0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce GTX 1660 Super Gaming X 6G",         DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_GTX1660S_DEV,        MSI_SUB_VEN,    MSI_GTX1660S_GAMING_X_6G_SUB_DEV,       0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 2060 Gaming Z 6G",               DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX2060_TU104_DEV,   MSI_SUB_VEN,    MSI_RTX2060_GAMING_Z_6G_SUB_DEV,        0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 2060 Gaming Z 6G",               DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX2060_TU106_DEV,   MSI_SUB_VEN,    MSI_RTX2060_GAMING_Z_6G_SUB_DEV,        0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 2060 Gaming Z 6G",               DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX2060_TU106_DEV,   MSI_SUB_VEN,    MSI_RTX2060_GAMING_Z_6G_SUB_DEV_2,      0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 2060 Super Gaming X",            DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX2060S_OC_DEV,     MSI_SUB_VEN,    MSI_RTX2060S_GAMING_X_SUB_DEV,          0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 2060 Super ARMOR OC",            DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX2060S_OC_DEV,     MSI_SUB_VEN,    MSI_RTX2060S_ARMOR_OC_SUB_DEV,          0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 2070 Gaming Z",                  DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX2070_OC_DEV,      MSI_SUB_VEN,    MSI_RTX2070_GAMING_Z_SUB_DEV,           0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 2070 Gaming",                    DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX2070_DEV,         MSI_SUB_VEN,    MSI_RTX2070_GAMING_SUB_DEV,             0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 2070 ARMOR",                     DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX2070_DEV,         MSI_SUB_VEN,    MSI_RTX2070_ARMOR_SUB_DEV,              0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 2070 ARMOR OC",                  DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX2070_OC_DEV,      MSI_SUB_VEN,    MSI_RTX2070_ARMOR_SUB_DEV,              0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 2070 SUPER ARMOR OC",            DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX2070S_OC_DEV,     MSI_SUB_VEN,    MSI_RTX2070S_ARMOR_OC_SUB_DEV,          0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 2070 Super Gaming",              DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX2070S_OC_DEV,     MSI_SUB_VEN,    MSI_RTX2070S_GAMING_SUB_DEV,            0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 2070 Super Gaming Trio",         DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX2070S_OC_DEV,     MSI_SUB_VEN,    MSI_RTX2070S_GAMING_TRIO_SUB_DEV,       0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 2070 Super Gaming X",            DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX2070S_OC_DEV,     MSI_SUB_VEN,    MSI_RTX2070S_GAMING_X_SUB_DEV,          0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 2070 Super Gaming X Trio",       DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX2070S_OC_DEV,     MSI_SUB_VEN,    MSI_RTX2070S_GAMING_X_TRIO_SUB_DEV,     0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 2070 Super Gaming Z Trio",       DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX2070S_OC_DEV,     MSI_SUB_VEN,    MSI_RTX2070S_GAMING_Z_TRIO_SUB_DEV,     0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 2080 Gaming Trio",               DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX2080_DEV,         MSI_SUB_VEN,    MSI_RTX2080_GAMING_TRIO_SUB_DEV,        0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 2080 Gaming X Trio",             DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX2080_A_DEV,       MSI_SUB_VEN,    MSI_RTX2080_GAMING_X_TRIO_SUB_DEV,      0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 2080 Sea Hawk EK X",             DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX2080_A_DEV,       MSI_SUB_VEN,    MSI_RTX2080_SEA_HAWK_EK_X_SUB_DEV,      0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 2080 Duke 8G OC",                DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX2080_A_DEV,       MSI_SUB_VEN,    MSI_RTX2080_DUKE_OC_SUB_DEV,            0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 2080 Super Gaming X Trio",       DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX2080S_DEV,        MSI_SUB_VEN,    MSI_RTX2080S_GAMING_X_TRIO_SUB_DEV,     0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 2080Ti Gaming X Trio",           DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX2080TI_A_DEV,     MSI_SUB_VEN,    MSI_RTX2080TI_GAMING_X_TRIO_SUB_DEV,    0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 2080Ti Gaming Z Trio",           DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX2080TI_A_DEV,     MSI_SUB_VEN,    MSI_RTX2080TI_GAMING_Z_TRIO_SUB_DEV,    0X68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 2080Ti 11G Gaming X Trio",       DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX2080TI_A_DEV,     MSI_SUB_VEN,    MSI_RTX2080TI_11G_GAMING_X_TRIO_SUB_DEV,0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 2080Ti Sea Hawk EK X",           DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX2080TI_A_DEV,     MSI_SUB_VEN,    MSI_RTX2080TI_SEA_HAWK_EK_X_SUB_DEV,    0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 3060 Gaming X 12G",              DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX3060_DEV,         MSI_SUB_VEN,    MSI_RTX3060_GAMING_X_12G_SUB_DEV,       0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 3060 Gaming X 12G LHR",          DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX3060_LHR_DEV,     MSI_SUB_VEN,    MSI_RTX3060_GAMING_X_12G_SUB_DEV,       0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 3060 Gaming X 12G (GA104)",      DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX3060_GA104_DEV,   MSI_SUB_VEN,    MSI_RTX3060_GAMING_X_12G_SUB_DEV,       0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 3060 12G Gaming X Trio LHR",     DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX3060_LHR_DEV,     MSI_SUB_VEN,    MSI_RTX3070_GAMING_X_TRIO_SUB_DEV,      0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 3060 12GB Gaming X Trio",        DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX3060_DEV,         MSI_SUB_VEN,    MSI_RTX3070_GAMING_X_TRIO_SUB_DEV,      0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 3060 Ti 8GB Gaming X Trio",      DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX3060TI_DEV,       MSI_SUB_VEN,    MSI_RTX3070_GAMING_X_TRIO_SUB_DEV,      0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 3060 Ti 8GB Gaming X LHR",       DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX3060TI_LHR_DEV,   MSI_SUB_VEN,    MSI_RTX3060TI_GAMING_X_SUB_DEV,         0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 3060 Ti 8GB Gaming X Trio LHR",  DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX3060TI_LHR_DEV,   MSI_SUB_VEN,    MSI_RTX3070_GAMING_X_TRIO_SUB_DEV,      0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 3070 8GB Gaming Trio",           DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX3070_LHR_DEV,     MSI_SUB_VEN,    MSI_RTX3070_GAMING_TRIO_SUB_DEV,        0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 3070 8GB Gaming X Trio",         DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX3070_DEV,         MSI_SUB_VEN,    MSI_RTX3070_GAMING_X_TRIO_SUB_DEV,      0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 3070 8GB Suprim",                DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX3070_DEV,         MSI_SUB_VEN,    MSI_RTX3070_SUPRIM_SUB_DEV,             0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 3070 8GB Suprim X",              DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX3070_DEV,         MSI_SUB_VEN,    MSI_RTX3070_SUPRIM_X_SUB_DEV,           0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 3070 8GB Suprim X LHR",          DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX3070_LHR_DEV,     MSI_SUB_VEN,    MSI_RTX3070_SUPRIM_X_SUB_DEV,           0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 3070 Ti 8GB Gaming X Trio",      DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX3070TI_DEV,       MSI_SUB_VEN,    MSI_RTX3070TI_GAMING_X_TRIO_SUB_DEV,    0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 3070 Ti Suprim X 8G",            DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX3070TI_DEV,       MSI_SUB_VEN,    MSI_RTX3070TI_SUPRIM_X_SUB_DEV,         0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 3080 10GB Gaming Z Trio",        DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX3080_DEV,         MSI_SUB_VEN,    MSI_RTX3080_GAMING_Z_TRIO_SUB_DEV,      0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 3080 10GB Gaming Z Trio LHR",    DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX3080_LHR_DEV,     MSI_SUB_VEN,    MSI_RTX3080_GAMING_Z_TRIO_SUB_DEV,      0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 3080 10GB Gaming X Trio",        DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX3080_DEV,         MSI_SUB_VEN,    MSI_RTX3080_GAMING_X_TRIO_SUB_DEV,      0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 3080 Suprim X 10G",              DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX3080_DEV,         MSI_SUB_VEN,    MSI_RTX3080_SUPRIM_X_SUB_DEV,           0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 3080 Suprim X 10G LHR",          DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX3080_LHR_DEV,     MSI_SUB_VEN,    MSI_RTX3080_SUPRIM_X_SUB_DEV,           0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 3080 Suprim X 12G LHR",          DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX3080_12G_LHR_DEV, MSI_SUB_VEN,    MSI_RTX3080_SUPRIM_X_SUB_DEV,           0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 3080 12GB Gaming Z Trio LHR",    DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX3080_12G_LHR_DEV, MSI_SUB_VEN,    MSI_RTX3080_12G_GAMING_Z_TRIO_SUB_DEV,  0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 3080 Ti Gaming X Trio 12G",      DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX3080TI_DEV,       MSI_SUB_VEN,    MSI_RTX3080TI_GAMING_X_TRIO_SUB_DEV,    0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 3080 Ti Suprim X 12G",           DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX3080TI_DEV,       MSI_SUB_VEN,    MSI_RTX3080TI_SUPRIM_X_SUB_DEV,         0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 3090 24GB Gaming X Trio",        DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX3090_DEV,         MSI_SUB_VEN,    MSI_RTX3090_GAMING_X_TRIO_SUB_DEV,      0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 3090 Suprim 24G",                DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX3090_DEV,         MSI_SUB_VEN,    MSI_RTX3090_SUPRIM_SUB_DEV,             0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 3090 Suprim X 24G",              DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX3090_DEV,         MSI_SUB_VEN,    MSI_RTX3090_SUPRIM_X_SUB_DEV,           0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 3090 Ti Suprim X 24G",           DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX3090TI_DEV,       MSI_SUB_VEN,    MSI_RTX3090TI_SUPRIM_X_SUB_DEV,         0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 3090 Ti Gaming X Trio 24G",      DetectMSIGPUControllers,    NVIDIA_VEN,     NVIDIA_RTX3090TI_DEV,       MSI_SUB_VEN,    MSI_RTX3090TI_GAMING_X_TRIO_SUB_DEV,    0x68);

/*-----------------------------------------*\
|  AMD GPUs                                 |
\*-----------------------------------------*/

REGISTER_I2C_PCI_DETECTOR("MSI Radeon RX 6600 XT Gaming X",                 DetectMSIGPUControllers,    AMD_GPU_VEN,    AMD_NAVI23_DEV,             MSI_SUB_VEN,    MSI_RX6600XT_GAMING_X_SUB_DEV,          0x68);
REGISTER_I2C_PCI_DETECTOR("MSI Radeon RX 6700 XT Gaming X",                 DetectMSIGPUControllers,    AMD_GPU_VEN,    AMD_NAVI22_DEV,             MSI_SUB_VEN,    MSI_RX6700XT_GAMING_X_SUB_DEV,          0x68);
REGISTER_I2C_PCI_DETECTOR("MSI Radeon RX 6750 XT Gaming X Trio 12G",        DetectMSIGPUControllers,    AMD_GPU_VEN,    AMD_NAVI22_DEV,             MSI_SUB_VEN,    MSI_RX6750XT_GAMING_X_TRIO_SUB_DEV,     0x68);
REGISTER_I2C_PCI_DETECTOR("MSI Radeon RX 6800 Gaming X Trio",               DetectMSIGPUControllers,    AMD_GPU_VEN,    AMD_NAVI21_DEV1,            MSI_SUB_VEN,    MSI_RX6800_GAMING_X_TRIO_SUB_DEV,       0x68);
REGISTER_I2C_PCI_DETECTOR("MSI Radeon RX 6800 XT Gaming X Trio",            DetectMSIGPUControllers,    AMD_GPU_VEN,    AMD_NAVI21_DEV1,            MSI_SUB_VEN,    MSI_RX6800XT_GAMING_X_TRIO_SUB_DEV,     0x68);
REGISTER_I2C_PCI_DETECTOR("MSI Radeon RX 6800 XT Gaming Z Trio",            DetectMSIGPUControllers,    AMD_GPU_VEN,    AMD_NAVI21_DEV1,            MSI_SUB_VEN,    MSI_RX6800XT_GAMING_Z_TRIO_SUB_DEV,     0x68);
REGISTER_I2C_PCI_DETECTOR("MSI Radeon RX 6900 XT Gaming X Trio",            DetectMSIGPUControllers,    AMD_GPU_VEN,    AMD_NAVI21_DEV1,            MSI_SUB_VEN,    MSI_RX6900XT_GAMING_X_TRIO_SUB_DEV,     0x68);
REGISTER_I2C_PCI_DETECTOR("MSI Radeon RX 6900 XT Gaming Z Trio",            DetectMSIGPUControllers,    AMD_GPU_VEN,    AMD_NAVI21_DEV2,            MSI_SUB_VEN,    MSI_RX6950XT_GAMING_X_TRIO_SUB_DEV,     0x68);
REGISTER_I2C_PCI_DETECTOR("MSI Radeon RX 6950 XT Gaming X Trio",            DetectMSIGPUControllers,    AMD_GPU_VEN,    AMD_NAVI21_DEV1,            MSI_SUB_VEN,    MSI_RX6950XT_GAMING_X_TRIO_SUB_DEV,     0x68);
REGISTER_I2C_PCI_DETECTOR("MSI Radeon RX 6950 XT Gaming X Trio",            DetectMSIGPUControllers,    AMD_GPU_VEN,    AMD_NAVI21_DEV3,            MSI_SUB_VEN,    MSI_RX6950XT_GAMING_X_TRIO_SUB_DEV,     0x68);
