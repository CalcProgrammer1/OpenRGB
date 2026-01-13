/*---------------------------------------------------------*\
| ElgatoLightStripControllerDetect.cpp                      |
|                                                           |
|   Detector for Elgato Light Strip                         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "ElgatoLightStripController.h"
#include "ResourceManager.h"
#include "RGBController_ElgatoLightStrip.h"
#include "SettingsManager.h"

DetectedControllers DetectElgatoLightStripControllers()
{
    DetectedControllers detected_controllers;
    json                elgato_lightstrip_settings;

    /*-----------------------------------------------------*\
    | Get LightStrip settings from settings manager         |
    \*-----------------------------------------------------*/
    elgato_lightstrip_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("ElgatoLightStripDevices");

    /*-----------------------------------------------------*\
    | If the Elgato Light Strip settings contains devices,  |
    | process                                               |
    \*-----------------------------------------------------*/
    if(elgato_lightstrip_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < elgato_lightstrip_settings["devices"].size(); device_idx++)
        {
            if(elgato_lightstrip_settings["devices"][device_idx].contains("ip"))
            {
                std::string elgato_lightstrip_ip = elgato_lightstrip_settings["devices"][device_idx]["ip"];

                ElgatoLightStripController*     controller     = new ElgatoLightStripController(elgato_lightstrip_ip);
                RGBController_ElgatoLightStrip* rgb_controller = new RGBController_ElgatoLightStrip(controller);

                detected_controllers.push_back(rgb_controller);
            }
        }
    }

    return(detected_controllers);
}

REGISTER_DETECTOR("Elgato Light Strip", DetectElgatoLightStripControllers);
