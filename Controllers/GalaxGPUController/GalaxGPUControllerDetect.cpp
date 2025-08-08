/*---------------------------------------------------------*\
| GalaxGPUControllerDetect.cpp                              |
|                                                           |
|   Detector for Galax/KFA2 GPU                             |
|                                                           |
|   Niels Westphal (crashniels)                 12 Jul 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "GalaxGPUv1Controller.h"
#include "GalaxGPUv2Controller.h"
#include "RGBController_GalaxGPUv1.h"
#include "RGBController_GalaxGPUv2.h"
#include "i2c_smbus.h"
#include "pci_ids.h"

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
    unsigned char res;

    switch (address)
    {
        /*-----------------------------------------------------------------*\
        | V1 Controller                                                     |
        \*-----------------------------------------------------------------*/
        case 0x32:
            res  = bus->i2c_smbus_read_byte_data(address, 0x00);
            if(res == 0x27 || res == 0x26) {
                res = bus->i2c_smbus_read_byte_data(address, 0x01);
                if (res == 0x10 || res == 0x20)
                    pass = true;
            }
            break;

        /*-----------------------------------------------------------------*\
        | V1 Controller - RTX 3080                                          |
        \*-----------------------------------------------------------------*/
        case 0x23:
            res = bus->i2c_smbus_read_byte_data(address, 0x00);
            if(res == 0x30)
            {
                pass = true;
            }
            break;

        /*-----------------------------------------------------------------*\
        | V2 Controller                                                     |
        \*-----------------------------------------------------------------*/
        case 0x51:
            res = bus->i2c_smbus_read_byte_data(address, 0x00);
            if (res == 0x80)
                pass = true;
            break;
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
        switch(i2c_addr)
        {
            /*-----------------------------------------------------------------*\
            | V1 Controller                                                     |
            \*-----------------------------------------------------------------*/
            case 0x32:
            case 0x23:
                {
                    GalaxGPUv1Controller*     controller     = new GalaxGPUv1Controller(bus, i2c_addr, name);
                    RGBController_GalaxGPUv1* rgb_controller = new RGBController_GalaxGPUv1(controller);

                    ResourceManager::get()->RegisterRGBController(rgb_controller);
                }
                break;

            /*-----------------------------------------------------------------*\
            | V2 Controller                                                     |
            \*-----------------------------------------------------------------*/
            case 0x51:
                {
                    GalaxGPUv2Controller*     controller     = new GalaxGPUv2Controller(bus, i2c_addr, name);
                    RGBController_GalaxGPUv2* rgb_controller = new RGBController_GalaxGPUv2(controller);

                    ResourceManager::get()->RegisterRGBController(rgb_controller);
                }
                break;
        }
    }
} /* DetectGalaxGPUControllers() */

REGISTER_I2C_PCI_DETECTOR("KFA2 GeForce RTX 2070 EX",                       DetectGalaxGPUControllers,  NVIDIA_VEN, NVIDIA_RTX2070_DEV,     NVIDIA_SUB_VEN, KFA2_RTX_2070_EX_SUB_DEV,               0x23);
REGISTER_I2C_PCI_DETECTOR("KFA2 GeForce RTX 2070 OC",                       DetectGalaxGPUControllers,  NVIDIA_VEN, NVIDIA_RTX2070_OC_DEV,  NVIDIA_SUB_VEN, KFA2_RTX_2070_OC_SUB_DEV,               0x23);
REGISTER_I2C_PCI_DETECTOR("GALAX GeForce RTX 2070 SUPER EX Gamer Black",    DetectGalaxGPUControllers,  NVIDIA_VEN, NVIDIA_RTX2070S_OC_DEV, NVIDIA_SUB_VEN, GALAX_RTX_2070S_EX_GAMER_BLACK_SUB_DEV, 0x23);
REGISTER_I2C_PCI_DETECTOR("KFA2 GeForce RTX 2080 EX OC",                    DetectGalaxGPUControllers,  NVIDIA_VEN, NVIDIA_RTX2080_DEV,     NVIDIA_SUB_VEN, KFA2_RTX_2080_EX_OC_SUB_DEV,            0x23);
REGISTER_I2C_PCI_DETECTOR("KFA2 GeForce RTX 2080 SUPER EX OC",              DetectGalaxGPUControllers,  NVIDIA_VEN, NVIDIA_RTX2080S_DEV,    NVIDIA_SUB_VEN, KFA2_RTX_2080_SUPER_EX_OC_SUB_DEV,      0x23);
REGISTER_I2C_PCI_DETECTOR("KFA2 GeForce RTX 2080 Ti EX OC",                 DetectGalaxGPUControllers,  NVIDIA_VEN, NVIDIA_RTX2080TI_DEV,   NVIDIA_SUB_VEN, KFA2_RTX_2080TI_EX_OC_SUB_DEV,          0x23);
REGISTER_I2C_PCI_DETECTOR("GALAX GeForce RTX 3080 SG",                      DetectGalaxGPUControllers,  NVIDIA_VEN, NVIDIA_RTX3080_DEV,     NVIDIA_SUB_VEN, GALAX_RTX_3080_SG_SUB_DEV,              0x23);
REGISTER_I2C_PCI_DETECTOR("GALAX GeForce RTX 5070 Ti EX Gamer 1-Click OC",  DetectGalaxGPUControllers,  NVIDIA_VEN, NVIDIA_RTX5070TI_DEV,   NVIDIA_SUB_VEN, GALAX_RTX_5070TI_EX_OC_SUB_DEV,         0x51);
