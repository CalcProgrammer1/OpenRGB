/*---------------------------------------------------------*\
| MSIClutchControllerDetect.cpp                             |
|                                                           |
|   Detector for MSI Clutch Gaming Mouse                    |
|                                                           |
|   Anatoly U. (TTolikoLL)                      20 SEP 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "MSIClutchGM11Controller.h"
#include "RGBController_MSIClutchGM11.h"

#define MSI_VID                 0x0DB0
#define MSI_CLUTCH_GM11_PID     0x0D11

DetectedControllers DetectMSIClutchGM11Controllers(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        MSIClutchGM11Controller* controller = new MSIClutchGM11Controller(dev, *info, name);
        RGBController_MSIClutchGM11* rgb_controller = new RGBController_MSIClutchGM11(controller);
        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IPU("MSI Clutch GM11 controller", DetectMSIClutchGM11Controllers, MSI_VID, MSI_CLUTCH_GM11_PID, 2, 0xFF00, 0xFF00);