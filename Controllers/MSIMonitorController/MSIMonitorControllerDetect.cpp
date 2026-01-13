/*---------------------------------------------------------*\
| MSIKeyboardControllerDetect.cpp                           |
|                                                           |
|   Detector for MSI  monitor (MSI Gaming Controller)       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "MSIMonitorController.h"
#include "RGBController_MSIMonitor.h"

#define MSI_USB_VID         0x1462
#define MSI_USB_PID         0x3FA4

DetectedControllers DetectMSIMonitorController(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        MSIMonitorController*     controller         = new MSIMonitorController(dev, *info, name);
        RGBController_MSIMonitor* rgb_controller     = new RGBController_MSIMonitor(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IPU("MSI Monitor (Gaming Controller)", DetectMSIMonitorController, MSI_USB_VID, MSI_USB_PID, 0, 0x01, 0);
