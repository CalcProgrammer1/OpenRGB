/*---------------------------------------------------------*\
| RobobloqLightStripControllerDetect.cpp                    |
|                                                           |
|   Detector for Robobloq RGB Light Strips                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "RGBController_RobobloqLightStrip.h"
#include "RobobloqLightStripController.h"

#define ROBOBLOQ_USB_VID 0x1A86

DetectedControllers DetectRobobloqLightStripController(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        RobobloqLightStripController*     controller     = new RobobloqLightStripController(dev, info->path, name);
        RGBController_RobobloqLightStrip* rgb_controller = new RGBController_RobobloqLightStrip(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_PU("Robobloq Monitor Light Strip", DetectRobobloqLightStripController, ROBOBLOQ_USB_VID, 0xFE07, 0xFF00, 0x01);
