/*---------------------------------------------------------*\
| RealtekARGBControllerDetect.cpp                           |
|                                                           |
|   Detector for Realtek USB ARGB ICs                       |
|                                                           |
|   Jerry Fan (JerryFan0612)                    13 Mar 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "RGBController.h"
#include "RGBController_RealtekARGB.h"

DetectedControllers DetectRealtekARGBControllers(hid_device_info* info, const std::string& /*name*/)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        RealtekARGBController * controller = new RealtekARGBController(dev, info);

        if(controller->get_support_openrgb())
        {
            RGBController_RealtekARGB * rgb_controller = new RGBController_RealtekARGB(controller);

            if(rgb_controller->GetDeviceType() != DEVICE_TYPE_UNKNOWN)
            {
                detected_controllers.push_back(rgb_controller);
            }
            else
            {
                delete rgb_controller;
            }
        }
        else
        {
            delete controller;
        }
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_PU("RTL9209", DetectRealtekARGBControllers, REALTEK_ARGB_VID, REALTEK_ARGB_PID, REALTEK_ARGB_HID2SCSI_PG, REALTEK_ARGB_HID2SCSI_USAGE);
