/*-----------------------------------------*\
|  MSIGPUControllerDetect.cpp               |
|                                           |
|  Driver for MSI GPUs                      |
|                                           |
\*-----------------------------------------*/

#include "Detector.h"
#include "LogManager.h"
#include "i2c_smbus.h"
#include "pci_ids.h"

#include "RGBController_MSIGPUv2.h"
#include "MSIGPUv2Controller.h"

/*-----------------------------------------------------------------------------------------*\
|                                                                                           |
|   DetectMSI GPU V2 Controllers                                                            |
|                                                                                           |
|       Detect MSI GPU v2 controllers on the enumerated I2C busses.                         |
|                                                                                           |
\*-----------------------------------------------------------------------------------------*/

void DetectMSIGPUv2Controllers(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name)
{
    if(bus->pci_vendor == NVIDIA_VEN && bus->port_id != 1)
    {
        return;
    }

        MSIGPUv2Controller*     controller      = new MSIGPUv2Controller(bus, i2c_addr);
        RGBController_MSIGPUv2* rgb_controller  = new RGBController_MSIGPUv2(controller);
        rgb_controller->name                    = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
        LOG_TRACE("MSI GPU Controler using ITE9 chip");


} /* DetectMSIGPUv2Controllers() */

REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4070Ti 12GB Gaming X Trio",      DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4070TI_DEV,       MSI_SUB_VEN,    MSI_RTX4070TI_GAMING_X_TRIO_SUB_DEV,    0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4070Ti 12GB Suprim X Trio",      DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4070TI_DEV,       MSI_SUB_VEN,    MSI_RTX4070TI_SUPRIM_X_TRIO_SUB_DEV,    0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4080 16GB Gaming X Trio",        DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4080_DEV,         MSI_SUB_VEN,    MSI_RTX4080_GAMING_X_TRIO_SUB_DEV,      0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4080 16GB Suprim X",             DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4080_DEV,         MSI_SUB_VEN,    MSI_RTX4080_SUPRIM_X_SUB_DEV,           0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4090 24GB Gaming X Trio",        DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         MSI_SUB_VEN,    MSI_RTX4090_GAMING_X_TRIO_SUB_DEV,      0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4090 24GB Suprim Liquid X",      DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         MSI_SUB_VEN,    MSI_RTX4090_SUPRIM_LIQUID_X_SUB_DEV,    0x68);
REGISTER_I2C_PCI_DETECTOR("MSI GeForce RTX 4090 24GB Suprim X",             DetectMSIGPUv2Controllers,    NVIDIA_VEN,     NVIDIA_RTX4090_DEV,         MSI_SUB_VEN,    MSI_RTX4090_SUPRIM_X_SUB_DEV,           0x68);
