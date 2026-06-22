/*---------------------------------------------------------*\
| ArcticControllerDetect.cpp                                |
|                                                           |
|   Detector for Arctic devices                             |
|                                                           |
|   Armin Wolf (Wer-Wolf)                       09 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <vector>
#include "Detector.h"
#include "ArcticController.h"
#include "RGBController_Arctic.h"
#include "SettingsManager.h"

#define CH341_VID   0x1A86
#define CH341_PID   0x7523

void DetectArcticControllers()
{
    json settings = ResourceManager::get()->GetSettingsManager()->GetSettings("ArcticDevices");

    if(settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < settings["devices"].size(); device_idx++)
        {
            if(settings["devices"][device_idx].contains("port"))
            {
                ArcticController *controller = new ArcticController(settings["devices"][device_idx]["port"]);

                if(controller->IsPresent())
                {
                    RGBController_Arctic *rgb_controller = new RGBController_Arctic(controller);
                    ResourceManager::get()->RegisterRGBController(rgb_controller);
                }
                else
                {
                    delete controller;
                }
            }
        }
    }
}

REGISTER_DETECTOR("Arctic RGB controller", DetectArcticControllers);
/*---------------------------------------------------------------------------------------------------------*\
| Entries for dynamic UDEV rules                                                                            |
|                                                                                                           |
| DUMMY_DEVICE_DETECTOR("Arctic RGB controller", DetectArcticControllers, 0x1A86, 0x7523 )                  |
\*---------------------------------------------------------------------------------------------------------*/
