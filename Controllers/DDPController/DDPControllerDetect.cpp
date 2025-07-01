/*---------------------------------------------------------*\
| DDPControllerDetect.cpp                                   |
|                                                           |
|   Detector for DDP devices                                |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string>
#include <vector>
#include "Detector.h"
#include "RGBController.h"
#include "RGBController_DDP.h"
#include "SettingsManager.h"
#include "LogManager.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

void DetectDDPControllers()
{
    json ddp_settings;
    std::vector<std::vector<DDPDevice>> device_lists;
    DDPDevice dev;

    ddp_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("DDPDevices");

    if(ddp_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < ddp_settings["devices"].size(); device_idx++)
        {
            dev.name            = "";
            dev.ip              = "";
            dev.port            = DDP_DEFAULT_PORT;
            dev.num_leds        = 0;

            if(ddp_settings["devices"][device_idx].contains("name"))
                dev.name = ddp_settings["devices"][device_idx]["name"];
            if(ddp_settings["devices"][device_idx].contains("ip"))
                dev.ip = ddp_settings["devices"][device_idx]["ip"];
            if(ddp_settings["devices"][device_idx].contains("port"))
                dev.port = ddp_settings["devices"][device_idx]["port"];
            if(ddp_settings["devices"][device_idx].contains("num_leds"))
                dev.num_leds = ddp_settings["devices"][device_idx]["num_leds"];

            if(dev.name.empty())
                dev.name = "DDP Device " + std::to_string(device_idx + 1);
            if(dev.ip.empty())
            {
                continue;
            }
            if(dev.num_leds == 0)
            {
                continue;
            }

            bool device_added_to_existing_list = false;

            for(unsigned int list_idx = 0; list_idx < device_lists.size(); list_idx++)
            {
                for(unsigned int existing_device_idx = 0; existing_device_idx < device_lists[list_idx].size(); existing_device_idx++)
                {
                    if(dev.ip == device_lists[list_idx][existing_device_idx].ip && 
                       dev.port == device_lists[list_idx][existing_device_idx].port)
                    {
                        device_lists[list_idx].push_back(dev);
                        device_added_to_existing_list = true;
                        break;
                    }
                }
                if(device_added_to_existing_list)
                    break;
            }

            if(!device_added_to_existing_list)
            {
                std::vector<DDPDevice> new_list;
                new_list.push_back(dev);
                device_lists.push_back(new_list);
            }
        }

        for(unsigned int list_idx = 0; list_idx < device_lists.size(); list_idx++)
        {
            RGBController_DDP* rgb_controller = new RGBController_DDP(device_lists[list_idx]);
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
    }
}

REGISTER_DETECTOR("DDP", DetectDDPControllers);
