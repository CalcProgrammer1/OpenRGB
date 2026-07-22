/*---------------------------------------------------------*\
| HoltekA1FAControllerDetect.cpp                            |
|                                                           |
|   Detector for Holtek A1FA Mousemat                       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "HoltekA1FAController.h"
#include "RGBController_HoltekA1FA.h"

/*---------------------------------------------------------*\
| Holtek Semiconductor Inc. vendor ID                       |
\*---------------------------------------------------------*/
#define HOLTEK_VID                                  0x04D9

/*---------------------------------------------------------*\
| Mousemats product IDs                                     |
\*---------------------------------------------------------*/
#define HOLTEK_A1FA_PID                             0xA1FA

DetectedControllers DetectHoltekA1FAControllers(hid_device_info *info, const std::string &name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        HoltekA1FAController*     controller     = new HoltekA1FAController(dev, info->path, name);
        RGBController_HoltekA1FA* rgb_controller = new RGBController_HoltekA1FA(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IPU("Holtek Mousemat", DetectHoltekA1FAControllers, HOLTEK_VID, HOLTEK_A1FA_PID, 2, 0xFF00, 0xFF00);
