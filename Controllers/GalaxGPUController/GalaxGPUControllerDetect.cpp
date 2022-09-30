/*-----------------------------------------*\
|  GalaxGPUControllerDetect.cpp             |
|                                           |
|  Driver for Galax / KFA2 RGB on GPUs      |
|                                           |
|  Niels Westphal (crashniels)  12.07.2020  |
\*-----------------------------------------*/

#include "Detector.h"
#include "GalaxGPUController.h"
#include "LogManager.h"
#include "RGBController.h"
#include "RGBController_GalaxGPU.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************************\
*                                                                                          *
*   TestForGalaxGPUController                                                              *
*                                                                                          *
*       Tests the given address to see if a Galax GPU controller exists there.             *
*                                                                                          *
\******************************************************************************************/

bool TestForGalaxGPUController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    unsigned char res  = bus->i2c_smbus_read_byte_data(address, 0x00);
    unsigned char res2 = bus->i2c_smbus_read_byte_data(address, 0x01);
    
    if(res == 0x27 && res2 == 0x10)
    {
        pass = true;
    }

    return(pass);

}   /* TestForGalaxGPUController() */


/******************************************************************************************\
*                                                                                          *
*   DetectGalaxGPUControllers                                                              *
*                                                                                          *
*       Detect Galax GPU controllers on the enumerated I2C busses.                         *
*                                                                                          *
\******************************************************************************************/

void DetectGalaxGPUControllers(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name)
{
    if(TestForGalaxGPUController(bus, i2c_addr))
    {
        GalaxGPUController*     controller     = new GalaxGPUController(bus, i2c_addr);
        RGBController_GalaxGPU* rgb_controller = new RGBController_GalaxGPU(controller);
        rgb_controller->name                   = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
} /* DetectGalaxGPUControllers() */

REGISTER_I2C_PCI_DETECTOR("KFA2 RTX 2070 EX",                       DetectGalaxGPUControllers,  NVIDIA_VEN, NVIDIA_RTX2070_DEV,     NVIDIA_SUB_VEN, KFA2_RTX_2070_EX_SUB_DEV,               0x23);
REGISTER_I2C_PCI_DETECTOR("GALAX RTX 2070 Super EX Gamer Black",    DetectGalaxGPUControllers,  NVIDIA_VEN, NVIDIA_RTX2070S_OC_DEV, NVIDIA_SUB_VEN, GALAX_RTX_2070S_EX_GAMER_BLACK_SUB_DEV, 0x23);
REGISTER_I2C_PCI_DETECTOR("KFA2 RTX 2080 EX OC",                    DetectGalaxGPUControllers,  NVIDIA_VEN, NVIDIA_RTX2080_DEV,     NVIDIA_SUB_VEN, KFA2_RTX_2080_EX_OC_SUB_DEV,            0x23);
REGISTER_I2C_PCI_DETECTOR("KFA2 RTX 2080 Super EX OC",              DetectGalaxGPUControllers,  NVIDIA_VEN, NVIDIA_RTX2080S_DEV,    NVIDIA_SUB_VEN, KFA2_RTX_2080_SUPER_EX_OC_SUB_DEV,      0x23);
REGISTER_I2C_PCI_DETECTOR("KFA2 RTX 2080 TI EX OC",                 DetectGalaxGPUControllers,  NVIDIA_VEN, NVIDIA_RTX2080TI_DEV,   NVIDIA_SUB_VEN, KFA2_RTX_2080_TI_EX_OC_SUB_DEV,         0x23);
