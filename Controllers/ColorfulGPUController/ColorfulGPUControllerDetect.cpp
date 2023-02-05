#include "Detector.h"
#include "ColorfulGPUController.h"
#include "RGBController.h"
#include "RGBController_ColorfulGPU.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

bool TestForColorfulGPU(i2c_smbus_interface* bus, uint8_t i2c_addr)
{
    int pktsz;
    const int read_sz  = 0x40;
    const int write_sz = 6;

    uint8_t data_pkt[write_sz] = { 0xAA, 0xEF, 0x81, 0x02, 0x1C, 0x02};
    bus->i2c_write_block(i2c_addr, write_sz, data_pkt);

    uint8_t read_pkt[read_sz] = {};
    pktsz = read_sz;

    int res = bus->i2c_read_block(i2c_addr, &pktsz, read_pkt);

    return res == 0 && (read_pkt[0] == 0xAA && read_pkt[1] == 0xEF && read_pkt[2] == 0x81);
}

void DetectColorfulGPUControllers(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name)
{
    if(TestForColorfulGPU(bus, i2c_addr))
    {
        ColorfulGPUController*     controller     = new ColorfulGPUController(bus, i2c_addr);
        RGBController_ColorfulGPU* rgb_controller = new RGBController_ColorfulGPU(controller);
        rgb_controller->name                      = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_I2C_PCI_DETECTOR("iGame GeForce RTX 3060 Ultra W OC 12G L-V",  DetectColorfulGPUControllers,   NVIDIA_VEN, NVIDIA_RTX3060_LHR_DEV,   COLORFUL_SUB_VEN,   COLORFUL_IGAME_RTX_3060_ULTRAW_OC_12G,      0x61);
REGISTER_I2C_PCI_DETECTOR("iGame GeForce RTX 3060 Ultra W OC 12G L-V",  DetectColorfulGPUControllers,   NVIDIA_VEN, NVIDIA_RTX3060_LHR_DEV,   COLORFUL_SUB_VEN,   COLORFUL_IGAME_RTX_3060_ULTRAW_OC_12G_2,    0x61);
REGISTER_I2C_PCI_DETECTOR("iGame GeForce RTX 3060 Ti Ultra W OC LHR-V", DetectColorfulGPUControllers,   NVIDIA_VEN, NVIDIA_RTX3060TI_LHR_DEV, COLORFUL_SUB_VEN,   COLORFUL_IGAME_RTX_3060_ULTRAW_OC_12G_2,    0x61);
REGISTER_I2C_PCI_DETECTOR("iGame GeForce RTX 3070 Advanced OC-V",       DetectColorfulGPUControllers,   NVIDIA_VEN, NVIDIA_RTX3070_DEV,       COLORFUL_SUB_VEN,   COLORFUL_IGAME_RTX_3070_ADVANCED_OCV,       0x61);
REGISTER_I2C_PCI_DETECTOR("iGame GeForce RTX 3070 Advanced OC-V",       DetectColorfulGPUControllers,   NVIDIA_VEN, NVIDIA_RTX3070_DEV,       COLORFUL_SUB_VEN,   COLORFUL_IGAME_RTX_3070_ADVANCED_OCV2,      0x61);
REGISTER_I2C_PCI_DETECTOR("iGame GeForce RTX 3070 Ultra W OC LHR",      DetectColorfulGPUControllers,   NVIDIA_VEN, NVIDIA_RTX3070_LHR_DEV,   COLORFUL_SUB_VEN,   COLORFUL_IGAME_RTX_3070_ULTRAW_OC,          0x61);
REGISTER_I2C_PCI_DETECTOR("iGame GeForce RTX 3070 Ti Advanced OC-V",    DetectColorfulGPUControllers,   NVIDIA_VEN, NVIDIA_RTX3070TI_DEV,     COLORFUL_SUB_VEN,   COLORFUL_IGAME_RTX_3070_TI_ADVANCED_OCV2,   0x61);
REGISTER_I2C_PCI_DETECTOR("iGame GeForce RTX 4080 Ultra W OC-V",        DetectColorfulGPUControllers,   NVIDIA_VEN, NVIDIA_RTX4080_DEV,       COLORFUL_SUB_VEN,   COLORFUL_IGAME_RTX_4080_ULTRAW_OCV2,        0x61);
