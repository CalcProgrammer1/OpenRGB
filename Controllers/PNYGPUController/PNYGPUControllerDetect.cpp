#include "Detector.h"
#include "LogManager.h"
#include "PNYGPUController.h"
#include "RGBController.h"
#include "RGBController_PNYGPU.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

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

    PNYGPUController*     controller        = new PNYGPUController(bus, i2c_addr);
    RGBController_PNYGPU* rgb_controller    = new RGBController_PNYGPU(controller);
    rgb_controller->name                    = name;
    rgb_controller->vendor                  = name.substr(0, name.find(' '));

    ResourceManager::get()->RegisterRGBController(rgb_controller);
} /* DetectPNYGPUControllers() */

REGISTER_I2C_PCI_DETECTOR("PNY XLR8 Revel EPIC-X RTX 3060",     DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3060_DEV,         PNY_SUB_VEN,    PNY_RTX_3060_XLR8_REVEL_EPIC_X_SUB_DEV, 0x49);
REGISTER_I2C_PCI_DETECTOR("PNY XLR8 Revel EPIC-X RTX 3070",     DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3070_DEV,         PNY_SUB_VEN,    PNY_RTX_3070_XLR8_REVEL_EPIC_X_SUB_DEV, 0x49);
REGISTER_I2C_PCI_DETECTOR("PNY XLR8 Revel EPIC-X RTX 3080",     DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3080_DEV,         PNY_SUB_VEN,    PNY_RTX_3080_XLR8_REVEL_EPIC_X_SUB_DEV, 0x49);
REGISTER_I2C_PCI_DETECTOR("PNY XLR8 Revel EPIC-X RTX 3090",     DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3090_DEV,         PNY_SUB_VEN,    PNY_RTX_3090_XLR8_REVEL_EPIC_X_SUB_DEV, 0x49);
REGISTER_I2C_PCI_DETECTOR("PNY XLR8 OC EDITION RTX 2060",       DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX2060_TU104_DEV,   PNY_SUB_VEN,    PNY_RTX_2060_XLR8_OC_SUB_DEV,           0x49);
REGISTER_I2C_PCI_DETECTOR("Palit 3060",                         DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3060_DEV,         PALIT_SUB_VEN,  PALIT_RTX3060_SUB_DEV,                  0x49);
REGISTER_I2C_PCI_DETECTOR("Palit 3060 LHR",                     DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3060_LHR_DEV,     PALIT_SUB_VEN,  PALIT_RTX3060_LHR_SUB_DEV,              0x49);
REGISTER_I2C_PCI_DETECTOR("Palit 3060Ti",                       DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3060TI_DEV,       PALIT_SUB_VEN,  PALIT_RTX3060TI_SUB_DEV,                0x49);
REGISTER_I2C_PCI_DETECTOR("Palit 3060TI LHR",                   DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3060TI_LHR_DEV,   PALIT_SUB_VEN,  NVIDIA_RTX3060TI_LHR_DEV,               0x49);
REGISTER_I2C_PCI_DETECTOR("Palit 3070",                         DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3070_DEV,         PALIT_SUB_VEN,  PALIT_RTX3070_SUB_DEV,                  0x49);
REGISTER_I2C_PCI_DETECTOR("Palit 3070 LHR",                     DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3070_LHR_DEV,     PALIT_SUB_VEN,  PALIT_RTX3070_LHR_SUB_DEV,              0x49);
REGISTER_I2C_PCI_DETECTOR("Palit 3070Ti GamingPro",             DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3070TI_DEV,       PALIT_SUB_VEN,  PALIT_RTX3070TI_GAMING_PRO_SUB_DEV,     0x49);
REGISTER_I2C_PCI_DETECTOR("Palit 3070Ti",                       DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3070TI_DEV,       PALIT_SUB_VEN,  PALIT_RTX3070TI_SUB_DEV,                0x49);
REGISTER_I2C_PCI_DETECTOR("Palit 3080",                         DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3080_DEV,         PALIT_SUB_VEN,  PALIT_RTX3080_SUB_DEV,                  0x49);
REGISTER_I2C_PCI_DETECTOR("Palit 3080 Gamerock",                DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3080_DEV,         PALIT_SUB_VEN,  PALIT_RTX3080_GAMEROCK_SUB_DEV,         0x49);
REGISTER_I2C_PCI_DETECTOR("Palit 3080 Gamerock LHR",            DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3080_LHR_DEV,     PALIT_SUB_VEN,  PALIT_RTX3080_GAMEROCK_SUB_DEV,         0x49);
REGISTER_I2C_PCI_DETECTOR("Palit 3080 LHR",                     DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3080_LHR_DEV,     PALIT_SUB_VEN,  PALIT_RTX3080_LHR_SUB_DEV,              0x49);
REGISTER_I2C_PCI_DETECTOR("Palit 3080Ti",                       DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3080TI_DEV,       PALIT_SUB_VEN,  PALIT_RTX3080TI_SUB_DEV,                0x49);
REGISTER_I2C_PCI_DETECTOR("Palit 3080Ti Gamerock",              DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3080TI_DEV,       PALIT_SUB_VEN,  PALIT_RTX3080TI_GAMEROCK_SUB_DEV,       0x49);
REGISTER_I2C_PCI_DETECTOR("Palit 3090",                         DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3090_DEV,         PALIT_SUB_VEN,  PALIT_RTX3090_SUB_DEV,                  0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3060 Ti Dual",     DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX3060TI_DEV,       NVIDIA_SUB_VEN, PALIT_RTX3060TI_SUB_DEV,                0x49);
REGISTER_I2C_PCI_DETECTOR("NVIDIA RTX2080S",                    DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX2080S_DEV,        NVIDIA_SUB_VEN, NVIDIA_RTX2080S_DEV,                    0x49);
REGISTER_I2C_PCI_DETECTOR("NVIDIA RTX2060S",                    DetectPNYGPUControllers,    NVIDIA_VEN, NVIDIA_RTX2060_TU106_DEV,        NVIDIA_SUB_VEN, NVIDIA_RTX2060_TU106_DEV,                    0x49);
