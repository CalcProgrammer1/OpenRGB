/*---------------------------------------------------------*\
| AMDWraithPrismControllerDetect.cpp                        |
|                                                           |
|   Detector for AMD Wraith Prism                           |
|                                                           |
|   Adam Honse (CalcProgrammer1)                06 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "AMDWraithPrismController.h"
#include "DetectionManager.h"
#include "RGBController_AMDWraithPrism.h"

/*---------------------------------------------------------*\
| AMD Wraith Prism vendor ID                                |
\*---------------------------------------------------------*/
#define AMD_WRAITH_PRISM_VID                        0x2516

/*---------------------------------------------------------*\
| AMD Wraith Prism product ID                               |
\*---------------------------------------------------------*/
#define AMD_WRAITH_PRISM_PID                        0x0051

DetectedControllers DetectAMDWraithPrismControllers(hid_device_info* info, const std::string&)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        AMDWraithPrismController*     controller     = new AMDWraithPrismController(dev, info->path);
        RGBController_AMDWraithPrism* rgb_controller = new RGBController_AMDWraithPrism(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IP("AMD Wraith Prism", DetectAMDWraithPrismControllers, AMD_WRAITH_PRISM_VID, AMD_WRAITH_PRISM_PID, 1, 0xFF00);
