/*---------------------------------------------------------*\
| QMKKeychronControllerDetect.cpp                           |
|                                                           |
|   Detector for Keychron QMK-based keyboards               |
|                                                           |
|   Amadej Kastelic                             21 Jun 2026 |
|   Adam Honse <calcprogrammer1@gmail.com>      22 Jun 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <chrono>
#include <thread>

#include "DetectionManager.h"
#include "QMKKeychronController.h"
#include "RGBController_QMKKeychron.h"

DetectedControllers DetectQMKKeychronControllers(hid_device_info* info, const std::string& /*name*/)
{
    DetectedControllers detected_controllers;

    for(unsigned int attempt = 0; attempt < 2; attempt++)
    {
        if(attempt != 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        hid_device* dev = hid_open_path(info->path);

        if(!dev)
        {
            continue;
        }

        QMKKeychronController* controller = new QMKKeychronController(dev, info->path, info->product_id);

        if(!controller->GetSupported())
        {
            delete controller;
            continue;
        }

        RGBController_QMKKeychron* rgb_controller = new RGBController_QMKKeychron(controller);

        detected_controllers.push_back(rgb_controller);
        break;
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IPU("Keychron RGB", DetectQMKKeychronControllers, KEYCHRON_VID, HID_PID_ANY, 1, KEYCHRON_QMK_USAGE_PAGE, KEYCHRON_QMK_USAGE);
