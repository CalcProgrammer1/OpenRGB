/*---------------------------------------------------------*\
| WitmodKeyboardControllerDetect.cpp                         |
|                                                           |
|   Detector for Witmod keyboards                           |
|                                                           |
|   vlack                                       03 May 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "WitmodKeyboardController.h"
#include "RGBController_WitmodKeyboard.h"

/*---------------------------------------------------------*\
| Witmod uses the Nuvoton/Winbond USB vendor ID.  The       |
| GK8110 and GK8120 boards share this VID and PID and are    |
| told apart by the model in the device info reply.         |
\*---------------------------------------------------------*/
#define WITMOD_VID                                 0x0416
#define WITMOD_KEYBOARD_PID                         0xC345

DetectedControllers DetectWitmodKeyboard(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        WitmodKeyboardController*     controller     = new WitmodKeyboardController(dev, *info, name);
        RGBController_WitmodKeyboard* rgb_controller = new RGBController_WitmodKeyboard(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_I("Witmod Keyboard", DetectWitmodKeyboard, WITMOD_VID, WITMOD_KEYBOARD_PID, 2);
