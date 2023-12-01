#include "Detector.h"
#include "GoveeController.h"
#include "RGBController.h"
#include "RGBController_Govee.h"
#include "SettingsManager.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************************\
*                                                                                          *
*   DetectGoveeControllers                                                                 *
*                                                                                          *
*       Detect Govee devices                                                               *
*                                                                                          *
\******************************************************************************************/

void DetectGoveeControllers()
{
    json                    govee_settings;

    /*-------------------------------------------------*\
    | Get Govee settings from settings manager          |
    \*-------------------------------------------------*/
    govee_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("GoveeDevices");

    /*-------------------------------------------------*\
    | If the Govee settings contains devices, process   |
    \*-------------------------------------------------*/
    if(govee_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < govee_settings["devices"].size(); device_idx++)
        {
            if(govee_settings["devices"][device_idx].contains("ip"))
            {
                std::string govee_ip  = govee_settings["devices"][device_idx]["ip"];

                GoveeController*     controller     = new GoveeController(govee_ip);
                RGBController_Govee* rgb_controller = new RGBController_Govee(controller);

                ResourceManager::get()->RegisterRGBController(rgb_controller);
            }
        }
    }

}   /* DetectGoveeControllers() */

REGISTER_DETECTOR("Govee", DetectGoveeControllers);
