/*---------------------------------------------------------*\
| DarkProjectControllerDetect.cpp                           |
|                                                           |
|   Detector for Dark Project devices                       |
|                                                           |
|   Chris M (DrNo)                              08 Apr 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "RGBController_DarkProjectKeyboard.h"

/*---------------------------------------------------------*\
| Dark Project vendor ID                                    |
\*---------------------------------------------------------*/
#define DARKPROJECT_VID                                0x195D

/*---------------------------------------------------------*\
| Product IDs                                               |
\*---------------------------------------------------------*/
#define KD3B_V2_PID                                    0x2061

void DetectDarkProjectKeyboardControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        DarkProjectKeyboardController*     controller       = new DarkProjectKeyboardController(dev, info->path, name);
        RGBController_DarkProjectKeyboard* rgb_controller   = new RGBController_DarkProjectKeyboard(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("Dark Project KD3B V2", DetectDarkProjectKeyboardControllers, DARKPROJECT_VID, KD3B_V2_PID, 2, 0xFFC2, 4);
