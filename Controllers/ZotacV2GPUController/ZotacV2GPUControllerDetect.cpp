#include "Detector.h"
#include "ZotacV2GPUController.h"
#include "RGBController.h"
#include "RGBController_ZotacV2GPU.h"
#include "i2c_smbus.h"
#include "pci_ids.h"

/******************************************************************************************\
*                                                                                          *
*   DetectZotacV2GPUControllers                                                            *
*                                                                                          *
*       Detect ZOTAC 30/40 series RGB controllers on the enumerated I2C busses             *
*       at address 0x49.                                                                   *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where RGB device is connected             *
*           dev - I2C address of RGB device                                                *
*                                                                                          *
\******************************************************************************************/

void DetectZotacV2GPUControllers(i2c_smbus_interface* bus, u8 i2c_addr, const std::string& name)
{
    ZotacV2GPUController*     controller     = new ZotacV2GPUController(bus, i2c_addr);
    RGBController_ZotacV2GPU* rgb_controller = new RGBController_ZotacV2GPU(controller);
    rgb_controller->name                     = name;

    if(rgb_controller->config.numberOfZones > 0)
    {
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_I2C_PCI_DETECTOR("ZOTAC GAMING GeForce RTX 3070 Ti Trinity OC",    DetectZotacV2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3070TI_DEV, ZOTAC_SUB_VEN, ZOTAC_RTX3070TI_TRINITY_SUB_DEV, 0x49);
REGISTER_I2C_PCI_DETECTOR("ZOTAC GAMING GeForce RTX 3080 Ti AMP Holo",      DetectZotacV2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3080TI_DEV, ZOTAC_SUB_VEN, ZOTAC_RTX3080TI_AMP_SUB_DEV, 0x49);
REGISTER_I2C_PCI_DETECTOR("ZOTAC GAMING GeForce RTX 3090 AMP Extreme Holo", DetectZotacV2GPUControllers, NVIDIA_VEN, NVIDIA_RTX3090_DEV, ZOTAC_SUB_VEN, ZOTAC_RTX3090_AMP_SUB_DEV, 0x49);
REGISTER_I2C_PCI_DETECTOR("ZOTAC GAMING GeForce RTX 4090 Trinity OC",       DetectZotacV2GPUControllers, NVIDIA_VEN, NVIDIA_RTX4090_DEV, ZOTAC_SUB_VEN, ZOTAC_RTX4090_TRINITY_SUB_DEV, 0x49);
REGISTER_I2C_PCI_DETECTOR("ZOTAC GAMING GeForce RTX 4090 AMP Extreme AIRO", DetectZotacV2GPUControllers, NVIDIA_VEN, NVIDIA_RTX4090_DEV, ZOTAC_SUB_VEN, ZOTAC_RTX4090_AMP_SUB_DEV, 0x49);
