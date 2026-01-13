/*---------------------------------------------------------*\
| LaviewTechnologyDetect.cpp                                |
|                                                           |
|   Detector for Laview Technology brand Mice (Glorious)    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include <string>
#include "DetectionManager.h"
#include "LaviewTechnologyController.h"
#include "RGBController.h"
#include "RGBController_LaviewTechnology.h"

#define LAVIEW_TECHNOLOGY_VID               0x22D4
#define GLORIOUS_MODEL_I_PID                0x1503 // Wired

DetectedControllers DetectLaviewTechnologyMouse(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        LaviewTechnologyController*     controller     = new LaviewTechnologyController(dev, info, name);
        RGBController_LaviewTechnology* rgb_controller = new RGBController_LaviewTechnology(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IPU("Glorious Model I", DetectLaviewTechnologyMouse, LAVIEW_TECHNOLOGY_VID, GLORIOUS_MODEL_I_PID, 1, 0xFF01, 0x02);
