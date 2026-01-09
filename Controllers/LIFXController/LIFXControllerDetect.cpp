/*---------------------------------------------------------*\
| LIFXControllerDetect.cpp                                  |
|                                                           |
|   Detector for LIFX                                       |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      05 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "LIFXController.h"
#include "ResourceManager.h"
#include "RGBController_LIFX.h"
#include "SettingsManager.h"

/******************************************************************************************\
*                                                                                          *
*   DetectLIFXControllers                                                                  *
*                                                                                          *
*       Detect LIFX devices                                                                *
*                                                                                          *
\******************************************************************************************/

void DetectLIFXControllers()
{
    json                    lifx_settings;

    /*-------------------------------------------------*\
    | Get LIFX settings from settings manager           |
    \*-------------------------------------------------*/
    lifx_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("LIFXDevices");

    /*-------------------------------------------------*\
    | If the Wiz settings contains devices, process     |
    \*-------------------------------------------------*/
    if(lifx_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < lifx_settings["devices"].size(); device_idx++)
        {
            if(lifx_settings["devices"][device_idx].contains("ip"))
            {
                std::string lifx_ip     = lifx_settings["devices"][device_idx]["ip"];
                std::string name        = lifx_settings["devices"][device_idx]["name"];
                bool multizone          = lifx_settings["devices"][device_idx]["multizone"];
                bool extended_multizone = lifx_settings["devices"][device_idx]["extended_multizone"];

                LIFXController* controller = new LIFXController(lifx_ip, name, multizone, extended_multizone);
                controller->FetchZoneCount();

                RGBController_LIFX* rgb_controller = new RGBController_LIFX(controller);

                DetectionManager::get()->RegisterRGBController(rgb_controller);
            }
        }
    }

}   /* DetectLIFXControllers() */

REGISTER_DETECTOR("LIFX", DetectLIFXControllers);
