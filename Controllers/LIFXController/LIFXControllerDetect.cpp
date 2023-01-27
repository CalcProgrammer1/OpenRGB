#include "Detector.h"
#include "LIFXController.h"
#include "RGBController.h"
#include "RGBController_LIFX.h"
#include "SettingsManager.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

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
                std::string lifx_ip = lifx_settings["devices"][device_idx]["ip"];
                std::string name    = lifx_settings["devices"][device_idx]["name"];

                LIFXController*     controller     = new LIFXController(lifx_ip, name);
                RGBController_LIFX* rgb_controller = new RGBController_LIFX(controller);

                ResourceManager::get()->RegisterRGBController(rgb_controller);
            }
        }
    }

}   /* DetectLIFXControllers() */

REGISTER_DETECTOR("LIFX", DetectLIFXControllers);
