#include "Detector.h"
#include "LinuxLEDController.h"
#include "RGBController.h"
#include "RGBController_LinuxLED.h"
#include "SettingsManager.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include <string>


/******************************************************************************************\
*                                                                                          *
*   DetectLinuxLEDControllers                                                              *
*                                                                                          *
*       Detect devices supported by the LinuxLED driver                                    *
*                                                                                          *
\******************************************************************************************/

void DetectLinuxLEDControllers(std::vector<RGBController*> &rgb_controllers)
{
    LinuxLEDController*     new_controller;
    RGBController_LinuxLED* new_rgbcontroller;
    json                    linux_led_settings;

    /*-------------------------------------------------*\
    | Get Linux LED settings from settings manager      |
    \*-------------------------------------------------*/
    linux_led_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("LinuxLEDDevices");

    /*-------------------------------------------------*\
    | If the LinuxLED settings contains devices, process|
    \*-------------------------------------------------*/
    if(linux_led_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < linux_led_settings["devices"].size(); device_idx++)
        {
            std::string name;
            std::string red_path;
            std::string green_path;
            std::string blue_path;

            if(linux_led_settings["devices"][device_idx].contains("name"))
            {
                name = linux_led_settings["devices"][device_idx]["name"];
            }

            if(linux_led_settings["devices"][device_idx].contains("red_path"))
            {
                red_path = linux_led_settings["devices"][device_idx]["red_path"];
            }

            if(linux_led_settings["devices"][device_idx].contains("green_path"))
            {
                green_path = linux_led_settings["devices"][device_idx]["green_path"];
            }

            if(linux_led_settings["devices"][device_idx].contains("blue_path"))
            {
                blue_path = linux_led_settings["devices"][device_idx]["blue_path"];
            }

            new_controller      = new LinuxLEDController();
            new_controller->OpenRedPath(red_path);
            new_controller->OpenGreenPath(green_path);
            new_controller->OpenBluePath(blue_path);

            new_rgbcontroller       = new RGBController_LinuxLED(new_controller);
            new_rgbcontroller->name = name;
            rgb_controllers.push_back(new_rgbcontroller);
        }
    }

}   /* DetectLinuxLEDControllers() */

REGISTER_DETECTOR("Linux LED", DetectLinuxLEDControllers);
