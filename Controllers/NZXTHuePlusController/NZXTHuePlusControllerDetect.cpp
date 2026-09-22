/*---------------------------------------------------------*\
| NZXTHuePlusControllerDetect.cpp                           |
|                                                           |
|   Detector for NZXT Hue Plus                              |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      27 Aug 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <vector>
#include "DetectionManager.h"
#include "NZXTHuePlusController.h"
#include "RGBController_NZXTHuePlus.h"

/*---------------------------------------------------------*\
| NZXT USB IDs                                              |
\*---------------------------------------------------------*/
#define NZXT_HUE_PLUS_VID                           0x04D8
#define NZXT_HUE_PLUS_PID                           0x00DF

DetectedControllers DetectNZXTHuePlusControllers(SerialDeviceInfo* port_info, const std::string& name)
{
    DetectedControllers         detected_controllers;

    HuePlusController*     controller     = new HuePlusController();
    controller->Initialize(port_info->port_path);

    RGBController_HuePlus* rgb_controller = new RGBController_HuePlus(controller);

    detected_controllers.push_back(rgb_controller);

    return(detected_controllers);
}

REGISTER_USB_SERIAL_DETECTOR("NZXT Hue+", DetectNZXTHuePlusControllers, NZXT_HUE_PLUS_VID, NZXT_HUE_PLUS_PID);
