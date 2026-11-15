/*---------------------------------------------------------*\
| NZXTHue2ControllerDetect.cpp                              |
|                                                           |
|   Detector for NZXT Hue 2                                 |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      29 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "LogManager.h"
#include "NZXTHue2Controller.h"
#include "RGBController_NZXTHue2.h"

DetectedControllers DetectNZXTHue2Controllers(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        NZXTHue2Controller*     controller     = new NZXTHue2Controller(dev, info->path, info->product_id, name);

        if(!controller->IsDeviceSupported())
        {
            LOG_WARNING("[NZXTHue2Controller] %s not registered, capabilities unknown", name.c_str());
            delete controller;
            return(detected_controllers);
        }

        RGBController_NZXTHue2* rgb_controller = new RGBController_NZXTHue2(controller);

        detected_controllers.push_back(rgb_controller);

        LOG_TRACE("[NZXTHue2Controller] NZXT Controller setup: %s", info->path);
    }
    else
    {
        LOG_DEBUG("[NZXTHue2Controller] Failed to load device: %s!", info->path);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR("NZXT RGB & Fan Controller",          DetectNZXTHue2Controllers, NZXT_VID, NZXT_RGB_FAN_CONTROLLER_PID);
REGISTER_HID_DETECTOR("NZXT RGB & Fan Controller",          DetectNZXTHue2Controllers, NZXT_VID, NZXT_RGB_FAN_CONTROLLER2_PID);
REGISTER_HID_DETECTOR("NZXT RGB & Fan Controller",          DetectNZXTHue2Controllers, NZXT_VID, NZXT_RGB_FAN_CONTROLLER3_PID);
REGISTER_HID_DETECTOR("NZXT RGB & Fan Controller",          DetectNZXTHue2Controllers, NZXT_VID, NZXT_RGB_FAN_CONTROLLER4_PID);
REGISTER_HID_DETECTOR("NZXT RGB & Fan Controller",          DetectNZXTHue2Controllers, NZXT_VID, NZXT_RGB_FAN_CONTROLLER5_PID);
REGISTER_HID_DETECTOR("NZXT RGB & Fan Controller",          DetectNZXTHue2Controllers, NZXT_VID, NZXT_RGB_FAN_CONTROLLER6_PID);
REGISTER_HID_DETECTOR("NZXT RGB & Fan Controller",          DetectNZXTHue2Controllers, NZXT_VID, NZXT_RGB_FAN_CONTROLLER7_PID);
REGISTER_HID_DETECTOR("NZXT RGB & Fan Controller 2024",     DetectNZXTHue2Controllers, NZXT_VID, NZXT_RGB_FAN_CONTROLLER8_PID);
REGISTER_HID_DETECTOR("NZXT B650E Motherboard",             DetectNZXTHue2Controllers, NZXT_VID, NZXT_RGB_FAN_CONTROLLER9_PID);
REGISTER_HID_DETECTOR("NZXT Hue 2",                         DetectNZXTHue2Controllers, NZXT_VID, NZXT_HUE_2_PID);
REGISTER_HID_DETECTOR("NZXT Hue 2 Ambient",                 DetectNZXTHue2Controllers, NZXT_VID, NZXT_HUE_2_AMBIENT_PID);
REGISTER_HID_DETECTOR("NZXT Hue 2 Motherboard",             DetectNZXTHue2Controllers, NZXT_VID, NZXT_MOTHERBOARD_DEVICE_PID);
REGISTER_HID_DETECTOR("NZXT Hue 2 Motherboard",             DetectNZXTHue2Controllers, NZXT_VID, NZXT_MOTHERBOARD_DEVICE_2_PID);
REGISTER_HID_DETECTOR("NZXT Kraken X3 Series",              DetectNZXTHue2Controllers, NZXT_VID, NZXT_KRAKEN_X3_SERIES_PID);
REGISTER_HID_DETECTOR("NZXT Kraken X3 Series RGB",          DetectNZXTHue2Controllers, NZXT_VID, NZXT_KRAKEN_X3_SERIES_RGB_PID);
REGISTER_HID_DETECTOR("NZXT Kraken 2024 ELITE Series RGB",  DetectNZXTHue2Controllers, NZXT_VID, NZXT_KRAKEN_2024_ELITE_SERIES_RGB_PID);
REGISTER_HID_DETECTOR("NZXT Kraken 2024 Plus",              DetectNZXTHue2Controllers, NZXT_VID, NZXT_KRAKEN_2024_PLUS_PID);
REGISTER_HID_DETECTOR("NZXT RGB Controller",                DetectNZXTHue2Controllers, NZXT_VID, NZXT_RGB_CONTROLLER_1_PID);
REGISTER_HID_DETECTOR("NZXT RGB Controller",                DetectNZXTHue2Controllers, NZXT_VID, NZXT_RGB_CONTROLLER_2_PID);
REGISTER_HID_DETECTOR("NZXT Smart Device V2",               DetectNZXTHue2Controllers, NZXT_VID, NZXT_SMART_DEVICE_V2_PID);
REGISTER_HID_DETECTOR("NZXT Smart Device V2",               DetectNZXTHue2Controllers, NZXT_VID, NZXT_SMART_DEVICE_V2_1_PID);
REGISTER_HID_DETECTOR("NZXT Smart Device V2",               DetectNZXTHue2Controllers, NZXT_VID, NZXT_SMART_DEVICE_V2_2_PID);
