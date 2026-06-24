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

#include "Detector.h"
#include "QMKKeychronController.h"
#include "RGBController_QMKKeychron.h"

void DetectQMKKeychronController(hid_device_info *info, const std::string&)
{
    hid_device *dev = hid_open_path(info->path);

    if(dev)
    {
        QMKKeychronController* controller = new QMKKeychronController(dev, info->path);

        if(controller->GetSupported())
        {
            RGBController_QMKKeychron* rgb_controller = new RGBController_QMKKeychron(controller);
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
        else
        {
            delete controller;
        }
    }
}

REGISTER_HID_DETECTOR_IPU("Keychron Q1 HE", DetectQMKKeychronController, KEYCHRON_VID, KEYCHRON_Q1_HE_PID, 1, KEYCHRON_QMK_USAGE_PAGE, KEYCHRON_QMK_USAGE);
REGISTER_HID_DETECTOR_IPU("Keychron Q2",    DetectQMKKeychronController, KEYCHRON_VID, KEYCHRON_Q2_PID,    1, KEYCHRON_QMK_USAGE_PAGE, KEYCHRON_QMK_USAGE);
