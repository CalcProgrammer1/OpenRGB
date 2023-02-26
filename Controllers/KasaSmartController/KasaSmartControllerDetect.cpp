#include "Detector.h"
#include "KasaSmartController.h"
#include "RGBController.h"
#include "RGBController_KasaSmart.h"
#include "SettingsManager.h"
#include <vector>
#include <cctype>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************************\
*                                                                                          *
*   DetectKasaSmartControllers                                                             *
*                                                                                          *
*       Detect Kasa Smart devices                                                          *
*                                                                                          *
\******************************************************************************************/

void DetectKasaSmartControllers()
{
    json kasa_smart_settings;

    /*---------------------------------------------*\
    | Get Kasa Smart settings from settings manager |
    \*---------------------------------------------*/
    kasa_smart_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("KasaSmartDevices");

    /*---------------------------------------------*\
    | If the Wiz settings contains devices, process |
    \*---------------------------------------------*/
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
                ResourceManager::get()->RegisterRGBController(rgb_controller);
            }
        }
    }

}   /* DetectKasaSmartControllers() */

REGISTER_DETECTOR("KasaSmart", DetectKasaSmartControllers);
