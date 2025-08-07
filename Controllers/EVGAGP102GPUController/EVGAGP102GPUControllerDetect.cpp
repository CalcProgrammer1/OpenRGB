/*---------------------------------------------------------*\
| EVGAGP102ControllerDetect.cpp                             |
|                                                           |
|   Detector for EVGA GP102 GPU                             |
|                                                           |
|   Fabricio Murta (avengerx)                   31 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <vector>
#include "Detector.h"
#include "EVGAGP102Controller.h"
#include "LogManager.h"
#include "RGBController_EVGAGP102.h"
#include "i2c_smbus.h"
#include "pci_ids.h"

/******************************************************************************************\
*                                                                                          *
*   DetectEVGAGP102GPUControllers                                                          *
*                                                                                          *
*       Detect EVGA GP102 GPU controllers on the enumerated I2C busses at address 0x49.    *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where EVGA GPU device is connected        *
*           address - unused, the address comes from the GPU zone info table               *
*           name - name string of detected PCI device                                      *
*                                                                                          *
\******************************************************************************************/

void DetectEVGAGP102GPUControllers(i2c_smbus_interface* bus, uint8_t /*address*/, const std::string& name)
{
    if(bus->port_id == 1)
    {
        RGBController_EVGAGP102*            new_rgbcontroller;
        std::vector<EVGAGP102Controller*>   controllers;

        for(unsigned int i = 0; i < sizeof(gpuzoneinfos) / sizeof(zoneinfo); i++)
        {
            EVGAGP102Controller* controller = new EVGAGP102Controller(bus, gpuzoneinfos[i], name);

            if(controller->IsValid())
            {
                controllers.push_back(controller);
            }
            else
            {
                delete controller;
            }
        }

        if(controllers.size() != 0)
        {
            new_rgbcontroller = new RGBController_EVGAGP102(controllers);

            ResourceManager::get()->RegisterRGBController(new_rgbcontroller);
        }
    }
}   /* DetectEVGAGP102GPUControllers() */

/*---------------------------------------------------------*\
| The I2C address is provided by the GPU Zone Info table,   |
| as these GPUs have multiple I2C devices per card.         |
\*---------------------------------------------------------*/
REGISTER_I2C_PCI_DETECTOR( "EVGA GeForce GTX 1070 FTW2 Gaming",     DetectEVGAGP102GPUControllers,  NVIDIA_VEN, NVIDIA_GTX1070_DEV,     EVGA_SUB_VEN,   EVGA_GTX1070_FTW2_SUB_DEV,          0x00 );
REGISTER_I2C_PCI_DETECTOR( "EVGA GeForce GTX 1080 FTW2 Gaming",     DetectEVGAGP102GPUControllers,  NVIDIA_VEN, NVIDIA_GTX1080_DEV,     EVGA_SUB_VEN,   EVGA_GTX1080_FTW2_SUB_DEV,          0x00 );
REGISTER_I2C_PCI_DETECTOR( "EVGA GeForce GTX 1080 FTW2 11G",        DetectEVGAGP102GPUControllers,  NVIDIA_VEN, NVIDIA_GTX1080_DEV,     EVGA_SUB_VEN,   EVGA_GTX1080_FTW2_11G_SUB_DEV,      0x00 );
REGISTER_I2C_PCI_DETECTOR( "EVGA GeForce GTX 1080 FTW2 DT",         DetectEVGAGP102GPUControllers,  NVIDIA_VEN, NVIDIA_GTX1080_DEV,     EVGA_SUB_VEN,   EVGA_GTX1080_FTW2_DT_SUB_DEV,       0x00 );
REGISTER_I2C_PCI_DETECTOR( "EVGA GeForce GTX 1080 Ti SC2 Gaming",   DetectEVGAGP102GPUControllers,  NVIDIA_VEN, NVIDIA_GTX1080TI_DEV,   EVGA_SUB_VEN,   EVGA_GTX1080TI_SC2_GAMING_SUB_DEV,  0x00 );
REGISTER_I2C_PCI_DETECTOR( "EVGA GeForce GTX 1080 Ti FTW3",         DetectEVGAGP102GPUControllers,  NVIDIA_VEN, NVIDIA_GTX1080TI_DEV,   EVGA_SUB_VEN,   EVGA_GTX1080TI_FTW3_SUB_DEV,        0x00 );
REGISTER_I2C_PCI_DETECTOR( "EVGA GeForce GTX 1080 Ti FTW3 Hybrid",  DetectEVGAGP102GPUControllers,  NVIDIA_VEN, NVIDIA_GTX1080TI_DEV,   EVGA_SUB_VEN,   EVGA_GTX1080TI_FTW3_HYBRID_SUB_DEV, 0x00 );
REGISTER_I2C_PCI_DETECTOR( "EVGA GeForce GTX 1080 Ti K|NGP|N",      DetectEVGAGP102GPUControllers,  NVIDIA_VEN, NVIDIA_GTX1080TI_DEV,   EVGA_SUB_VEN,   EVGA_GTX1080TI_KINGPIN_SUB_DEV,     0x00 );
