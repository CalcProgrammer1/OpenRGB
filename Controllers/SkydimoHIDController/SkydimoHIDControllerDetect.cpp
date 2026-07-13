/*---------------------------------------------------------*\
| SkydimoHIDControllerDetect.cpp                            |
|                                                           |
|   Detector for Skydimo HID devices                        |
|                                                           |
|   Bartholomew Ho <qiangqiang101@hotmail.com>  09 Jul 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "LogManager.h"
#include "RGBController_SkydimoHID.h"
#include "SkydimoHIDController.h"

/*---------------------------------------------------------*\
| Skydimo HID vendor and product IDs                        |
\*---------------------------------------------------------*/
#define SKYDIMO_VID                 0x1A86
#define SKYDIMO_SK0902_PID          0xE316
#define SKYDIMO_SK0902_INTERFACE    0
#define SKYDIMO_SK0902_USAGE_PAGE   0xFF00
#define SKYDIMO_SK0902_USAGE        0x01

DetectedControllers DetectSkydimoHIDControllers(hid_device_info* info, const std::string&)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        SkydimoHIDController*       controller     = new SkydimoHIDController(dev, info->path);
        RGBController_SkydimoHID*   rgb_controller = new RGBController_SkydimoHID(controller);

        detected_controllers.push_back(rgb_controller);
        LOG_INFO("[SkydimoHIDControllerDetect] Detected Skydimo SK0902 at %s", info->path);
    }
    else
    {
        LOG_WARNING("[SkydimoHIDControllerDetect] Failed to open Skydimo SK0902 at %s", info->path);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IPU("Skydimo SK0902", DetectSkydimoHIDControllers, SKYDIMO_VID, SKYDIMO_SK0902_PID, SKYDIMO_SK0902_INTERFACE, SKYDIMO_SK0902_USAGE_PAGE, SKYDIMO_SK0902_USAGE);
