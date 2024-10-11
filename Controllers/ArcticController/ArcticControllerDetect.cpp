/*---------------------------------------------------------*\
| ArcticControllerDetect.cpp                                |
|                                                           |
|   Detector for Arctic devices                             |
|                                                           |
|   Armin Wolf (Wer-Wolf)                       09 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <vector>
#include "Detector.h"
#include "ArcticController.h"
#include "RGBController_Arctic.h"
#include "find_usb_serial_port.h"

#define CH341_VID   0x1A86
#define CH341_PID   0x7523

void DetectArcticControllers()
{
    std::vector<std::string *> ports = find_usb_serial_port(CH341_VID, CH341_PID);

    for(unsigned int device = 0; device < ports.size(); device++)
    {
        ArcticController *controller = new ArcticController(*ports[device]);

        if(controller->IsPresent())
        {
            RGBController_Arctic *rgb_controller = new RGBController_Arctic(controller);
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
        else
        {
            delete controller;
        }

        delete ports[device];
    }
}

REGISTER_DETECTOR("Arctic RGB controller", DetectArcticControllers);
/*---------------------------------------------------------------------------------------------------------*\
| Entries for dynamic UDEV rules                                                                            |
|                                                                                                           |
| DUMMY_DEVICE_DETECTOR("Arctic RGB controller", DetectArcticControllers, 0x1A86, 0x7523 )                  |
\*---------------------------------------------------------------------------------------------------------*/
