/*---------------------------------------------------------*\
| EVGAAmpereGPUControllerDetect.cpp                         |
|                                                           |
|   Detector for EVGA V3 (Ampere) GPU                       |
|                                                           |
|   TheRogueZeta                                15 Jul 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "EVGAGPUv3Controller.h"
#include "LogManager.h"
#include "RGBController_EVGAGPUv3.h"
#include "i2c_smbus.h"
#include "pci_ids.h"

/******************************************************************************************\
*                                                                                          *
*   DetectEVGAAmpereGPUControllers                                                         *
*                                                                                          *
*       Detect EVGA Ampere GPU controllers on the enumerated I2C busses at address 0x2D.   *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where EVGA GPU device is connected        *
*           dev - I2C address of EVGA GPU device                                           *
*                                                                                          *
\******************************************************************************************/

void DetectEVGAAmpereGPUControllers(i2c_smbus_interface* bus, uint8_t address, const std::string& name)
{
    if(bus->port_id == 1)
    {
        EVGAGPUv3Controller*     controller;
        RGBController_EVGAGPUv3* rgb_controller;

        controller = new EVGAGPUv3Controller(bus, address, name);

        if(controller-> ReadFWVersion() != "")
        {
            rgb_controller = new RGBController_EVGAGPUv3(controller);

            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
        else
        {
            LOG_INFO("[%s] Failed to get a valid FW version, does the i2c interface support `i2c_smbus_read_i2c_block_data`?", controller->GetDeviceName().c_str());
            delete controller;
        }
    }
}   /* DetectEVGAAmpereGPUControllers() */

REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3060 Ti FTW3 Gaming"                , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3060TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3060TI_FTW3_GAMING_SUB_DEV,             0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3060 Ti FTW3 Ultra"                 , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3060TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3060TI_FTW3_ULTRA_SUB_DEV,              0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3060 Ti FTW3 Ultra LHR"             , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3060TI_LHR_DEV,   EVGA_SUB_VEN,   EVGA_RTX3060TI_FTW3_ULTRA_KL_SUB_DEV,           0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3060 Ti FTW3 Ultra Gaming LHR"      , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3060TI_LHR_DEV,   EVGA_SUB_VEN,   EVGA_RTX3060TI_FTW3_ULTRA_GAMING_LHR_SUB_DEV,   0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3070 Black Gaming"                  , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3070_DEV,         EVGA_SUB_VEN,   EVGA_RTX3070_XC3_BLACK_SUB_DEV,                 0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3070 XC3 Gaming"                    , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3070_DEV,         EVGA_SUB_VEN,   EVGA_RTX3070_XC3_GAMING_SUB_DEV,                0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3070 XC3 Ultra"                     , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3070_DEV,         EVGA_SUB_VEN,   EVGA_RTX3070_XC3_ULTRA_SUB_DEV,                 0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3070 XC3 Ultra LHR"                 , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3070_LHR_DEV,     EVGA_SUB_VEN,   EVGA_RTX3070_XC3_ULTRA_LHR_SUB_DEV,             0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3070 XC3 Ultra Gaming"              , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3070_LHR_DEV,     EVGA_SUB_VEN,   EVGA_RTX3070_XC3_ULTRA_GAMING_LHR_SUB_DEV,      0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3070 FTW3 Ultra"                    , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3070_DEV,         EVGA_SUB_VEN,   EVGA_RTX3070_FTW3_ULTRA_SUB_DEV,                0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3070 FTW3 Ultra LHR"                , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3070_LHR_DEV,     EVGA_SUB_VEN,   EVGA_RTX3070_FTW3_ULTRA_LHR_SUB_DEV,            0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3070 FTW3 Ultra LHR"                , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3070_LHR_DEV,     EVGA_SUB_VEN,   EVGA_RTX3070_FTW3_ULTRA_LHR_ALT_SUB_DEV,        0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3070 Ti XC3 Gaming"                 , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3070TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3070TI_XC3_GAMING_SUB_DEV,              0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3070 Ti XC3 Ultra"                  , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3070TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3070TI_XC3_ULTRA_SUB_DEV,               0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3070 Ti XC3 Ultra v2"               , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3070TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3070TI_XC3_ULTRA_V2_SUB_DEV,            0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3070 Ti FTW3 Ultra"                 , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3070TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3070TI_FTW3_ULTRA_SUB_DEV,              0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3070 Ti FTW3 Ultra v2"              , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3070TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3070TI_FTW3_ULTRA_V2_SUB_DEV,           0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3080 XC3 Black"                     , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         EVGA_SUB_VEN,   EVGA_RTX3080_XC3_BLACK_SUB_DEV,                 0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3080 XC3 Black LHR"                 , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3080_LHR_DEV,     EVGA_SUB_VEN,   EVGA_RTX3080_XC3_BLACK_LHR_SUB_DEV,             0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3080 XC3 Gaming"                    , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         EVGA_SUB_VEN,   EVGA_RTX3080_XC3_GAMING_SUB_DEV,                0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3080 XC3 Gaming LHR"                , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3080_LHR_DEV,     EVGA_SUB_VEN,   EVGA_RTX3080_XC3_GAMING_LHR_SUB_DEV,            0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3080 XC3 Ultra"                     , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         EVGA_SUB_VEN,   EVGA_RTX3080_XC3_ULTRA_SUB_DEV,                 0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3080 XC3 Ultra LHR"                 , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3080_LHR_DEV,     EVGA_SUB_VEN,   EVGA_RTX3080_XC3_ULTRA_LHR_SUB_DEV,             0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3080 XC3 Ultra Hybrid"              , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         EVGA_SUB_VEN,   EVGA_RTX3080_XC3_ULTRA_HYBRID_SUB_DEV,          0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3080 XC3 Ultra Hybrid LHR"          , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3080_LHR_DEV,     EVGA_SUB_VEN,   EVGA_RTX3080_XC3_ULTRA_HYBRID_LHR_SUB_DEV,      0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3080 XC3 Ultra Hydro Copper"        , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         EVGA_SUB_VEN,   EVGA_RTX3080_XC3_ULTRA_HC_SUB_DEV,              0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3080 FTW3 Gaming"                   , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         EVGA_SUB_VEN,   EVGA_RTX3080_FTW3_GAMING_SUB_DEV,               0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3080 FTW3 Ultra"                    , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         EVGA_SUB_VEN,   EVGA_RTX3080_FTW3_ULTRA_SUB_DEV,                0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3080 FTW3 Ultra v2 LHR"             , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3080_LHR_DEV,     EVGA_SUB_VEN,   EVGA_RTX3080_FTW3_ULTRA_LHR_V2_SUB_DEV,         0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3080 FTW3 Ultra LHR"                , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3080_LHR_DEV,     EVGA_SUB_VEN,   EVGA_RTX3080_FTW3_ULTRA_LHR_SUB_DEV,            0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3080 FTW3 Ultra Hybrid"             , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         EVGA_SUB_VEN,   EVGA_RTX3080_FTW3_ULTRA_HYBRID_SUB_DEV,         0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3080 FTW3 Ultra Hybrid LHR"         , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3080_LHR_DEV,     EVGA_SUB_VEN,   EVGA_RTX3080_FTW3_ULTRA_HYBRID_LHR_SUB_DEV,     0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3080 FTW3 Ultra Hybrid Gaming LHR"  , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3080_12G_LHR_DEV, EVGA_SUB_VEN,   EVGA_RTX3080_FTW3_ULTRA_HYBRID_GAMING_SUB_DEV,  0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3080 FTW3 Ultra Hydro Copper"       , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3080_DEV,         EVGA_SUB_VEN,   EVGA_RTX3080_FTW3_ULTRA_HC_SUB_DEV,             0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3080 XC3 Ultra 12G"                 , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3080_12G_LHR_DEV, EVGA_SUB_VEN,   EVGA_RTX3080_12G_XC3_ULTRA_SUB_DEV,             0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3080 FTW3 Ultra 12GB"               , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3080_12G_LHR_DEV, EVGA_SUB_VEN,   EVGA_RTX3080_12G_FTW3_ULTRA_SUB_DEV,            0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3080 FTW3 Ultra Hydro Copper 12G"   , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3080_12G_LHR_DEV, EVGA_SUB_VEN,   EVGA_RTX3080_12G_FTW3_ULTRA_HC_SUB_DEV,         0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3080 Ti XC3 Gaming"                 , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3080TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3080TI_XC3_GAMING_SUB_DEV,              0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3080 Ti XC3 Ultra Gaming"           , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3080TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3080TI_XC3_ULTRA_GAMING_SUB_DEV,        0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3080 Ti XC3 Gaming Hybrid"          , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3080TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3080TI_XC3_GAMING_HYBRID_SUB_DEV,       0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3080 Ti XC3 Gaming Hydro Copper"    , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3080TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3080TI_XC3_GAMING_HC_SUB_DEV,           0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3080 Ti FTW3 Ultra"                 , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3080TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3080TI_FTW3_ULTRA_SUB_DEV,              0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3080 Ti FTW3 Ultra Hybrid"          , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3080TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3080TI_FTW3_ULTRA_HYBRID_SUB_DEV,       0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3080 Ti FTW3 Ultra Hydro Copper"    , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3080TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3080TI_FTW3_ULTRA_HC_SUB_DEV,           0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3090 XC3 Black"                     , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_XC3_BLACK_SUB_DEV,                 0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3090 XC3 Gaming"                    , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_XC3_GAMING_SUB_DEV,                0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3090 XC3 Ultra"                     , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_XC3_ULTRA_SUB_DEV,                 0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3090 XC3 Ultra Hybrid"              , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_XC3_ULTRA_HYBRID_SUB_DEV,          0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3090 XC3 Ultra Hydro Copper"        , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_XC3_ULTRA_HC_SUB_DEV,              0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3090 FTW3 Ultra"                    , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_FTW3_ULTRA_SUB_DEV,                0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3090 FTW3 Ultra v2"                 , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_FTW3_ULTRA_V2_SUB_DEV,             0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3090 FTW3 Ultra v3"                 , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_FTW3_ULTRA_V3_SUB_DEV,             0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3090 FTW3 Ultra Hybrid"             , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_FTW3_ULTRA_HYBRID_SUB_DEV,         0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3090 FTW3 Ultra Hydro Copper"       , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_FTW3_ULTRA_HC_SUB_DEV,             0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3090 K|NGP|N Hybrid"                , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_KINGPIN_HYBRID_SUB_DEV,            0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3090 K|NGP|N Hydro Copper"          , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3090_DEV,         EVGA_SUB_VEN,   EVGA_RTX3090_KINGPIN_HC_SUB_DEV,                0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3090 Ti FTW3 Black Gaming"          , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3090TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3090TI_FTW3_BLACK_SUB_DEV,              0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3090 Ti FTW3 Gaming"                , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3090TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3090TI_FTW3_GAMING_SUB_DEV,             0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3090 Ti FTW3 Ultra Gaming"          , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3090TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3090TI_FTW3_ULTRA_GAMING_SUB_DEV,       0x2D);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 3090 Ti FTW3 Ultra Hybrid Gaming"   , DetectEVGAAmpereGPUControllers, NVIDIA_VEN,   NVIDIA_RTX3090TI_DEV,       EVGA_SUB_VEN,   EVGA_RTX3090TI_FTW3_ULTRA_HYBRID_GAMING_SUB_DEV,0x2D);
