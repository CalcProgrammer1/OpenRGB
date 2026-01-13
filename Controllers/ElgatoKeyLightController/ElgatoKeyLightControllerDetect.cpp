/*---------------------------------------------------------*\
| ElgatoKeyLightControllerDetect.cpp                        |
|                                                           |
|   Detector for Elgato Key Light                           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "ElgatoKeyLightController.h"
#include "ResourceManager.h"
#include "RGBController_ElgatoKeyLight.h"
#include "SettingsManager.h"

DetectedControllers DetectElgatoKeyLightControllers()
{
    DetectedControllers detected_controllers;
    json                elgato_keylight_settings;

    /*-----------------------------------------------------*\
    | Get KeyLight settings from settings manager           |
    \*-----------------------------------------------------*/
    elgato_keylight_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("ElgatoKeyLightDevices");

    /*-----------------------------------------------------*\
    | If the Elgato Key Light settings contains devices,    |
    | process                                               |
    \*-----------------------------------------------------*/
    if(elgato_keylight_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < elgato_keylight_settings["devices"].size(); device_idx++)
        {
            if(elgato_keylight_settings["devices"][device_idx].contains("ip"))
            {
                std::string elgato_keylight_ip = elgato_keylight_settings["devices"][device_idx]["ip"];

                ElgatoKeyLightController*     controller     = new ElgatoKeyLightController(elgato_keylight_ip);
                RGBController_ElgatoKeyLight* rgb_controller = new RGBController_ElgatoKeyLight(controller);

                detected_controllers.push_back(rgb_controller);
            }
        }
    }

    return(detected_controllers);
}

REGISTER_DETECTOR("ElgatoKeyLight", DetectElgatoKeyLightControllers);
