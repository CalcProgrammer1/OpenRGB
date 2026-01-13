/*---------------------------------------------------------*\
| OKSKeyboardControllerDetect.cpp                           |
|                                                           |
|   Detector for OKS keyboard                               |
|                                                           |
|   Merafour (OKS)                              24 Feb 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "OKSKeyboardController.h"
#include "RGBController_OKSKeyboard.h"

DetectedControllers DetectOKSKeyboardControllers(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        OKSKeyboardController*     controller     = new OKSKeyboardController(dev, info->path, info->product_id, name);
        RGBController_OKSKeyboard* rgb_controller = new RGBController_OKSKeyboard(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_I("OKS Optical Axis RGB",           DetectOKSKeyboardControllers, OKS_VID, OKS_OPTICAL_RGB_PID,   1);
