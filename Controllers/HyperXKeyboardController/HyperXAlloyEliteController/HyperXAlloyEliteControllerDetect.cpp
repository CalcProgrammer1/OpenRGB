/*---------------------------------------------------------*\
| HyperXAlloyEliteControllerDetect.cpp                      |
|                                                           |
|   Driver for HyperX Alloy Elite keyboards                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "HyperXAlloyEliteController.h"
#include "RGBController_HyperXAlloyElite.h"

/*---------------------------------------------------------*\
| HyperX keyboard vendor and product IDs                    |
\*---------------------------------------------------------*/
#define HYPERX_KEYBOARD_VID                         0x0951
#define HYPERX_ALLOY_ELITE_PID                      0x16BE

DetectedControllers DetectHyperXAlloyElite(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        HyperXAlloyEliteController*     controller     = new HyperXAlloyEliteController(dev, info->path, name);
        RGBController_HyperXAlloyElite* rgb_controller = new RGBController_HyperXAlloyElite(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IP("HyperX Alloy Elite RGB", DetectHyperXAlloyElite, HYPERX_KEYBOARD_VID, HYPERX_ALLOY_ELITE_PID, 2, 0xFF01);
