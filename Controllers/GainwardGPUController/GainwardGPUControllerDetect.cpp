/*-----------------------------------------*\
|  GainwardGPUControllerDetect.cpp          |
|                                           |
|  Driver for Gainward RGB on GPUs          |
|                                           |
|  TheRogueZeta 11/05/2020                  |
\*-----------------------------------------*/

#include "Detector.h"
#include "GainwardGPUv1Controller.h"
#include "GainwardGPUv2Controller.h"
#include "LogManager.h"
#include "RGBController.h"
#include "RGBController_GainwardGPUv1.h"
#include "RGBController_GainwardGPUv2.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************************\
*                                                                                          *
*   TestForGainwardGPUController                                                           *
*                                                                                          *
*       Tests the given address to see if a Gainward GPU controller exists there.          *
*                                                                                          *
\******************************************************************************************/

bool TestForGainwardGPUController(i2c_smbus_interface* bus, uint8_t i2c_addr)
{
    bool pass = false;

    switch(i2c_addr)
    {
        /*-----------------------------------------------------------------*\
        | V1 Controller                                                     |
        \*-----------------------------------------------------------------*/
        case 0x08:
            pass = bus->i2c_smbus_write_quick(i2c_addr, I2C_SMBUS_WRITE);
            break;

        /*-----------------------------------------------------------------*\
        | V2 Controller                                                     |
        \*-----------------------------------------------------------------*/
        case 0x49:
            /*-------------------------------------------------------------*\
            | This detection might need some modifications                  |
            | Reading 0x6F*0x73 and comparing to 0x64 might be a possibility|
            \*-------------------------------------------------------------*/
            s32 data        = bus->i2c_smbus_read_byte_data(i2c_addr, 0x0);
            s32 mode_data   = bus->i2c_smbus_read_byte_data(i2c_addr, 0xe0);
            pass            = (data == 0x0) && (mode_data < 0x5);
            break;
    }

    return(pass);

}   /* TestForGainwardGPUController() */


/******************************************************************************************\
*                                                                                          *
*   DetectGainwardGPUControllers                                                           *
*                                                                                          *
*       Detect Gainward GPU controllers on the enumerated I2C busses.                      *
*                                                                                          *
\******************************************************************************************/

void DetectGainwardGPUControllers(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name)
{
    if(TestForGainwardGPUController(bus, i2c_addr))
    {
        switch(i2c_addr)
        {
            /*-----------------------------------------------------------------*\
            | V1 Controller                                                     |
            \*-----------------------------------------------------------------*/
            case 0x08:
                {
                    GainwardGPUv1Controller*     controller     = new GainwardGPUv1Controller(bus, i2c_addr);
                    RGBController_GainwardGPUv1* rgb_controller = new RGBController_GainwardGPUv1(controller);
                    rgb_controller->name                        = name;

                    ResourceManager::get()->RegisterRGBController(rgb_controller);
                }
                break;

            /*-----------------------------------------------------------------*\
            | V2 Controller                                                     |
            \*-----------------------------------------------------------------*/
            case 0x49:
                {
                    GainwardGPUv2Controller*     controller     = new GainwardGPUv2Controller(bus, i2c_addr);
                    RGBController_GainwardGPUv2* rgb_controller = new RGBController_GainwardGPUv2(controller);
                    rgb_controller->name                        = name;
                    
                    ResourceManager::get()->RegisterRGBController(rgb_controller);
                }
                break;
        }
    }
} /* DetectGainwardGPUControllers() */

REGISTER_I2C_PCI_DETECTOR("Gainward GTX 1080 Phoenix"      ,    DetectGainwardGPUControllers,   NVIDIA_VEN, NVIDIA_GTX1080_DEV,     GAINWARD_SUB_VEN,   GAINWARD_GTX_1080_PHOENIX,      0x08);
REGISTER_I2C_PCI_DETECTOR("Gainward GTX 1080 Ti Phoenix"   ,    DetectGainwardGPUControllers,   NVIDIA_VEN, NVIDIA_GTX1080TI_DEV,   GAINWARD_SUB_VEN,   GAINWARD_GTX_1080_TI_PHOENIX,   0x08);
REGISTER_I2C_PCI_DETECTOR("Gainward RTX 2070 Super Phantom",    DetectGainwardGPUControllers,   NVIDIA_VEN, NVIDIA_RTX2070S_OC_DEV, GAINWARD_SUB_VEN,   NVIDIA_RTX2070S_OC_DEV,         0x49);
REGISTER_I2C_PCI_DETECTOR("Gainward RTX 2080 Phoenix GS"   ,    DetectGainwardGPUControllers,   NVIDIA_VEN, NVIDIA_RTX2080_DEV,     GAINWARD_SUB_VEN,   NVIDIA_RTX2080_A_DEV,           0x49);
REGISTER_I2C_PCI_DETECTOR("Gainward RTX 3070 Phoenix"      ,    DetectGainwardGPUControllers,   NVIDIA_VEN, NVIDIA_RTX3070_DEV,     GAINWARD_SUB_VEN,   NVIDIA_RTX3070_DEV,             0x49);
REGISTER_I2C_PCI_DETECTOR("Gainward RTX 3070 Ti Phoenix"   ,    DetectGainwardGPUControllers,   NVIDIA_VEN, NVIDIA_RTX3070TI_DEV,   GAINWARD_SUB_VEN,   NVIDIA_RTX3070TI_DEV,           0x49);
REGISTER_I2C_PCI_DETECTOR("Gainward RTX 3080 Ti Phoenix"   ,    DetectGainwardGPUControllers,   NVIDIA_VEN, NVIDIA_RTX3080TI_DEV,   GAINWARD_SUB_VEN,   NVIDIA_RTX3080TI_DEV,           0x49);
REGISTER_I2C_PCI_DETECTOR("Gainward RTX 3090 Phoenix"      ,    DetectGainwardGPUControllers,   NVIDIA_VEN, NVIDIA_RTX3090_DEV,     GAINWARD_SUB_VEN,   NVIDIA_RTX3090_DEV,             0x49);
