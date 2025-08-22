/*---------------------------------------------------------*\
| CorsairCommanderCoreControllerDetect.cpp                  |
|                                                           |
|   Detector for Corsair Commander Core                     |
|                                                           |
|   Jeff P.                                                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "CorsairCommanderCoreController.h"
#include "RGBController_CorsairCommanderCore.h"

/*-----------------------------------------------------*\
| Corsair vendor ID                                     |
\*-----------------------------------------------------*/
#define CORSAIR_VID                         0x1B1C

/******************************************************************************************\
*                                                                                          *
*   DetectCorsairCommanderCoreControllers                                                  *
*                                                                                          *
*       Tests the USB address to see if a Corsair RGB Cooler controller exists there.      *
*                                                                                          *
\******************************************************************************************/

void DetectCorsairCommanderCoreControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        CorsairCommanderCoreController*     controller     = new CorsairCommanderCoreController(dev, info->path, info->product_id, name);
        RGBController_CorsairCommanderCore* rgb_controller = new RGBController_CorsairCommanderCore(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("Corsair Commander Core", DetectCorsairCommanderCoreControllers, CORSAIR_VID, CORSAIR_COMMANDER_CORE_PID,  0x00, 0xFF42, 0x01);
REGISTER_HID_DETECTOR_IPU("Corsair Commander Core", DetectCorsairCommanderCoreControllers, CORSAIR_VID, CORSAIR_COMMANDER_CORE2_PID, 0x00, 0xFF42, 0x01);
REGISTER_HID_DETECTOR_IPU("Corsair Commander Core", DetectCorsairCommanderCoreControllers, CORSAIR_VID, CORSAIR_COMMANDER_CORE3_PID, 0x00, 0xFF42, 0x01);
REGISTER_HID_DETECTOR_IPU("Corsair Commander Core", DetectCorsairCommanderCoreControllers, CORSAIR_VID, CORSAIR_COMMANDER_CORE4_PID, 0x00, 0xFF42, 0x01);
REGISTER_HID_DETECTOR_IPU("Corsair Commander Core", DetectCorsairCommanderCoreControllers, CORSAIR_VID, CORSAIR_COMMANDER_CORE5_PID, 0x00, 0xFF42, 0x01);
REGISTER_HID_DETECTOR_IPU("Corsair Commander Core", DetectCorsairCommanderCoreControllers, CORSAIR_VID, CORSAIR_COMMANDER_CORE6_PID, 0x00, 0xFF42, 0x01);
REGISTER_HID_DETECTOR_IPU("Corsair Commander Core XT", DetectCorsairCommanderCoreControllers, CORSAIR_VID, CORSAIR_COMMANDER_CORE_XT_PID, 0x00, 0xFF42, 0x01);
