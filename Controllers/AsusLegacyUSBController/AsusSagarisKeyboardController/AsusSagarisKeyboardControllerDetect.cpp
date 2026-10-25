/*---------------------------------------------------------*\
| AsusSagarisKeyboardControllerDetect.cpp                   |
|                                                           |
|   Detector for ASUS Sagaris keyboard devices              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "AsusSagarisKeyboardController.h"
#include "RGBController_AsusSagarisKeyboard.h"

/*---------------------------------------------------------*\
| ASUS VID                                                  |
\*---------------------------------------------------------*/
#define ASUS_USB_VID                                0x0B05

/*---------------------------------------------------------*\
| ASUS Sagaris GK1100 PID                                   |
\*---------------------------------------------------------*/
#define ASUS_SAGARIS_GK1100_PID                     0x1835

DetectedControllers DetectAsusSagarisKeyboard(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        AsusSagarisKeyboardController*     controller          = new AsusSagarisKeyboardController(dev, info->path, info->release_number, name);
        RGBController_AsusSagarisKeyboard* rgb_controller      = new RGBController_AsusSagarisKeyboard(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

/*---------------------------------------------------------*\
| ASUS Sagaris GK1100 Detector                              |
\*---------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("ASUS Sagaris GK1100", DetectAsusSagarisKeyboard, ASUS_USB_VID, ASUS_SAGARIS_GK1100_PID, 1, 0xFF02, 2);
