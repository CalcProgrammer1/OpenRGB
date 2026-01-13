/*---------------------------------------------------------*\
| EVGAGP102ControllerDetect.cpp                             |
|                                                           |
|   Detector for EVGA GP102 GPU                             |
|                                                           |
|   Fabricio Murta (avengerx)                   31 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <vector>
#include "DetectionManager.h"
#include "EVGAGP102Controller.h"
#include "i2c_smbus.h"
#include "LogManager.h"
#include "pci_ids.h"
#include "RGBController_EVGAGP102.h"

DetectedControllers DetectEVGAGP102GPUControllers(i2c_smbus_interface* bus, uint8_t /*address*/, const std::string& name)
{
    DetectedControllers detected_controllers;

    if(bus->port_id == 1)
    {
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
            RGBController_EVGAGP102* rgb_controller = new RGBController_EVGAGP102(controllers);

            detected_controllers.push_back(rgb_controller);
        }
    }

    return(detected_controllers);
}

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
