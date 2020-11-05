#include "Detector.h"
#include "PhilipsWizController.h"
#include "RGBController.h"
#include "RGBController_PhilipsWiz.h"
#include "SettingsManager.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************************\
*                                                                                          *
*   DetectPhilipsWizControllers                                                            *
*                                                                                          *
*       Detect Philips Wiz devices                                                         *
*                                                                                          *
\******************************************************************************************/

void DetectPhilipsWizControllers(std::vector<RGBController*> &rgb_controllers)
{
    PhilipsWizController* new_controller;
    RGBController_PhilipsWiz* new_rgbcontroller;

    json                    wiz_settings;

    /*-------------------------------------------------*\
    | Get Philips Wiz settings from settings manager    |
    \*-------------------------------------------------*/
    wiz_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("Setting_PhilipsWizDevices");

    /*-------------------------------------------------*\
    | If the Wiz settings contains devices, process     |
    \*-------------------------------------------------*/
    if(wiz_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < wiz_settings["devices"].size(); device_idx++)
        {
            if(wiz_settings["devices"][device_idx].contains("ip"))
            {
                std::string wiz_ip = wiz_settings["devices"][device_idx]["ip"];

                new_controller = new PhilipsWizController(wiz_ip);
                new_rgbcontroller = new RGBController_PhilipsWiz(new_controller);

                rgb_controllers.push_back(new_rgbcontroller);
            }
        }
    }

}   /* DetectPhilipsWizControllers() */

REGISTER_DETECTOR("Philips Wiz", DetectPhilipsWizControllers);
