/*---------------------------------------------------------*\
| HyperXOrigins2_65ControllerDetect.cpp                     |
|                                                           |
|   Driver for HyperX Origins 2 65 keyboards                |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "HyperXOrigins2_65Controller.h"
#include "RGBController_HyperXOrigins2_65.h"

/*---------------------------------------------------------*\
| HyperX keyboard vendor and product IDs (HP)               |
\*---------------------------------------------------------*/
#define HP_KEYBOARD_VID                             0x03F0
#define HYPERX_ORIGINS_2_65_HP_PID                  0x0CC2

DetectedControllers DetectHyperXOrigins2_65(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        HyperXOrigins2_65Controller* controller = new HyperXOrigins2_65Controller(dev, info->path, name);
        RGBController_HyperXOrigins2_65* rgb_controller = new RGBController_HyperXOrigins2_65(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_I("HyperX Origins 2 65 (HP)", DetectHyperXOrigins2_65, HP_KEYBOARD_VID, HYPERX_ORIGINS_2_65_HP_PID, 3);
