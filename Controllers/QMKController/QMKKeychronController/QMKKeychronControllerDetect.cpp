/*---------------------------------------------------------*\
| QMKKeychronControllerDetect.cpp                           |
|                                                           |
|   Detector for Keychron QMK-based keyboards               |
|   (Q1 HE and other KEYCHRON_RGB-enabled models)           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "QMKKeychronController.h"
#include "RGBController_QMKKeychron.h"

void DetectQMKKeychronController(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        QMKKeychronController*     controller      = new QMKKeychronController(dev, info->path);
        RGBController_QMKKeychron* rgb_controller  = new RGBController_QMKKeychron(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("Keychron Q1 HE", DetectQMKKeychronController, KEYCHRON_VID, KEYCHRON_Q1_HE_PID, 1, KEYCHRON_QMK_USAGE_PAGE, KEYCHRON_QMK_USAGE);
REGISTER_HID_DETECTOR_IPU("Keychron Q2",    DetectQMKKeychronController, KEYCHRON_VID, KEYCHRON_Q2_PID,    1, KEYCHRON_QMK_USAGE_PAGE, KEYCHRON_QMK_USAGE);
