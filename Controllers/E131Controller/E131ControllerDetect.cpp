/*---------------------------------------------------------*\
| E131ControllerDetect.cpp                                  |
|                                                           |
|   Detector for E1.31 devices                              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <fcntl.h>
#include <string>
#include <vector>
#include "Detector.h"
#include "RGBController.h"
#include "RGBController_E131.h"
#include "SettingsManager.h"

/******************************************************************************************\
*                                                                                          *
*   DetectE131Controllers                                                                  *
*                                                                                          *
*       Detect devices supported by the E131 driver                                        *
*                                                                                          *
\******************************************************************************************/

void DetectE131Controllers()
{
    json                e131_settings;

    std::vector<std::vector<E131Device>> device_lists;
	E131Device dev;

    /*-------------------------------------------------*\
    | Get E1.31 settings from settings manager          |
    \*-------------------------------------------------*/
    e131_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("E131Devices");

    /*-------------------------------------------------*\
    | If the E1.31 settings contains devices, process   |
    \*-------------------------------------------------*/
    if(e131_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < e131_settings["devices"].size(); device_idx++)
        {
            /*-------------------------------------------------*\
            | Clear E1.31 device data                           |
            \*-------------------------------------------------*/
            dev.name           = "";
            dev.ip             = "";
            dev.type           = ZONE_TYPE_SINGLE;
            dev.num_leds       = 0;
            dev.rgb_order      = E131_RGB_ORDER_RBG;
            dev.matrix_order   = E131_MATRIX_ORDER_HORIZONTAL_TOP_LEFT;
            dev.matrix_width   = 0;
            dev.matrix_height  = 0;
            dev.start_channel  = 1;
            dev.start_universe = 1;
            dev.keepalive_time = 0;
            dev.universe_size  = 512;

            if(e131_settings["devices"][device_idx].contains("name"))
            {
                dev.name = e131_settings["devices"][device_idx]["name"];
            }

            if(e131_settings["devices"][device_idx].contains("ip"))
            {
                dev.ip = e131_settings["devices"][device_idx]["ip"];
            }

            if(e131_settings["devices"][device_idx].contains("num_leds"))
            {
                dev.num_leds = e131_settings["devices"][device_idx]["num_leds"];
            }

            if(e131_settings["devices"][device_idx].contains("start_universe"))
            {
                dev.start_universe = e131_settings["devices"][device_idx]["start_universe"];
            }

            if(e131_settings["devices"][device_idx].contains("start_channel"))
            {
                dev.start_channel = e131_settings["devices"][device_idx]["start_channel"];
            }

            if(e131_settings["devices"][device_idx].contains("keepalive_time"))
            {
                dev.keepalive_time = e131_settings["devices"][device_idx]["keepalive_time"];
            }

            if(e131_settings["devices"][device_idx].contains("matrix_order"))
            {
                if(e131_settings["devices"][device_idx]["matrix_order"].is_string())
                {
                    std::string matrix_order_val = e131_settings["devices"][device_idx]["matrix_order"];

                    if(matrix_order_val == "HORIZONTAL_TOP_LEFT")
                    {
                        dev.matrix_order = E131_MATRIX_ORDER_HORIZONTAL_TOP_LEFT;
                    }
                    else if(matrix_order_val == "HORIZONTAL_TOP_RIGHT")
                    {
                        dev.matrix_order = E131_MATRIX_ORDER_HORIZONTAL_TOP_RIGHT;
                    }
                    else if(matrix_order_val == "HORIZONTAL_BOTTOM_LEFT")
                    {
                        dev.matrix_order = E131_MATRIX_ORDER_HORIZONTAL_BOTTOM_LEFT;
                    }
                    else if(matrix_order_val == "HORIZONTAL_BOTTOM_RIGHT")
                    {
                        dev.matrix_order = E131_MATRIX_ORDER_HORIZONTAL_BOTTOM_RIGHT;
                    }
                    else if(matrix_order_val == "VERTICAL_TOP_LEFT")
                    {
                        dev.matrix_order = E131_MATRIX_ORDER_VERTICAL_TOP_LEFT;
                    }
                    else if(matrix_order_val == "VERTICAL_TOP_RIGHT")
                    {
                        dev.matrix_order = E131_MATRIX_ORDER_VERTICAL_TOP_RIGHT;
                    }
                    else if(matrix_order_val == "VERTICAL_BOTTOM_LEFT")
                    {
                        dev.matrix_order = E131_MATRIX_ORDER_VERTICAL_BOTTOM_LEFT;
                    }
                    else if(matrix_order_val == "VERTICAL_BOTTOM_RIGHT")
                    {
                        dev.matrix_order = E131_MATRIX_ORDER_VERTICAL_BOTTOM_RIGHT;
                    }
                }
                else
                {
                    dev.matrix_order = e131_settings["devices"][device_idx]["matrix_order"];
                }
            }

            if(e131_settings["devices"][device_idx].contains("rgb_order"))
            {
                if(e131_settings["devices"][device_idx]["rgb_order"].is_string())
                {
                    std::string rgb_order_val = e131_settings["devices"][device_idx]["rgb_order"];

                    if(rgb_order_val == "RGB")
                    {
                        dev.rgb_order = E131_RGB_ORDER_RGB;
                    }
                    else if(rgb_order_val == "RBG")
                    {
                        dev.rgb_order = E131_RGB_ORDER_RBG;
                    }
                    else if(rgb_order_val == "GRB")
                    {
                        dev.rgb_order = E131_RGB_ORDER_GRB;
                    }
                    else if(rgb_order_val == "GBR")
                    {
                        dev.rgb_order = E131_RGB_ORDER_GBR;
                    }
                    else if(rgb_order_val == "BRG")
                    {
                        dev.rgb_order = E131_RGB_ORDER_BRG;
                    }
                    else if(rgb_order_val == "BGR")
                    {
                        dev.rgb_order = E131_RGB_ORDER_BGR;
                    }
                }
                else
                {
                    dev.rgb_order = e131_settings["devices"][device_idx]["rgb_order"];
                }
            }

            if(e131_settings["devices"][device_idx].contains("matrix_width"))
            {
                dev.matrix_width = e131_settings["devices"][device_idx]["matrix_width"];
            }

            if(e131_settings["devices"][device_idx].contains("matrix_height"))
            {
                dev.matrix_height = e131_settings["devices"][device_idx]["matrix_height"];
            }

            if(e131_settings["devices"][device_idx].contains("universe_size"))
            {
                dev.universe_size = e131_settings["devices"][device_idx]["universe_size"];
            }

            if(e131_settings["devices"][device_idx].contains("type"))
            {
                if(e131_settings["devices"][device_idx]["type"].is_string())
                {
                    std::string type_val = e131_settings["devices"][device_idx]["type"];

                    if(type_val == "SINGLE")
                    {
                        dev.type = ZONE_TYPE_SINGLE;
                    }
                    else if(type_val == "LINEAR")
                    {
                        dev.type = ZONE_TYPE_LINEAR;
                    }
                    else if(type_val == "MATRIX")
                    {
                        dev.type = ZONE_TYPE_MATRIX;
                    }
                }
                else
                {
                    dev.type = e131_settings["devices"][device_idx]["type"];
                }
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
                    | Determine if there is any overlap between this device and |
                    | any existing device list                                  |
                    | Offset the end by two - one because the range is 1-512    |
                    | rather than 0-511, and one because the start channel is   |
                    | included in the first set of 3 channels.                  |
                    \*---------------------------------------------------------*/
                    unsigned int dev_start  = dev.start_universe;
                    unsigned int list_start = device_lists[list_idx][device_idx].start_universe;
                    unsigned int dev_end    = dev.start_universe + ((dev.start_channel + (3 * dev.num_leds) - 2) / 512);
                    unsigned int list_end   = device_lists[list_idx][device_idx].start_universe + ((device_lists[list_idx][device_idx].start_channel + (3 * device_lists[list_idx][device_idx].num_leds) - 2) / 512);
                    std::string  dev_ip     = dev.ip;
                    std::string  list_ip    = device_lists[list_idx][device_idx].ip;

                    bool overlap = dev_ip == list_ip && !(dev_end < list_start || list_end < dev_start);

                    /*---------------------------------------------------------*\
                    | Check if any universes used by this new device exist in   |
                    | the existing device.  If so, add the new device to the    |
                    | existing list.                                            |
                    \*---------------------------------------------------------*/
                    if(overlap)
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
                std::vector<E131Device> new_list;

                new_list.push_back(dev);

                device_lists.push_back(new_list);
            }
        }

        for(unsigned int list_idx = 0; list_idx < device_lists.size(); list_idx++)
        {
            RGBController_E131* rgb_controller;
            rgb_controller = new RGBController_E131(device_lists[list_idx]);
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
    }

}   /* DetectE131Controllers() */

REGISTER_DETECTOR("E1.31", DetectE131Controllers);
