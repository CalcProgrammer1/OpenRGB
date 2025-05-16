/*---------------------------------------------------------*\
| GoveeControllerDetect.cpp                                 |
|                                                           |
|   Detector for Govee wireless lighting devices            |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      01 Dec 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "Detector.h"
#include "GoveeController.h"
#include "RGBController.h"
#include "RGBController_Govee.h"
#include "SettingsManager.h"

/******************************************************************************************\
*                                                                                          *
*   DetectGoveeControllers                                                                 *
*                                                                                          *
*       Detect Govee devices                                                               *
*                                                                                          *
\******************************************************************************************/

void DetectGoveeControllers()
{
    json                    govee_settings;

    /*-----------------------------------------------------*\
    | Get Govee settings from settings manager              |
    \*-----------------------------------------------------*/
    govee_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("GoveeDevices");

    /*-----------------------------------------------------*\
    | If the Govee settings contains devices, process       |
    \*-----------------------------------------------------*/
    if(govee_settings.contains("devices"))
    {
        GoveeController::ReceiveThreadRun = false;

        if(govee_settings["devices"].size() > 0)
        {
            /*---------------------------------------------*\
            | Open a UDP client sending to and receiving    |
            | from the Govee Multicast IP, send port 4001   |
            | and receive port 4002                         |
            \*---------------------------------------------*/
            GoveeController::broadcast_port.udp_client("239.255.255.250", "4001", "4002");
            GoveeController::broadcast_port.udp_join_multicast_group("239.255.255.250");

            /*---------------------------------------------*\
            | Start a thread to handle responses received   |
            | from the Govee device                         |
            \*---------------------------------------------*/
            GoveeController::ReceiveThreadRun = true;
            GoveeController::ReceiveThread = new std::thread(&GoveeController::ReceiveBroadcastThreadFunction);
        }

        for(unsigned int device_idx = 0; device_idx < govee_settings["devices"].size(); device_idx++)
        {
            if(govee_settings["devices"][device_idx].contains("ip"))
            {
                std::string govee_ip  = govee_settings["devices"][device_idx]["ip"];

                GoveeController*     controller     = new GoveeController(govee_ip);
                RGBController_Govee* rgb_controller = new RGBController_Govee(controller);

                ResourceManager::get()->RegisterRGBController(rgb_controller);
            }
        }

        /*-------------------------------------------------*\
        | All controllers have been created, the broadcast  |
        | receiver thread is no longer needed and can be    |
        | shut down                                         |
        \*-------------------------------------------------*/
        if(GoveeController::ReceiveThreadRun)
        {
            GoveeController::ReceiveThreadRun = false;
            GoveeController::ReceiveThread->join();
            delete GoveeController::ReceiveThread;
            GoveeController::broadcast_port.tcp_close();
        }
    }

}   /* DetectGoveeControllers() */

REGISTER_DETECTOR("Govee", DetectGoveeControllers);
