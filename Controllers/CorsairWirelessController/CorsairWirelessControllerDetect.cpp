/*---------------------------------------------------------*\
| CorsairWirelessControllerDetect.cpp                       |
|                                                           |
|   Detector for Corsair wireless keyboard                  |
|                                                           |
|   Adam Honse (CalcProgrammer1)                08 May 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "CorsairWirelessController.h"
#include "RGBController.h"
#include "RGBController_CorsairWireless.h"

/*-----------------------------------------------------*\
| Corsair vendor ID                                     |
\*-----------------------------------------------------*/
#define CORSAIR_VID                     0x1B1C

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define CORSAIR_K57_RGB_WIRED_PID       0x1B6E
#define CORSAIR_K57_RGB_WIRELESS_PID    0x1B62

/******************************************************************************************\
*                                                                                          *
*   DetectCorsairWirelessControllers                                                       *
*                                                                                          *
*       Tests the USB address to see if a Corsair RGB Keyboard controller exists there.    *
*                                                                                          *
\******************************************************************************************/

void DetectCorsairWirelessControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        CorsairWirelessController* controller = new CorsairWirelessController(dev, info->path);
        controller->SetName(name);

        if(controller->GetDeviceType() != DEVICE_TYPE_UNKNOWN)
        {
            RGBController_CorsairWireless* rgb_controller = new RGBController_CorsairWireless(controller);
            DetectionManager::get()->RegisterRGBController(rgb_controller);
        }
        else
        {
            delete controller;
        }
    }
}   /* DetectCorsairWirelessControllers() */

/*-----------------------------------------------------------------------------------------------------*\
| Keyboards                                                                                             |
\*-----------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Corsair K57 RGB (Wired)",         DetectCorsairWirelessControllers,   CORSAIR_VID,    CORSAIR_K57_RGB_WIRED_PID,     1,  0xFF42, 1);
//REGISTER_HID_DETECTOR_IPU("Corsair K57 RGB (Wireless)",      DetectCorsairWirelessControllers,   CORSAIR_VID,    CORSAIR_K57_RGB_WIRELESS_PID,  1,  0xFF42, 1);
