/*---------------------------------------------------------*\
| HoltekA070ControllerDetect.cpp                            |
|                                                           |
|   Detector for Holtek A070 Mouse                          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "HoltekA070Controller.h"
#include "RGBController_HoltekA070.h"

/*---------------------------------------------------------*\
| Holtek Semiconductor Inc. vendor ID                       |
\*---------------------------------------------------------*/
#define HOLTEK_VID                                  0x04D9

/*---------------------------------------------------------*\
| Mouse product IDs                                         |
\*---------------------------------------------------------*/
#define HOLTEK_A070_PID                             0xA070

DetectedControllers DetectHoltekA070Controllers(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        HoltekA070Controller*     controller     = new HoltekA070Controller(dev, info->path, name);
        RGBController_HoltekA070* rgb_controller = new RGBController_HoltekA070(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IPU("Holtek USB Gaming Mouse", DetectHoltekA070Controllers, HOLTEK_VID, HOLTEK_A070_PID, 1, 0xFF00, 2);
