/*---------------------------------------------------------*\
| DMXControllerDetect.cpp                                   |
|                                                           |
|   Detector for DMX devices                                |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <fcntl.h>
#include <string>
#include <vector>
#include "Detector.h"
#include "RGBController_DMX.h"
#include "SettingsManager.h"

/******************************************************************************************\
*                                                                                          *
*   DetectDMXControllers                                                                   *
*                                                                                          *
*       Detect devices supported by the DMX driver                                         *
*                                                                                          *
\******************************************************************************************/

void DetectDMXControllers()
{
    json                dmx_settings;

    std::vector<std::vector<DMXDevice>> device_lists;
	DMXDevice dev;

    /*-------------------------------------------------*\
    | Get DMX settings from settings manager            |
    \*-------------------------------------------------*/
    dmx_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("DMXDevices");

    /*-------------------------------------------------*\
    | If the DMX settings contains devices, process     |
    \*-------------------------------------------------*/
    if(dmx_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < dmx_settings["devices"].size(); device_idx++)
        {
            /*-------------------------------------------------*\
            | Clear DMX device data                             |
            \*-------------------------------------------------*/
            dev.name           = "";
            dev.keepalive_time = 0;

            if(dmx_settings["devices"][device_idx].contains("name"))
            {
                dev.name = dmx_settings["devices"][device_idx]["name"];
            }

            if(dmx_settings["devices"][device_idx].contains("port"))
            {
                dev.port = dmx_settings["devices"][device_idx]["port"];
            }

            if(dmx_settings["devices"][device_idx].contains("keepalive_time"))
            {
                dev.keepalive_time = dmx_settings["devices"][device_idx]["keepalive_time"];
            }

            if(dmx_settings["devices"][device_idx].contains("red_channel"))
            {
                dev.red_channel = dmx_settings["devices"][device_idx]["red_channel"];
            }

            if(dmx_settings["devices"][device_idx].contains("green_channel"))
            {
                dev.green_channel = dmx_settings["devices"][device_idx]["green_channel"];
            }

            if(dmx_settings["devices"][device_idx].contains("blue_channel"))
            {
                dev.blue_channel = dmx_settings["devices"][device_idx]["blue_channel"];
            }

            if(dmx_settings["devices"][device_idx].contains("brightness_channel"))
            {
                dev.brightness_channel = dmx_settings["devices"][device_idx]["brightness_channel"];
            }

            /*---------------------------------------------------------*\
            | Determine whether to create a new list or add this device |
            | to an existing list.  A device is added to an existing    |
            | list if both devices share one or more universes for the  |
            | same output destination                                   |
            \*---------------------------------------------------------*/
            bool device_added_to_existing_list = false;

            /*---------------------------------------------------------*\
            | Track grouping for all controllers.                       |
            \*---------------------------------------------------------*/
            for(unsigned int list_idx = 0; list_idx < device_lists.size(); list_idx++)
            {
                for(unsigned int device_idx = 0; device_idx < device_lists[list_idx].size(); device_idx++)
                {
                    /*---------------------------------------------------------*\
                    | Check if the port used by this new device is the same as  |
                    | in the existing device.  If so, add the new device to the |
                    | existing list.                                            |
                    \*---------------------------------------------------------*/
                    if(1)
                    {
                        device_lists[list_idx].push_back(dev);
                        device_added_to_existing_list = true;
                        break;
                    }
                }

                if(device_added_to_existing_list)
                {
                    break;
                }
            }

            /*---------------------------------------------------------*\
            | If the device did not overlap with existing devices,      |
            | create a new list for it                                  |
            \*---------------------------------------------------------*/
            if(!device_added_to_existing_list)
            {
                std::vector<DMXDevice> new_list;

                new_list.push_back(dev);

                device_lists.push_back(new_list);
            }
        }


        for(unsigned int list_idx = 0; list_idx < device_lists.size(); list_idx++)
        {
            RGBController_DMX* rgb_controller;
            rgb_controller = new RGBController_DMX(device_lists[list_idx]);
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
    }

}   /* DetectDMXControllers() */

REGISTER_DETECTOR("DMX", DetectDMXControllers);
