/*---------------------------------------------------------*\
| ClevoKeyboardControllerDetect.cpp                         |
|                                                           |
|   Detector for Clevo per-key RGB keyboard (ITE 8291)      |
|                                                           |
|   Kyle Cascade (kyle@cascade.family)          16 Jan 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "ClevoKeyboardController.h"
#include "RGBController_ClevoKeyboard.h"
#include "RGBController.h"
#include <hidapi.h>

/*-----------------------------------------------------*\
| ITE Tech vendor ID                                    |
\*-----------------------------------------------------*/
#define ITE_VID                         0x048D

/*-----------------------------------------------------*\
| Clevo Keyboard product IDs                            |
| These are ITE 8291 per-key RGB keyboard controllers   |
\*-----------------------------------------------------*/
#define CLEVO_KEYBOARD_PID_600B        0x600B

void DetectClevoKeyboardControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        ClevoKeyboardController*     controller     = new ClevoKeyboardController(dev, *info);
        RGBController_ClevoKeyboard* rgb_controller = new RGBController_ClevoKeyboard(controller);
        rgb_controller->name                         = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_PU("Clevo Keyboard", DetectClevoKeyboardControllers, 0x048D, 0x600B, 0xFF03, 0x01);
