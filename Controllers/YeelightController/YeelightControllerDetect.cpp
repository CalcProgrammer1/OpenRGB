/*---------------------------------------------------------*\
| YeelightControllerDetect.cpp                              |
|                                                           |
|   Detector for Yeelight                                   |
|                                                           |
|   Adam Honse (CalcProgrammer1)                18 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "YeelightController.h"
#include "RGBController_Yeelight.h"
#include "SettingsManager.h"

/******************************************************************************************\
*                                                                                          *
*   DetectYeelightControllers                                                              *
*                                                                                          *
*       Detect Yeelight devices                                                            *
*                                                                                          *
\******************************************************************************************/

void DetectYeelightControllers()
{
    json                    yeelight_settings;

    /*-------------------------------------------------*\
    | Get Yeelight settings from settings manager    |
    \*-------------------------------------------------*/
    yeelight_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("YeelightDevices");

    /*-------------------------------------------------*\
    | If the Yeelight settings contains devices, process|
    \*-------------------------------------------------*/
    if(yeelight_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < yeelight_settings["devices"].size(); device_idx++)
        {
            std::string yeelight_host_ip;

            if(yeelight_settings["devices"][device_idx].contains("host_ip"))
            {
                yeelight_host_ip = yeelight_settings["devices"][device_idx]["host_ip"];
            }

            if(yeelight_settings["devices"][device_idx].contains("ip"))
            {
                std::string yeelight_ip = yeelight_settings["devices"][device_idx]["ip"];
                bool        music_mode  = false;

                if(yeelight_settings["devices"][device_idx].contains("music_mode"))
                {
                    music_mode = yeelight_settings["devices"][device_idx]["music_mode"];
                }

                YeelightController*     controller     = new YeelightController(yeelight_ip, yeelight_host_ip, music_mode);
                RGBController_Yeelight* rgb_controller = new RGBController_Yeelight(controller);

                ResourceManager::get()->RegisterRGBController(rgb_controller);
            }
        }
    }

}   /* DetectYeelightControllers() */

REGISTER_DETECTOR("Yeelight", DetectYeelightControllers);
