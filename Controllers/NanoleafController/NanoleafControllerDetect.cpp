/*---------------------------------------------------------*\
| NanoleafControllerDetect.cpp                              |
|                                                           |
|   Detector for Nanoleaf                                   |
|                                                           |
|   Nikita Rushmanov                            13 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "RGBController_Nanoleaf.h"
#include "SettingsManager.h"

DetectedControllers DetectNanoleafControllers()
{
    DetectedControllers detected_controllers;
    json                nanoleaf_settings;

    nanoleaf_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("NanoleafDevices");

    if(nanoleaf_settings.contains("devices"))
    {
        for(json::const_iterator it = nanoleaf_settings["devices"].begin(); it != nanoleaf_settings["devices"].end(); ++it)
        {
            const json& device = it.value();

            if(device.contains("ip") && device.contains("port") && device.contains("auth_token"))
            {
                try
                {
                    RGBController_Nanoleaf* rgb_controller = new RGBController_Nanoleaf(device["ip"], device["port"], device["auth_token"]);

                    detected_controllers.push_back(rgb_controller);
                }
                catch(...)
                {
                    LOG_DEBUG("[Nanoleaf] Could not connect to device at %s:%d using auth_token %s", device["ip"].get<std::string>().c_str(), device["port"].get<int>(), device["auth_token"].get<std::string>().c_str());
                }
            }
        }
    }

    return(detected_controllers);
}

REGISTER_DETECTOR("Nanoleaf", DetectNanoleafControllers);
