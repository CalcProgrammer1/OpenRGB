/*---------------------------------------------------------*\
| PhilipsWizControllerDetect.cpp                            |
|                                                           |
|   Detector for Philips Wiz                                |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      03 Nov 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "PhilipsWizController.h"
#include "RGBController_PhilipsWiz.h"
#include "SettingsManager.h"

/******************************************************************************************\
*                                                                                          *
*   DetectPhilipsWizControllers                                                            *
*                                                                                          *
*       Detect Philips Wiz devices                                                         *
*                                                                                          *
\******************************************************************************************/

void DetectPhilipsWizControllers()
{
    json                    wiz_settings;

    /*-------------------------------------------------*\
    | Get Philips Wiz settings from settings manager    |
    \*-------------------------------------------------*/
    wiz_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("PhilipsWizDevices");

    /*-------------------------------------------------*\
    | If the Wiz settings contains devices, process     |
    \*-------------------------------------------------*/
    if(wiz_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < wiz_settings["devices"].size(); device_idx++)
        {
            if(wiz_settings["devices"][device_idx].contains("ip"))
            {
                std::string wiz_ip  = wiz_settings["devices"][device_idx]["ip"];

                bool wiz_cool       = false;

                if(wiz_settings["devices"][device_idx].contains("use_cool_white"))
                {
                    wiz_cool        = wiz_settings["devices"][device_idx]["use_cool_white"];
                }

                bool wiz_warm       = false;
                if(wiz_settings["devices"][device_idx].contains("use_warm_white"))
                {
                    wiz_warm        = wiz_settings["devices"][device_idx]["use_warm_white"];
                }
                std::string wiz_white_strategy = "Average";
                if(wiz_settings["devices"][device_idx].contains("selected_white_strategy"))
                {
                    wiz_white_strategy = wiz_settings["devices"][device_idx]["selected_white_strategy"];
                }

                PhilipsWizController*     controller     = new PhilipsWizController(wiz_ip, wiz_cool, wiz_warm, wiz_white_strategy);
                RGBController_PhilipsWiz* rgb_controller = new RGBController_PhilipsWiz(controller);

                ResourceManager::get()->RegisterRGBController(rgb_controller);
            }
        }
    }

}   /* DetectPhilipsWizControllers() */

REGISTER_DETECTOR("Philips Wiz", DetectPhilipsWizControllers);
