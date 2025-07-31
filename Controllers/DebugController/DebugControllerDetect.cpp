/*---------------------------------------------------------*\
| DebugControllerDetect.cpp                                 |
|                                                           |
|   Detector for debug devices                              |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      31 Jul 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "RGBController.h"
#include "RGBController_Debug.h"
#include "SettingsManager.h"

/******************************************************************************************\
*                                                                                          *
*   DetectDebugControllers                                                                 *
*                                                                                          *
*       Add debug controllers based on the DebugDevices key in the settings json           *
*                                                                                          *
\******************************************************************************************/

void DetectDebugControllers()
{
    json            debug_settings;

    /*-----------------------------------------------------*\
    | Get Debug Device settings from settings manager       |
    \*-----------------------------------------------------*/
    debug_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("DebugDevices");

    /*-----------------------------------------------------*\
    | If the Debug settings contains devices, process       |
    \*-----------------------------------------------------*/
    if(debug_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < debug_settings["devices"].size(); device_idx++)
        {
            RGBController_Debug * debug_controller = new RGBController_Debug(false, debug_settings["devices"][device_idx]);
            ResourceManager::get()->RegisterRGBController(debug_controller);
        }
    }

    if (debug_settings.contains("CustomDevices"))
    {
        for(unsigned int device_idx = 0; device_idx < debug_settings["CustomDevices"].size(); device_idx++)
        {
            json custom_device_settings = debug_settings["CustomDevices"][device_idx];

            /*---------------------------------------------*\
            | If ANY of the attributes are missing then go  |
            | ahead and skip the entry                      |
            \*---------------------------------------------*/
            if(
              !custom_device_settings.contains("DeviceName")        ||
              !custom_device_settings.contains("DeviceType")        ||
              !custom_device_settings.contains("DeviceDescription") ||
              !custom_device_settings.contains("DeviceLocation")    ||
              !custom_device_settings.contains("DeviceVersion")     ||
              !custom_device_settings.contains("DeviceSerial")      ||
              !custom_device_settings.contains("DeviceZones")
              )
            {
                continue;
            }
            else
            {
                RGBController_Debug * debug_controller = new RGBController_Debug(true, custom_device_settings);
                ResourceManager::get()->RegisterRGBController(debug_controller);
            }
        }
    }

}   /* DetectDebugControllers() */

REGISTER_DETECTOR("Debug Controllers", DetectDebugControllers);
