/*---------------------------------------------------------*\
| HyperXAlloyFPSControllerDetect.cpp                        |
|                                                           |
|   Driver for HyperX Alloy FPS keyboards                   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "HyperXAlloyFPSController.h"
#include "RGBController_HyperXAlloyFPS.h"

/*---------------------------------------------------------*\
| HyperX keyboard vendor and product IDs                    |
\*---------------------------------------------------------*/
#define HYPERX_KEYBOARD_VID                         0x0951
#define HYPERX_ALLOY_FPS_RGB_PID                    0x16DC

DetectedControllers DetectHyperXAlloyFPS(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        HyperXAlloyFPSController*     controller     = new HyperXAlloyFPSController(dev, info->path, name);
        RGBController_HyperXAlloyFPS* rgb_controller = new RGBController_HyperXAlloyFPS(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IP("HyperX Alloy FPS RGB", DetectHyperXAlloyFPS, HYPERX_KEYBOARD_VID, HYPERX_ALLOY_FPS_RGB_PID, 2, 0xFF01);
