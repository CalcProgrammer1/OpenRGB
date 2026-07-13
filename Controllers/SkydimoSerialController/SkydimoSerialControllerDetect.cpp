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
#include "find_usb_serial_port.h"

/*---------------------------------------------------------*\
| Skydimo serial vendor and product IDs                     |
\*---------------------------------------------------------*/
#define SKYDIMO_SERIAL_VID    0x1A86
#define SKYDIMO_SERIAL_PID    0x7523

DetectedControllers DetectSkydimoSerialControllers()
{
    DetectedControllers         detected_controllers;
    std::vector<std::string>    ports = find_usb_serial_port(SKYDIMO_SERIAL_VID, SKYDIMO_SERIAL_PID);

    for(unsigned int port_idx = 0; port_idx < ports.size(); port_idx++)
    {
        SkydimoSerialController* controller = new SkydimoSerialController(ports[port_idx]);

        if(controller->IsPresent())
        {
            RGBController_SkydimoSerial* rgb_controller = new RGBController_SkydimoSerial(controller);
            detected_controllers.push_back(rgb_controller);
            LOG_INFO("[SkydimoSerialControllerDetect] Detected %s at %s",
                     controller->GetName().c_str(), ports[port_idx].c_str());
        }
        else
        {
            delete controller;
        }
    }

    return(detected_controllers);
}

REGISTER_DETECTOR("Skydimo Serial", DetectSkydimoSerialControllers);
/*---------------------------------------------------------------------------------------------------------*\
| Entries for dynamic UDEV rules                                                                            |
|                                                                                                           |
| DUMMY_DEVICE_DETECTOR("Skydimo Serial", DetectSkydimoSerialControllers, 0x1A86, 0x7523 )                  |
\*---------------------------------------------------------------------------------------------------------*/
