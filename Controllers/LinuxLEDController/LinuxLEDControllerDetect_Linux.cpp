/*---------------------------------------------------------*\
| LinuxLEDControllerDetect_Linux.cpp                        |
|                                                           |
|   Detector for Linux sysfs LEDs                           |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      25 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <string>
#include "DetectionManager.h"
#include "ResourceManager.h"
#include "RGBController_LinuxLED_Linux.h"
#include "SettingsManager.h"

DetectedControllers DetectLinuxLEDControllers()
{
    DetectedControllers detected_controllers;
    json                linux_led_settings;

    /*-----------------------------------------------------*\
    | Get Linux LED settings from settings manager          |
    \*-----------------------------------------------------*/
    linux_led_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("LinuxLEDDevices");

    /*-----------------------------------------------------*\
    | If the LinuxLED settings contains devices, process    |
    \*-----------------------------------------------------*/
    if(linux_led_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < linux_led_settings["devices"].size(); device_idx++)
        {
            std::string name;
            std::string red_path;
            std::string green_path;
            std::string blue_path;
            std::string rgb_path;

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

            if(linux_led_settings["devices"][device_idx].contains("rgb_path"))
            {
                rgb_path = linux_led_settings["devices"][device_idx]["rgb_path"];
            }

            LinuxLEDController*     controller     = new LinuxLEDController(name);
            controller->OpenRedPath(red_path);
            controller->OpenGreenPath(green_path);
            controller->OpenBluePath(blue_path);
            controller->OpenRgbPath(rgb_path);

            RGBController_LinuxLED* rgb_controller = new RGBController_LinuxLED(controller);

            detected_controllers.push_back(rgb_controller);
        }
    }

    return(detected_controllers);
}

REGISTER_DETECTOR("Linux LED", DetectLinuxLEDControllers);
