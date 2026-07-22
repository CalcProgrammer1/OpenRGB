/*---------------------------------------------------------*\
| HyperXAlloyElite2ControllerDetect.cpp                     |
|                                                           |
|   Driver for HyperX Alloy Elite 2 keyboards               |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "HyperXAlloyElite2Controller.h"
#include "RGBController_HyperXAlloyElite2.h"

/*---------------------------------------------------------*\
| HyperX keyboard vendor and product IDs                    |
\*---------------------------------------------------------*/
#define HYPERX_KEYBOARD_VID                         0x0951
#define HYPERX_ALLOY_ELITE_2_PID                    0x1711

/*---------------------------------------------------------*\
| HyperX keyboard vendor and product IDs (HP)               |
\*---------------------------------------------------------*/
#define HP_KEYBOARD_VID                             0x03F0
#define HYPERX_ALLOY_ELITE_2_HP_PID                 0x058F

DetectedControllers DetectHyperXAlloyElite2(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        HyperXAlloyElite2Controller*     controller     = new HyperXAlloyElite2Controller(dev, info->path, name);
        RGBController_HyperXAlloyElite2* rgb_controller = new RGBController_HyperXAlloyElite2(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

#ifdef _WIN32
REGISTER_HID_DETECTOR_IP("HyperX Alloy Elite 2",          DetectHyperXAlloyElite2,         HYPERX_KEYBOARD_VID, HYPERX_ALLOY_ELITE_2_PID,         3, 0xFF90);
REGISTER_HID_DETECTOR_IP("HyperX Alloy Elite 2 (HP)",     DetectHyperXAlloyElite2,         HP_KEYBOARD_VID,     HYPERX_ALLOY_ELITE_2_HP_PID,      3, 0xFF90);
#else
REGISTER_HID_DETECTOR_I("HyperX Alloy Elite 2",           DetectHyperXAlloyElite2,         HYPERX_KEYBOARD_VID, HYPERX_ALLOY_ELITE_2_PID,         0);
REGISTER_HID_DETECTOR_I("HyperX Alloy Elite 2 (HP)",      DetectHyperXAlloyElite2,         HP_KEYBOARD_VID,     HYPERX_ALLOY_ELITE_2_HP_PID,      0);
#endif
