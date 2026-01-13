/*---------------------------------------------------------*\
| MNTKeyboardControllerDetect.cpp                           |
|                                                           |
|   Driver for the MNT Reform keyboards                     |
|                                                           |
|   Christian Heller <c.heller@plomlompom.de>   07 Aug 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include <hidapi.h>
#include "MNTReformKeyboardController.h"
#include "MNTPocketReformKeyboardController.h"
#include "RGBController_MNTReformKeyboard.h"
#include "RGBController_MNTPocketReformKeyboard.h"

#define PID_KBD_REFORM          0x6D02
#define PID_KBD_POCKET_REFORM   0x6D06

DetectedControllers DetectMNTKeyboardControllers(hid_device_info *info, const std::string &name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        if(info->product_id == PID_KBD_REFORM)
        {
            MNTReformKeyboardController *     controller     = new MNTReformKeyboardController(dev, info->path);
            RGBController_MNTReformKeyboard * rgb_controller = new RGBController_MNTReformKeyboard(controller);

            detected_controllers.push_back(rgb_controller);
        }
        else if(info->product_id == PID_KBD_POCKET_REFORM)
        {
            MNTPocketReformKeyboardController *     controller     = new MNTPocketReformKeyboardController(dev, info->path);
            RGBController_MNTPocketReformKeyboard * rgb_controller = new RGBController_MNTPocketReformKeyboard(controller);

            detected_controllers.push_back(rgb_controller);
        }
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IPU("MNT Reform Keyboard", DetectMNTKeyboardControllers, KBD_VID, PID_KBD_REFORM, KBD_INTERFACE, HID_USAGE_PAGE_DESKTOP, HID_USAGE_DESKTOP_KEYBOARD);
REGISTER_HID_DETECTOR_IPU("MNT Pocket Reform Keyboard", DetectMNTKeyboardControllers, KBD_VID, PID_KBD_POCKET_REFORM, KBD_INTERFACE, HID_USAGE_PAGE_DESKTOP, HID_USAGE_DESKTOP_KEYBOARD);
