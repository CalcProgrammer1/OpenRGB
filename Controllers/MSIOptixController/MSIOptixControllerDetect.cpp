/*---------------------------------------------------------*\
| MSIOptixControllerDetect.cpp                              |
|                                                           |
|   Detector for MSI Optix                                  |
|                                                           |
|   Morgan Guimard (morg)                       10 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "MSIOptixController.h"
#include "RGBController_MSIOptix.h"

/*---------------------------------------------------------*\
| MSI vendor ID                                             |
\*---------------------------------------------------------*/
#define MSI_VID                                        0x1462

/*---------------------------------------------------------*\
| Product ID                                                |
\*---------------------------------------------------------*/
#define MSI_OPTIX_MAG274QRF_PID                        0x3FA4

DetectedControllers DetectMSIOptixControllers(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        MSIOptixController*     controller      = new MSIOptixController(dev, *info, name);
        RGBController_MSIOptix* rgb_controller  = new RGBController_MSIOptix(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IPU("MSI Optix controller", DetectMSIOptixControllers, MSI_VID, MSI_OPTIX_MAG274QRF_PID, 0, 0xFF00, 1);
