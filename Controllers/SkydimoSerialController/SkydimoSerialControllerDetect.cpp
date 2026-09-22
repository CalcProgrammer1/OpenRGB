/*---------------------------------------------------------*\
| SkydimoSerialControllerDetect.cpp                         |
|                                                           |
|   Detector for Skydimo serial devices                     |
|                                                           |
|   Bartholomew Ho <qiangqiang101@hotmail.com>  30 Jun 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <vector>
#include "DetectionManager.h"
#include "LogManager.h"
#include "RGBController_SkydimoSerial.h"
#include "SkydimoSerialController.h"

/*---------------------------------------------------------*\
| Skydimo serial vendor and product IDs                     |
\*---------------------------------------------------------*/
#define SKYDIMO_SERIAL_VID                          0x1A86
#define SKYDIMO_SERIAL_PID                          0x7523

DetectedControllers DetectSkydimoSerialControllers(SerialDeviceInfo* port_info, const std::string& name)
{
    DetectedControllers         detected_controllers;

    SkydimoSerialController* controller = new SkydimoSerialController(port_info->port_path);

    if(controller->IsPresent())
    {
        RGBController_SkydimoSerial* rgb_controller = new RGBController_SkydimoSerial(controller);
        detected_controllers.push_back(rgb_controller);
    }
    else
    {
        delete controller;
    }

    return(detected_controllers);
}

REGISTER_USB_SERIAL_DETECTOR("Skydimo Serial", DetectSkydimoSerialControllers, SKYDIMO_SERIAL_VID, SKYDIMO_SERIAL_PID);
