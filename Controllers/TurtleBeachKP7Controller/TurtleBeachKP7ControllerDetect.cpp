/*---------------------------------------------------------*\
| TurtleBeachKP7ControllerDetect.cpp                        |
|                                                           |
|   Detector for Turtle Beach Command Series KP7 keypad     |
|                                                           |
|   Joe Peterson (joevpeterson@gmail.com)       05 Aug 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "RGBController_TurtleBeachKP7.h"
#include "TurtleBeachKP7Controller.h"

DetectedControllers DetectTurtleBeachKP7Controllers(hid_device_info* info, const std::string& /*name*/)
{
    DetectedControllers detected_controllers;
    hid_device*         dev = hid_open_path(info->path);

    if(dev)
    {
        TurtleBeachKP7Controller*      controller      = new TurtleBeachKP7Controller(dev, info->path);
        RGBController_TurtleBeachKP7*  rgb_controller  = new RGBController_TurtleBeachKP7(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_I("Turtle Beach Command Series KP7", DetectTurtleBeachKP7Controllers, TURTLE_BEACH_VID, TURTLE_BEACH_KP7_PID, TURTLE_BEACH_KP7_INTERFACE);
