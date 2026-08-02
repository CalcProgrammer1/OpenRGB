/*---------------------------------------------------------*\
| ClevoKeyboardControllerDetect.cpp                         |
|                                                           |
|   Detector for Clevo per-key RGB keyboard                 |
|   Supports ITE 8291 (PID 0x600B) and ITE 829x (0x8910)    |
|                                                           |
|   Kyle Cascade (kyle@cascade.family)          16 Jan 2026 |
|   Valentin Lobstein (balgogan@protonmail.com) 27 Mar 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "ClevoKeyboardController.h"
#include "DetectionManager.h"
#include "RGBController_ClevoKeyboard.h"
#include "RGBController.h"
#include <hidapi.h>

/*---------------------------------------------------------*\
| ITE Tech vendor ID                                        |
\*---------------------------------------------------------*/
#define ITE_VID                                 0x048D

/*---------------------------------------------------------*\
| Clevo Keyboard product IDs                                |
|   ITE 8291 and ITE 829x per-key RGB keyboard controllers  |
\*---------------------------------------------------------*/
#define CLEVO_KEYBOARD_PID_8291                 0x600B
#define CLEVO_KEYBOARD_PID_829X                 0x8910

static DetectedControllers DetectClevoKeyboard(hid_device_info* info, clevo_keyboard_type kb_type)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        ClevoKeyboardController*     controller     = new ClevoKeyboardController(dev, *info, kb_type);
        RGBController_ClevoKeyboard* rgb_controller = new RGBController_ClevoKeyboard(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

DetectedControllers DetectClevoKeyboard8291(hid_device_info* info, const std::string& /*name*/)
{
    return(DetectClevoKeyboard(info, CLEVO_KB_ITE8291));
}

DetectedControllers DetectClevoKeyboard829x(hid_device_info* info, const std::string& /*name*/)
{
    return(DetectClevoKeyboard(info, CLEVO_KB_ITE829X));
}

REGISTER_HID_DETECTOR_PU("Clevo Keyboard (ITE 8291)", DetectClevoKeyboard8291, ITE_VID, CLEVO_KEYBOARD_PID_8291, 0xFF03, 0x01);
REGISTER_HID_DETECTOR_PU("Clevo Keyboard (ITE 829x)", DetectClevoKeyboard829x, ITE_VID, CLEVO_KEYBOARD_PID_829X, 0xFF89, 0xCC);
