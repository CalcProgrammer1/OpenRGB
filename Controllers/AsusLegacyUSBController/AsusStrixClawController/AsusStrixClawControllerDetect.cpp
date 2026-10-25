/*---------------------------------------------------------*\
| AsusStrixClawControllerDetect.cpp                         |
|                                                           |
|   Detector for ASUS Strix Claw mouse devices              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "AsusStrixClawController.h"
#include "RGBController_AsusStrixClaw.h"

/*---------------------------------------------------------*\
| ASUS VID                                                  |
\*---------------------------------------------------------*/
#define ASUS_LEGACY_USB_VID                         0x195D

/*---------------------------------------------------------*\
| ASUS ROG Strix Claw PID                                   |
\*---------------------------------------------------------*/
#define ASUS_ROG_STRIX_CLAW_PID                     0x1016

DetectedControllers DetectAsusStrixClaw(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        StrixClawController*     controller          = new StrixClawController(dev, info->path, name);
        RGBController_StrixClaw* rgb_controller      = new RGBController_StrixClaw(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

/*---------------------------------------------------------*\
| ASUS ROG Strix Claw Detector                              |
\*---------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("ASUS ROG Strix Claw", DetectAsusStrixClaw, ASUS_LEGACY_USB_VID, ASUS_ROG_STRIX_CLAW_PID, 0, 0xFF01, 1);
