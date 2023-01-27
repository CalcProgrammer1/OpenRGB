#include "Detector.h"
#include "EspurnaController.h"
#include "RGBController.h"
#include "RGBController_Espurna.h"
#include "SettingsManager.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************************\
*                                                                                          *
*   DetectEspurnaControllers                                                               *
*                                                                                          *
*       Detect devices supported by the Espurna driver                                     *
*                                                                                          *
\******************************************************************************************/

void DetectEspurnaControllers()
{
    json espurna_settings;

    /*-------------------------------------------------*\
    | Get Espurna settings from settings manager        |
    \*-------------------------------------------------*/
    espurna_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("EspurnaDevices");

    /*-------------------------------------------------*\
    | If the Espurna settings contains devices, process |
    \*-------------------------------------------------*/
    if(espurna_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < espurna_settings["devices"].size(); device_idx++)
        {
            std::string ip;
            std::string port;
            std::string apikey;

            if(espurna_settings["devices"][device_idx].contains("ip"))
            {
                ip = espurna_settings["devices"][device_idx]["ip"];
            }

            if(espurna_settings["devices"][device_idx].contains("port"))
            {
                if(espurna_settings["devices"][device_idx]["port"].type() == json::value_t::string)
                {
                    port = espurna_settings["devices"][device_idx]["port"];
                }
                else
                {
                    port = std::to_string((unsigned int)espurna_settings["devices"][device_idx]["port"]);
                }
            }

            if(espurna_settings["devices"][device_idx].contains("apikey"))
            {
                apikey = espurna_settings["devices"][device_idx]["apikey"];
            }

            std::string value = ip + "," + port + "," + apikey;

            EspurnaController*     controller     = new EspurnaController();
            controller->Initialize((char *)value.c_str());

            RGBController_Espurna* rgb_controller = new RGBController_Espurna(controller);

            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
    }

}   /* DetectEspurnaControllers() */

REGISTER_DETECTOR("Espurna", DetectEspurnaControllers);
