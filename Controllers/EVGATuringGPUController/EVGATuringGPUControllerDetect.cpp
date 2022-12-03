#include "Detector.h"
#include "EVGAGPUv2Controller.h"
#include "RGBController.h"
#include "RGBController_EVGAGPUv2.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************************\
*                                                                                          *
*   DetectEVGATuringGPUControllers                                                         *
*                                                                                          *
*       Detect EVGA Turing GPU controllers on the enumerated I2C busses at address 0x49.   *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where EVGA GPU device is connected        *
*           dev - I2C address of EVGA GPU device                                           *
*                                                                                          *
\******************************************************************************************/

void DetectEVGATuringGPUControllers(i2c_smbus_interface* bus, uint8_t address, const std::string& name)
{
    if (bus->port_id == 1)
    {
        EVGAGPUv2Controller*     controller;
        RGBController_EVGAGPUv2* rgb_controller;

        controller              = new EVGAGPUv2Controller(bus, address);
        rgb_controller          = new RGBController_EVGAGPUv2(controller);
        rgb_controller->name    = name.c_str();
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectEVGATuringGPUControllers() */

REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 2070 XC Black"                        , DetectEVGATuringGPUControllers, NVIDIA_VEN,     NVIDIA_RTX2070_DEV,     EVGA_SUB_VEN,   EVGA_RTX2070_XC_BLACK_SUB_DEV,             0x49);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 2070 XC Gaming"                       , DetectEVGATuringGPUControllers, NVIDIA_VEN,     NVIDIA_RTX2070_OC_DEV,  EVGA_SUB_VEN,   EVGA_RTX2070_XC_GAMING_SUB_DEV,            0x49);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 2070 XC OC"                           , DetectEVGATuringGPUControllers, NVIDIA_VEN,     NVIDIA_RTX2070_OC_DEV,  EVGA_SUB_VEN,   EVGA_RTX2070_XC_OC_SUB_DEV,                0x49);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 2070 SUPER XC Ultra"                  , DetectEVGATuringGPUControllers, NVIDIA_VEN,     NVIDIA_RTX2070S_OC_DEV, EVGA_SUB_VEN,   EVGA_RTX2070S_XC_ULTRA_SUB_DEV,            0x49);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 2070 SUPER XC Ultra+"                 , DetectEVGATuringGPUControllers, NVIDIA_VEN,     NVIDIA_RTX2070S_OC_DEV, EVGA_SUB_VEN,   EVGA_RTX2070S_XC_ULTRA_PLUS_SUB_DEV,       0x49);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 2070 SUPER FTW3 Ultra"                , DetectEVGATuringGPUControllers, NVIDIA_VEN,     NVIDIA_RTX2070S_OC_DEV, EVGA_SUB_VEN,   EVGA_RTX2070S_FTW3_ULTRA_OC_SUB_DEV,       0x49);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 2070 SUPER FTW3 Ultra+"               , DetectEVGATuringGPUControllers, NVIDIA_VEN,     NVIDIA_RTX2070S_OC_DEV, EVGA_SUB_VEN,   EVGA_RTX2070S_FTW3_ULTRA_PLUS_OC_SUB_DEV,  0x49);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 2080 Black"                           , DetectEVGATuringGPUControllers, NVIDIA_VEN,     NVIDIA_RTX2080_DEV,     EVGA_SUB_VEN,   EVGA_RTX2080_BLACK_SUB_DEV,                0x49);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 2080 XC Black"                        , DetectEVGATuringGPUControllers, NVIDIA_VEN,     NVIDIA_RTX2080_A_DEV,   EVGA_SUB_VEN,   EVGA_RTX2080_XC_BLACK_SUB_DEV,             0x49);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 2080 XC Gaming"                       , DetectEVGATuringGPUControllers, NVIDIA_VEN,     NVIDIA_RTX2080_A_DEV,   EVGA_SUB_VEN,   EVGA_RTX2080_XC_GAMING_SUB_DEV,            0x49);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 2080 XC Ultra Gaming"                 , DetectEVGATuringGPUControllers, NVIDIA_VEN,     NVIDIA_RTX2080_A_DEV,   EVGA_SUB_VEN,   EVGA_RTX2080_XC_ULTRA_GAMING_SUB_DEV,      0x49);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 2080 SUPER XC Gaming"                 , DetectEVGATuringGPUControllers, NVIDIA_VEN,     NVIDIA_RTX2080S_DEV,    EVGA_SUB_VEN,   EVGA_RTX2080S_XC_GAMING_SUB_DEV,           0x49);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 2080 SUPER XC Ultra"                  , DetectEVGATuringGPUControllers, NVIDIA_VEN,     NVIDIA_RTX2080S_DEV,    EVGA_SUB_VEN,   EVGA_RTX2080S_XC_ULTRA_SUB_DEV,            0x49);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 2080 SUPER FTW3 Ultra"                , DetectEVGATuringGPUControllers, NVIDIA_VEN,     NVIDIA_RTX2080S_DEV,    EVGA_SUB_VEN,   EVGA_RTX2080S_FTW3_ULTRA_SUB_DEV,          0x49);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 2080 SUPER FTW3 Hybrid OC"            , DetectEVGATuringGPUControllers, NVIDIA_VEN,     NVIDIA_RTX2080S_DEV,    EVGA_SUB_VEN,   EVGA_RTX2080S_FTW3_HYBRID_OC_SUB_DEV,      0x49);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 2080 SUPER FTW3 Ultra Hydro Copper"   , DetectEVGATuringGPUControllers, NVIDIA_VEN,     NVIDIA_RTX2080S_DEV,    EVGA_SUB_VEN,   EVGA_RTX2080S_FTW3_ULTRA_HC_SUB_DEV,       0x49);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 2080Ti Black"                         , DetectEVGATuringGPUControllers, NVIDIA_VEN,     NVIDIA_RTX2080TI_DEV,   EVGA_SUB_VEN,   EVGA_RTX2080TI_BLACK_SUB_DEV,              0x49);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 2080Ti XC Ultra"                      , DetectEVGATuringGPUControllers, NVIDIA_VEN,     NVIDIA_RTX2080TI_A_DEV, EVGA_SUB_VEN,   EVGA_RTX2080TI_XC_ULTRA_GAMING_SUB_DEV,    0x49);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 2080Ti XC HYBRID GAMING"              , DetectEVGATuringGPUControllers, NVIDIA_VEN,     NVIDIA_RTX2080TI_A_DEV, EVGA_SUB_VEN,   EVGA_RTX2080TI_XC_HYBRID_GAMING_SUB_DEV,   0x49);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 2080Ti XC HYDRO COPPER"               , DetectEVGATuringGPUControllers, NVIDIA_VEN,     NVIDIA_RTX2080TI_DEV,   EVGA_SUB_VEN,   EVGA_RTX2080TI_XC_HYDRO_COPPER_SUB_DEV,    0x49);
REGISTER_I2C_PCI_DETECTOR("EVGA GeForce RTX 2080Ti FTW3 Ultra"                    , DetectEVGATuringGPUControllers, NVIDIA_VEN,     NVIDIA_RTX2080TI_A_DEV, EVGA_SUB_VEN,   EVGA_RTX2080TI_FTW3_ULTRA_SUB_DEV,         0x49);
