/*---------------------------------------------------------*\
| AkkoKeyboardControllerDetect.cpp                          |
|                                                           |
|   Detector for Akko Multi-modes keyboards                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "AkkoKeyboardController.h"
#include "RGBController_AkkoKeyboard.h"

#define AKKO_KEYBOARD_VID          0x3151
#define AKKO_KEYBOARD_PID          0x4003
#define AKKO_INTERFACE             0

DetectedControllers DetectAkkoKeyboardControllers(
    hid_device_info* info,
    const std::string&
)
{
    DetectedControllers detected_controllers;

    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AkkoKeyboardController* controller =
            new AkkoKeyboardController(dev, info->path);

        if(controller->IsValid())
        {
            detected_controllers.push_back(
                new RGBController_AkkoKeyboard(controller)
            );
        }
        else
        {
            delete controller;
        }
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_I(
    "Akko Multi-modes Keyboard-B",
    DetectAkkoKeyboardControllers,
    AKKO_KEYBOARD_VID,
    AKKO_KEYBOARD_PID,
    AKKO_INTERFACE
);
