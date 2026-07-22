/*---------------------------------------------------------*\
| HyperXEve1800ControllerDetect.cpp                         |
|                                                           |
|   Driver for HyperX Eve 1800 keyboards                    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "HyperXEve1800Controller.h"
#include "RGBController_HyperXEve1800.h"

/*---------------------------------------------------------*\
| HyperX keyboard vendor and product IDs (HP)               |
\*---------------------------------------------------------*/
#define HP_KEYBOARD_VID                             0x03F0
#define HYPERX_EVE_1800_HP_PID                      0x08C2

DetectedControllers DetectHyperXEve1800(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        HyperXEve1800Controller*     controller     = new HyperXEve1800Controller(dev, info->path, name);
        RGBController_HyperXEve1800* rgb_controller = new RGBController_HyperXEve1800(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return detected_controllers;
}

REGISTER_HID_DETECTOR_I("HyperX Eve 1800 (HP)", DetectHyperXEve1800, HP_KEYBOARD_VID, HYPERX_EVE_1800_HP_PID, 2);
