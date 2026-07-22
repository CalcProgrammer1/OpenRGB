/*---------------------------------------------------------*\
| HyperXAlloyOriginsControllerDetect.cpp                    |
|                                                           |
|   Driver for HyperX Alloy Origins keyboards               |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "HyperXAlloyOriginsController.h"
#include "RGBController_HyperXAlloyOrigins.h"

/*---------------------------------------------------------*\
| HyperX keyboard vendor and product IDs                    |
\*---------------------------------------------------------*/
#define HYPERX_KEYBOARD_VID                         0x0951
#define HYPERX_ALLOY_ORIGINS_PID                    0x16E5

/*---------------------------------------------------------*\
| HyperX keyboard vendor and product IDs (HP)               |
\*---------------------------------------------------------*/
#define HP_KEYBOARD_VID                             0x03F0
#define HYPERX_ALLOY_ORIGINS_HP_PID                 0x0591

DetectedControllers DetectHyperXAlloyOrigins(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        HyperXAlloyOriginsController*     controller     = new HyperXAlloyOriginsController(dev, info->path, name);
        RGBController_HyperXAlloyOrigins* rgb_controller = new RGBController_HyperXAlloyOrigins(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

#ifdef _WIN32
REGISTER_HID_DETECTOR_I("HyperX Alloy Origins",           DetectHyperXAlloyOrigins,        HYPERX_KEYBOARD_VID, HYPERX_ALLOY_ORIGINS_PID,         3);
REGISTER_HID_DETECTOR_IP("HyperX Alloy Origins (HP)",     DetectHyperXAlloyOrigins,        HP_KEYBOARD_VID,     HYPERX_ALLOY_ORIGINS_HP_PID,      3, 0xFF90);
#else
REGISTER_HID_DETECTOR_I("HyperX Alloy Origins",           DetectHyperXAlloyOrigins,        HYPERX_KEYBOARD_VID, HYPERX_ALLOY_ORIGINS_PID,         0);
REGISTER_HID_DETECTOR_I("HyperX Alloy Origins (HP)",      DetectHyperXAlloyOrigins,        HP_KEYBOARD_VID,     HYPERX_ALLOY_ORIGINS_HP_PID,      0);
#endif
