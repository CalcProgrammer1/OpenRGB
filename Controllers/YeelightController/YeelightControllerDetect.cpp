#include "Detector.h"
#include "YeelightController.h"
#include "RGBController.h"
#include "RGBController_Yeelight.h"
#include "SettingsManager.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************************\
*                                                                                          *
*   DetectYeelightControllers                                                              *
*                                                                                          *
*       Detect Yeelight devices                                                            *
*                                                                                          *
\******************************************************************************************/

void DetectYeelightControllers(std::vector<RGBController*> &rgb_controllers)
{
    YeelightController* new_controller;
    RGBController_Yeelight* new_rgbcontroller;

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
            if(yeelight_settings["devices"][device_idx].contains("ip"))
            {
                std::string yeelight_ip = yeelight_settings["devices"][device_idx]["ip"];
                bool        music_mode  = false;

                if(yeelight_settings["devices"][device_idx].contains("music_mode"))
                {
                    music_mode = yeelight_settings["devices"][device_idx]["music_mode"];
                }

                new_controller = new YeelightController(yeelight_ip, music_mode);
                new_rgbcontroller = new RGBController_Yeelight(new_controller);

                rgb_controllers.push_back(new_rgbcontroller);
            }
        }
    }

}   /* DetectYeelightControllers() */

REGISTER_DETECTOR("Yeelight", DetectYeelightControllers);
