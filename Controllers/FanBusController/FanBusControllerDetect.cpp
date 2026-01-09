/*---------------------------------------------------------*\
| FanBusControllerDetect.cpp                                |
|                                                           |
|   Detector for FanBus devices                             |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      12 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "FanBusController.h"
#include "RGBController_FanBus.h"
#include "ResourceManager.h"
#include "SettingsManager.h"

void DetectFanBusControllers()
{
    FanBusInterface*    new_interface;
    json                fanbus_settings;

    /*-------------------------------------------------*\
    | Get LED Strip settings from settings manager      |
    \*-------------------------------------------------*/
    fanbus_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("FanBusDevices");

    /*-------------------------------------------------*\
    | If the LEDStrip settings contains devices, process|
    \*-------------------------------------------------*/
    if(fanbus_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < fanbus_settings["devices"].size(); device_idx++)
        {
            if(fanbus_settings["devices"][device_idx].contains("port"))
            {
                std::string port_val = fanbus_settings["devices"][device_idx]["port"];

                new_interface     = new FanBusInterface(port_val.c_str());

                std::vector<unsigned char> detected_controllers = new_interface->DetectControllers();

                for(unsigned int controller_idx = 0; controller_idx < detected_controllers.size(); controller_idx++)
                {
                    FanBusController*     controller     = new FanBusController(new_interface, detected_controllers[controller_idx]);
                    RGBController_FanBus* rgb_controller = new RGBController_FanBus(controller);

                    DetectionManager::get()->RegisterRGBController(rgb_controller);
                }
            }
        }
    }
}

REGISTER_DETECTOR("FanBus", DetectFanBusControllers);
