/*---------------------------------------------------------*\
| KasaSmartControllerDetect.cpp                             |
|                                                           |
|   Detector for Kasa Smart bulbs                           |
|                                                           |
|   Devin Wendt (umbreon222@gmail.com)          16 Feb 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "KasaSmartController.h"
#include "ResourceManager.h"
#include "RGBController_KasaSmart.h"
#include "SettingsManager.h"

DetectedControllers DetectKasaSmartControllers()
{
    DetectedControllers detected_controllers;
    json                kasa_smart_settings;

    /*-----------------------------------------------------*\
    | Get Kasa Smart settings from settings manager         |
    \*-----------------------------------------------------*/
    kasa_smart_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("KasaSmartDevices");

    /*-----------------------------------------------------*\
    | If the Kasa Smart settings contains devices, process  |
    \*-----------------------------------------------------*/
    if(kasa_smart_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < kasa_smart_settings["devices"].size(); device_idx++)
        {
            if(kasa_smart_settings["devices"][device_idx].contains("ip"))
            {
                std::string kasa_smart_ip = kasa_smart_settings["devices"][device_idx]["ip"];
                std::string name          = kasa_smart_settings["devices"][device_idx]["name"];

                KasaSmartController* controller = new KasaSmartController(kasa_smart_ip, name);

                if(!controller->Initialize())
                {
                    continue;
                }

                RGBController_KasaSmart* rgb_controller = new RGBController_KasaSmart(controller);

                detected_controllers.push_back(rgb_controller);
            }
        }
    }

    return(detected_controllers);
}

REGISTER_DETECTOR("KasaSmart", DetectKasaSmartControllers);
