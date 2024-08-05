/*---------------------------------------------------------*\
| LightSaltControllerDetect.cpp                             |
|                                                           |
|   Detector for LightSalt Peripherals                      |
|                                                           |
|   James Buren (braewoods)                     23 Jul 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "LightSaltController.h"
#include "RGBController_LightSaltKeyboard.h"
#include "RGBController_LightSaltKeypad.h"

#define LIGHTSALT_VID 0x0483
#define LIGHTSALT_PID 0x5750

void DetectLightSaltControllers(hid_device_info* info, const std::string &)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        LightSaltController* controller     = new LightSaltController(dev, *info);
        RGBController*       rgb_controller = nullptr;

        switch(controller->GetDeviceType())
        {
            case LIGHTSALT_TYPE_KEYBOARD:
                rgb_controller = new RGBController_LightSaltKeyboard(controller);
                break;

            case LIGHTSALT_TYPE_KEYPAD:
                rgb_controller = new RGBController_LightSaltKeypad(controller);
                break;

            default:
                delete controller;
                break;
        }

        if(rgb_controller != nullptr)
        {
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
    }
}   /* DetectLightSaltControllers() */

REGISTER_HID_DETECTOR_IPU("LightSalt Peripherals", DetectLightSaltControllers, LIGHTSALT_VID, LIGHTSALT_PID, 1, 1, 0);
