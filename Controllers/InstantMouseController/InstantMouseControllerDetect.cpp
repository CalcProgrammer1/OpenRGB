/*---------------------------------------------------------*\
| InstantMouseControllerDetect.cpp                          |
|                                                           |
|   Detector for Instant mouse                              |
|                                                           |
|   Morgan Guimard (morg)                       19 Jan 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "InstantMouseDevices.h"
#include "InstantMouseController.h"
#include "RGBController_InstantMouse.h"

DetectedControllers DetectInstantMouseControllers(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        InstantMouseController*     controller         = new InstantMouseController(dev, *info, name);
        RGBController_InstantMouse* rgb_controller     = new RGBController_InstantMouse(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IPU("Advanced GTA 250 USB Gaming Mouse",  DetectInstantMouseControllers,  INSTANT_MICROELECTRONICS_VID, ADVANCED_GTA_250_PID, 1, 0xFF01, 0x01);
REGISTER_HID_DETECTOR_IPU("Anko KM43243952 USB Gaming Mouse",   DetectInstantMouseControllers,  INSTANT_MICROELECTRONICS_VID, ANKO_KM43243952_VID,  1, 0xFF01, 0x01);
REGISTER_HID_DETECTOR_IPU("Anko KM43277483 USB Gaming Mouse",   DetectInstantMouseControllers,  INSTANT_MICROELECTRONICS_VID, ANKO_KM43277483_VID,  1, 0xFF01, 0x01);
REGISTER_HID_DETECTOR_IPU("AntEsports GM600 USB Gaming Mouse",  DetectInstantMouseControllers,  INSTANT_MICROELECTRONICS_VID, ANTESPORTS_GM600_PID, 1, 0xFF01, 0001);

