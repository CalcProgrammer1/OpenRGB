/*---------------------------------------------------------*\
| EKControllerDetect.cpp                                    |
|                                                           |
|   Detector for EK Loop Connect                            |
|                                                           |
|   Chris M (Dr_No)                             16 Jul 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "EKController.h"
#include "RGBController_EKController.h"

#define EK_VID                0x0483
#define EK_LOOP_CONNECT       0x5750

DetectedControllers DetectEKControllers(hid_device_info* info, const std::string&)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        EKController*               controller     = new EKController(dev, info->path);
        RGBController_EKController* rgb_controller = new RGBController_EKController(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IPU("EK Loop Connect", DetectEKControllers, EK_VID, EK_LOOP_CONNECT, 0, 0xFFA0, 1);
