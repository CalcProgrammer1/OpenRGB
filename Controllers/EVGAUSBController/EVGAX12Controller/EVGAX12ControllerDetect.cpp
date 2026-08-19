/*---------------------------------------------------------*\
| EVGAX12ControllerDetect.cpp                               |
|                                                           |
|   Detector for the EVGA X12 gaming mouse                  |
|                                                           |
|   QuickMythril                               08 Aug 2026  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "EVGAX12Controller.h"
#include "RGBController_EVGAX12.h"

#define EVGA_USB_VID                        0x3842
#define EVGA_X12_PID                        0x2422
#define EVGA_X12_INTERFACE                  1
#define EVGA_X12_USAGE_PAGE                 0x0008
#define EVGA_X12_USAGE                      0x004B

DetectedControllers DetectEVGAX12(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        EVGAX12Controller*     controller     = new EVGAX12Controller(dev, *info, name);
        RGBController_EVGAX12* rgb_controller = new RGBController_EVGAX12(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IPU("EVGA X12 Gaming Mouse", DetectEVGAX12, EVGA_USB_VID, EVGA_X12_PID, EVGA_X12_INTERFACE, EVGA_X12_USAGE_PAGE, EVGA_X12_USAGE);
