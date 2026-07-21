/*---------------------------------------------------------*\
| AkkoKeyboardControllerDetect.cpp                          |
|                                                           |
|   Detector for Akko Multi-modes keyboards                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "AkkoKeyboardController.h"
#include "RGBController_AkkoKeyboard.h"

#define AKKO_KEYBOARD_VID          0x3151
#define AKKO_KEYBOARD_PID          0x4003
#define AKKO_INTERFACE             0

void DetectAkkoKeyboardControllers(hid_device_info* info, const std::string&)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AkkoKeyboardController* controller = new AkkoKeyboardController(dev, info->path);

        if(controller->IsValid())
        {
            RGBController_AkkoKeyboard* rgb_controller = new RGBController_AkkoKeyboard(controller);
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
        else
        {
            delete controller;
        }
    }
}

REGISTER_HID_DETECTOR_I(
    "Akko Multi-modes Keyboard-B",
    DetectAkkoKeyboardControllers,
    AKKO_KEYBOARD_VID,
    AKKO_KEYBOARD_PID,
    AKKO_INTERFACE
);
