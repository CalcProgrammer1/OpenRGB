/*---------------------------------------------------------*\
| GigabyteAorusPCCaseControllerDetect.cpp                   |
|                                                           |
|   Detector for Gigabyte Aorus case                        |
|                                                           |
|   Denis Nazarov (nenderus)                    10 Feb 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "GigabyteAorusPCCaseController.h"
#include "RGBController_GigabyteAorusPCCase.h"

/*---------------------------------------------------------*\
| Vendor ID                                                 |
\*---------------------------------------------------------*/
#define HOLTEK_VID                          0x1044

/*---------------------------------------------------------*\
| Controller product ids                                    |
\*---------------------------------------------------------*/
#define C300_GLASS_PID                      0x7A30

DetectedControllers DetectGigabyteAorusPCCaseControllers(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        GigabyteAorusPCCaseController*      controller      = new GigabyteAorusPCCaseController(dev, info->path, name);
        RGBController_GigabyteAorusPCCase*  rgb_controller  = new RGBController_GigabyteAorusPCCase(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IPU("Gigabyte AORUS C300 GLASS",  DetectGigabyteAorusPCCaseControllers,   HOLTEK_VID, C300_GLASS_PID, 0,  0xFF01, 0x01);
