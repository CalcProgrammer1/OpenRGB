/*-----------------------------------------*\
|  AsusAuraGPUControllerDetect.cpp          |
|                                           |
|  Driver for ASUS Aura RGB on GPUs         |
|                                           |
|  Jan Rettig (Klapstuhl) 14.02.2020        |
\*-----------------------------------------*/

#include "Detector.h"
#include "AsusAuraGPUController.h"
#include "LogManager.h"
#include "RGBController.h"
#include "RGBController_AsusAuraGPU.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#define ASUSGPU_CONTROLLER_NAME "ASUS Aura GPU"

/******************************************************************************************\
*                                                                                          *
*   TestForAuraGPUController                                                               *
*                                                                                          *
*       Tests the given address to see if an Aura GPU controller exists there.             *
*                                                                                          *
\******************************************************************************************/

bool TestForAsusAuraGPUController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    unsigned char aura_gpu_magic_high = bus->i2c_smbus_read_byte_data(address, 0x20);   // High Byte of magic (0x15)
    unsigned char aura_gpu_magic_low  = bus->i2c_smbus_read_byte_data(address, 0x21);   // Low Byte of magic (0x89)

    LOG_DEBUG("[%s] Test GPU expect: 0x1589 received: 0x%02X%02X", ASUSGPU_CONTROLLER_NAME, aura_gpu_magic_high, aura_gpu_magic_low);

    if((aura_gpu_magic_high << 8) + aura_gpu_magic_low == AURA_GPU_MAGIC_VAL)
    {
        pass = true;
    }

    return(pass);

}   /* TestForAuraGPUController() */

/******************************************************************************************\
*                                                                                          *
*   DetectAuraGPUControllers                                                               *
*                                                                                          *
*       Detect Aura GPU controllers on the enumerated I2C busses.                          *
*                                                                                          *
\******************************************************************************************/

void DetectAsusAuraGPUControllers(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name)
{
    if(TestForAsusAuraGPUController(bus, i2c_addr))
    {
        AuraGPUController*     controller     = new AuraGPUController(bus, i2c_addr);
        RGBController_AuraGPU* rgb_controller = new RGBController_AuraGPU(controller);
        rgb_controller->name                  = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
} /* DetectAsusAuraGPUControllers() */

/*-----------------------------------------*\
|  NVidia GPUs                              |
\*-----------------------------------------*/

REGISTER_I2C_PCI_DETECTOR("ASUS ROG Strix GTX 1050 O2G Gaming"        , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_GTX1050_DEV,         ASUS_SUB_VEN,   ASUS_GTX1050_STRIX_O2G_GAMING,              0x29);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG Strix GTX 1050 TI 4G Gaming"      , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_GTX1050TI_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_GTX1050TI_4G_GAMING,         0x29);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG Strix GTX 1050 TI O4G Gaming"     , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_GTX1050TI_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_GTX1050TI_O4G_GAMING,        0x29);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG Strix GTX 1050 TI O4G Gaming"     , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_GTX1050TI_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_GTX1050TI_O4G_GAMING_2,      0x29);
REGISTER_I2C_PCI_DETECTOR("ASUS GTX 1060 Strix 6G Gaming"             , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_GTX1060_DEV,         ASUS_SUB_VEN,   ASUS_GTX1060_STRIX_6G_GAMING,               0x29);
REGISTER_I2C_PCI_DETECTOR("ASUS GTX 1060 Strix"                       , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_GTX1060_DEV,         ASUS_SUB_VEN,   ASUS_GTX1060_STRIX,                         0x29);
REGISTER_I2C_PCI_DETECTOR("ASUS GTX 1070 Strix Gaming"                , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_GTX1070_DEV,         ASUS_SUB_VEN,   ASUS_GTX1070_STRIX_GAMING,                  0x29);
REGISTER_I2C_PCI_DETECTOR("ASUS GTX 1070 Strix OC"                    , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_GTX1070_DEV,         ASUS_SUB_VEN,   ASUS_GTX1070_STRIX_OC,                      0x29);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG Strix GTX1070 Ti 8G Gaming"       , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_GTX1070TI_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_GTX1070TI_8G_GAMING,         0x29);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG Strix GTX1070 Ti A8G Gaming"      , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_GTX1070TI_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_GTX1070TI_A8G_GAMING,        0x29);
REGISTER_I2C_PCI_DETECTOR("ASUS GTX 1080 Strix OC"                    , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_GTX1080_DEV,         ASUS_SUB_VEN,   ASUS_GTX1080_STRIX,                         0x29);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG Strix GTX1080 A8G Gaming"         , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_GTX1080_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_GTX1080_A8G_GAMING,          0x29);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG Strix GTX1080 O8G Gaming"         , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_GTX1080_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_GTX1080_O8G_GAMING,          0x29);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG Strix GTX1080 O8G 11Gbps"         , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_GTX1080_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_GTX1080_O8G_11GBPS,          0x29);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG Strix GTX1080 Ti Gaming"          , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_GTX1080TI_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_GTX1080TI_GAMING,            0x29);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG Strix GTX1080 Ti 11G Gaming"      , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_GTX1080TI_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_GTX1080TI_11G_GAMING,        0x29);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG Strix GTX1080 Ti O11G Gaming"     , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_GTX1080TI_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_GTX1080TI_O11G_GAMING,       0x29);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG Strix GTX1080 Ti O11G Gaming"     , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_GTX1080TI_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_GTX1080TI_O11G_GAMING_A02,   0x29);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG Strix GTX 1650S OC 4G"            , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_GTX1650S_DEV,        ASUS_SUB_VEN,   ASUS_ROG_STRIX_GTX1650S_OC,                 0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG Strix GTX 1660S O6G Gaming"       , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_GTX1660S_DEV,        ASUS_SUB_VEN,   ASUS_ROG_STRIX_GTX1660S_O6G_GAMING,         0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG GTX 1660 Ti OC 6G"                , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_GTX1660TI_DEV,       ASUS_SUB_VEN,   ASUS_ROG_GTX1660TI_OC,                      0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 2060 EVO Gaming 6G"     , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX2060_TU106_DEV,   ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX2060_EVO_GAMING,          0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 2060 O6G Gaming"        , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX2060_TU106_DEV,   ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX2060_O6G_GAMING,          0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 2060 O6G Gaming"        , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX2060_TU106_DEV,   ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX2060_O6G_GAMING_86D2,     0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 2060 O6G EVO Gaming"    , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX2060_TU104_DEV,   ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX2060_O6G_EVO_GAMING,      0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 2060S 8G Gaming"        , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX2060S_OC_DEV,     ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX2060S_8G_GAMING,          0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 2060S A8G EVO Gaming"   , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX2060S_DEV,        ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX2060S_A8G_EVO_GAMING,     0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 2060S O8G Gaming"       , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX2060S_OC_DEV,     ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX2060S_O8G_GAMING,         0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 2060S A8G Gaming"       , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX2060S_OC_DEV,     ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX2060S_A8G_GAMING,         0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 2070 A8G Gaming"        , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX2070_OC_DEV,      ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX2070_A8G_GAMING,          0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 2070 O8G Gaming"        , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX2070_OC_DEV,      ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX2070_O8G_GAMING,          0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 2070S A8G Gaming"       , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX2070S_DEV,        ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX2070S_A8G_GAMING_86FF,    0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 2070S A8G Gaming"       , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX2070S_OC_DEV,     ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX2070S_A8G_GAMING,         0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 2070S A8G Gaming"       , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX2070S_OC_DEV,     ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX2070S_A8G_GAMING_8706,    0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 2070S O8G Gaming"       , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX2070S_OC_DEV,     ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX2070S_O8G_GAMING_8729,    0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 2070S 8G Gaming"        , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX2070S_OC_DEV,     ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX2070S_8G_GAMING_8707,     0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 2070S O8G Gaming"       , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX2070S_OC_DEV,     ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX2070S_O8G_GAMING,         0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 2080 8G Gaming"         , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX2080_DEV,         ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX2080_8G_GAMING,           0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 2080 O8G Gaming"        , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX2080_A_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX2080_O8G_GAMING,          0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 2080 O8G V2 Gaming"     , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX2080_A_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX2080_O8G_V2_GAMING,       0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 2080S A8G Gaming"       , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX2080S_DEV,        ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX2080S_A8G_GAMING,         0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 2080S O8G Gaming"       , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX2080S_DEV,        ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX2080S_O8G_GAMING,         0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 2080S O8G White"        , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX2080S_DEV,        ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX2080S_O8G_WHITE,          0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 2080 Ti 11G Gaming"     , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX2080TI_DEV,       ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX2080TI_11G_GAMING,        0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 2080 Ti 11G Gaming"     , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX2080TI_A_DEV,     ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX2080TI_11G_GAMING_866C,   0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 2080 Ti A11G Gaming"    , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX2080TI_A_DEV,     ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX2080TI_A11G_GAMING,       0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RTX 2080 Ti O11G Gaming"    , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX2080TI_A_DEV,     ASUS_SUB_VEN,   ASUS_ROG_STRIX_RTX2080TI_O11G_GAMING,       0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS TUF RTX 3060 Ti 8G Gaming OC"         , DetectAsusAuraGPUControllers,   NVIDIA_VEN,     NVIDIA_RTX3060TI_LHR_DEV,   ASUS_SUB_VEN,   ASUS_TUF_RTX_3060_TI_O8G_OC,                0x2A);

/*-----------------------------------------*\
|  AMD GPUs                                 |
\*-----------------------------------------*/

REGISTER_I2C_PCI_DETECTOR("ASUS AREZ Strix RX Vega 56 O8G"            , DetectAsusAuraGPUControllers,   AMD_GPU_VEN,    AMD_VEGA10_DEV,             ASUS_SUB_VEN,   ASUS_AREZ_STRIX_VEGA56_08G_GAMING,          0x29);
REGISTER_I2C_PCI_DETECTOR("ASUS Vega 64 Strix"                        , DetectAsusAuraGPUControllers,   AMD_GPU_VEN,    AMD_VEGA10_DEV,             ASUS_SUB_VEN,   ASUS_VEGA64_STRIX,                          0x29);
REGISTER_I2C_PCI_DETECTOR("ASUS RX 5600XT Strix O6G Gaming"           , DetectAsusAuraGPUControllers,   AMD_GPU_VEN,    AMD_NAVI10_DEV,             ASUS_SUB_VEN,   ASUS_RX5600XT_STRIX_O6G_GAMING,             0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS RX 5700XT Strix Gaming OC"            , DetectAsusAuraGPUControllers,   AMD_GPU_VEN,    AMD_NAVI10_DEV,             ASUS_SUB_VEN,   ASUS_RX5700XT_STRIX_GAMING_OC,              0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS RX 5700XT Strix 08G Gaming"           , DetectAsusAuraGPUControllers,   AMD_GPU_VEN,    AMD_NAVI10_DEV,             ASUS_SUB_VEN,   ASUS_RX5700XT_STRIX_O8G_GAMING,             0x2A);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RX470 O4G Gaming"           , DetectAsusAuraGPUControllers,   AMD_GPU_VEN,    AMD_POLARIS_DEV,            ASUS_SUB_VEN,   ASUS_RX470_STRIX_O4G_GAMING,                0x29);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RX480 Gaming OC"            , DetectAsusAuraGPUControllers,   AMD_GPU_VEN,    AMD_POLARIS_DEV,            ASUS_SUB_VEN,   ASUS_RX480_STRIX_GAMING_OC,                 0x29);
REGISTER_I2C_PCI_DETECTOR("ASUS ROG STRIX RX560 Gaming"               , DetectAsusAuraGPUControllers,   AMD_GPU_VEN,    AMD_POLARIS11,              ASUS_SUB_VEN,   ASUS_RX560_STRIX_4G_GAMING,                 0x29);
REGISTER_I2C_PCI_DETECTOR("ASUS RX 570 Strix O4G Gaming OC"           , DetectAsusAuraGPUControllers,   AMD_GPU_VEN,    AMD_POLARIS_DEV,            ASUS_SUB_VEN,   ASUS_RX570_STRIX_O4G_GAMING_OC,             0x29);
REGISTER_I2C_PCI_DETECTOR("ASUS RX 570 Strix O8G Gaming OC"           , DetectAsusAuraGPUControllers,   AMD_GPU_VEN,    AMD_POLARIS_DEV,            ASUS_SUB_VEN,   ASUS_RX570_STRIX_O8G_GAMING_OC,             0x29);
REGISTER_I2C_PCI_DETECTOR("ASUS RX 580 Strix Gaming OC"               , DetectAsusAuraGPUControllers,   AMD_GPU_VEN,    AMD_POLARIS_DEV,            ASUS_SUB_VEN,   ASUS_RX580_STRIX_GAMING_OC,                 0x29);
REGISTER_I2C_PCI_DETECTOR("ASUS RX 580 Strix Gaming TOP"              , DetectAsusAuraGPUControllers,   AMD_GPU_VEN,    AMD_POLARIS_DEV,            ASUS_SUB_VEN,   ASUS_RX580_STRIX_GAMING_TOP,                0x29);
