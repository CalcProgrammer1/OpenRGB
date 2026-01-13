/*---------------------------------------------------------*\
| MSI3ZoneControllerDetect.cpp                              |
|                                                           |
|   Detector for MSI/SteelSeries 3-Zone keyboard            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                25 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "MSI3ZoneController.h"
#include "RGBController_MSI3Zone.h"

#define MSI_3_ZONE_KEYBOARD_VID 0x1770
#define MSI_3_ZONE_KEYBOARD_PID 0xFF00

DetectedControllers DetectMSI3ZoneControllers(hid_device_info* info, const std::string&)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        MSI3ZoneController*     controller     = new MSI3ZoneController(dev, info->path);
        RGBController_MSI3Zone* rgb_controller = new RGBController_MSI3Zone(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR("MSI 3-Zone Laptop", DetectMSI3ZoneControllers, MSI_3_ZONE_KEYBOARD_VID, MSI_3_ZONE_KEYBOARD_PID);
