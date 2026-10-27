/*---------------------------------------------------------*\
| AlienwareKeyboardControllerDetect.cpp                     |
|                                                           |
|   Detector for Alienware keyboards                        |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "AlienwareKeyboardController.h"
#include "RGBController_AlienwareKeyboard.h"
#include "AlienwareDevices.h"

DetectedControllers DetectAlienwareAW410KControllers(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        AlienwareKeyboardController*     controller     = new AlienwareKeyboardController(dev, info->path, name);
        RGBController_AlienwareKeyboard* rgb_controller = new RGBController_AlienwareKeyboard(controller, GetAlienwareDevice(ALIENWARE_AW410K_PID));

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

DetectedControllers DetectAlienwareAW510KControllers(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        AlienwareKeyboardController*     controller     = new AlienwareKeyboardController(dev, info->path, name);
        RGBController_AlienwareKeyboard* rgb_controller = new RGBController_AlienwareKeyboard(controller, GetAlienwareDevice(ALIENWARE_AW510K_PID));

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IPU("Alienware AW410K",   DetectAlienwareAW410KControllers,   ALIENWARE_VID,  ALIENWARE_AW410K_PID,   0x02,   0xFF00, 0x01);
REGISTER_HID_DETECTOR_IPU("Alienware AW510K",   DetectAlienwareAW510KControllers,   ALIENWARE_VID,  ALIENWARE_AW510K_PID,   0x02,   0xFF00, 0x01);
