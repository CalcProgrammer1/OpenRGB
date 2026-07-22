/*---------------------------------------------------------*\
| HyperXAlloyOriginsCoreControllerDetect.cpp                |
|                                                           |
|   Driver for HyperX Alloy Origins Core keyboards          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "HyperXAlloyOriginsCoreController.h"
#include "RGBController_HyperXAlloyOriginsCore.h"

/*---------------------------------------------------------*\
| HyperX keyboard vendor and product IDs                    |
\*---------------------------------------------------------*/
#define HYPERX_KEYBOARD_VID                         0x0951
#define HYPERX_ALLOY_ORIGINS_CORE_PID               0x16E6

/*---------------------------------------------------------*\
| HyperX keyboard vendor and product IDs (HP)               |
\*---------------------------------------------------------*/
#define HP_KEYBOARD_VID                             0x03F0
#define HYPERX_ALLOY_ORIGINS_CORE_HP_PID            0x098F

DetectedControllers DetectHyperXAlloyOriginsCore(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        HyperXAlloyOriginsCoreController*     controller     = new HyperXAlloyOriginsCoreController(dev, info, name);
        RGBController_HyperXAlloyOriginsCore* rgb_controller = new RGBController_HyperXAlloyOriginsCore(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_I("HyperX Alloy Origins Core",      DetectHyperXAlloyOriginsCore,    HYPERX_KEYBOARD_VID, HYPERX_ALLOY_ORIGINS_CORE_PID,    2);
REGISTER_HID_DETECTOR_I("HyperX Alloy Origins Core (HP)", DetectHyperXAlloyOriginsCore,    HP_KEYBOARD_VID,     HYPERX_ALLOY_ORIGINS_CORE_HP_PID, 2);
