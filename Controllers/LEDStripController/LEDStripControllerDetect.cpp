#include "Detector.h"
#include "LEDStripController.h"
#include "RGBController.h"
#include "RGBController_LEDStrip.h"
#include "SettingsManager.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************************\
*                                                                                          *
*   DetectLEDStripControllers                                                              *
*                                                                                          *
*       Detect devices supported by the LEDStrip driver                                    *
*                                                                                          *
\******************************************************************************************/

void DetectLEDStripControllers(std::vector<RGBController*> &rgb_controllers)
{
    LEDStripController*     new_ledstrip;
    RGBController_LEDStrip* new_controller;
    json                    ledstrip_settings;
    LEDStripDevice          dev;

    /*-------------------------------------------------*\
    | Get LED Strip settings from settings manager      |
    \*-------------------------------------------------*/
    ledstrip_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("Setting_LEDStripDevices");

    /*-------------------------------------------------*\
    | If the LEDStrip settings contains devices, process|
    \*-------------------------------------------------*/
    if(ledstrip_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < ledstrip_settings["devices"].size(); device_idx++)
        {
            if(ledstrip_settings["devices"][device_idx].contains("port"))
            {
                dev.port = ledstrip_settings["devices"][device_idx]["port"];
            }

            if(ledstrip_settings["devices"][device_idx].contains("baud"))
            {
                dev.baud = ledstrip_settings["devices"][device_idx]["baud"];
            }

            if(ledstrip_settings["devices"][device_idx].contains("num_leds"))
            {
                dev.num_leds = ledstrip_settings["devices"][device_idx]["num_leds"];
            }

            std::string value = dev.port + "," + std::to_string(dev.baud) + "," + std::to_string(dev.num_leds);

            new_ledstrip = new LEDStripController();
            new_ledstrip->Initialize((char *)value.c_str());

            new_controller = new RGBController_LEDStrip(new_ledstrip);
            rgb_controllers.push_back(new_controller);
        }
    }

}   /* DetectLEDStripControllers() */

REGISTER_DETECTOR("LED Strip", DetectLEDStripControllers);
