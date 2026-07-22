/*---------------------------------------------------------*\
| BloodyB820RControllerDetect.cpp                           |
|                                                           |
|   Detector for Bloody B820R Devices                       |
|                                                           |
|   Mohammed Julfikar Ali Mahbub (o-julfikar)   01 Apr 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "BloodyB820RController.h"
#include "DetectionManager.h"
#include "RGBController_BloodyB820R.h"

/*---------------------------------------------------------*\
| A4 Tech USB vendor ID                                     |
\*---------------------------------------------------------*/
#define A4_TECH_VID                                 0x09DA

DetectedControllers DetectBloodyB820R(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        BloodyB820RController*     controller     = new BloodyB820RController(dev, info->path, name);
        RGBController_BloodyB820R* rgb_controller = new RGBController_BloodyB820R(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IPU("Bloody B820R", DetectBloodyB820R, A4_TECH_VID, BLOODY_B820R_PID, 2, 0xFF52, 0x0210);
