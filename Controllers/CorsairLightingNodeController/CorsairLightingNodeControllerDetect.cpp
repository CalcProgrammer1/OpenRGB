/*---------------------------------------------------------*\
| CorsairLightingNodeControllerDetect.cpp                   |
|                                                           |
|   Detector for Corsair Lighting Node devices              |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      12 Jan 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "CorsairLightingNodeController.h"
#include "RGBController_CorsairLightingNode.h"

#define CORSAIR_VID                     0x1B1C
#define CORSAIR_LIGHTING_NODE_CORE_PID  0x0C1A
#define CORSAIR_LIGHTING_NODE_PRO_PID   0x0C0B
#define CORSAIR_COMMANDER_PRO_PID       0x0C10
#define CORSAIR_LS100_PID               0x0C1E
#define CORSAIR_1000D_OBSIDIAN_PID      0x1D00
#define CORSAIR_SPEC_OMEGA_RGB_PID      0x1D04
#define CORSAIR_LT100_PID               0x0C23

/******************************************************************************************\
*                                                                                          *
*   DetectCorsairLightingNodeControllers                                                   *
*                                                                                          *
*       Detect devices supported by the Corsair Lighting Node Pro driver                   *
*                                                                                          *
\******************************************************************************************/

void DetectCorsairLightingNodeControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        CorsairLightingNodeController*     controller     = new CorsairLightingNodeController(dev, info->path, name);
        RGBController_CorsairLightingNode* rgb_controller = new RGBController_CorsairLightingNode(controller);

        DetectionManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectCorsairLightingNodeControllers() */

REGISTER_HID_DETECTOR("Corsair Lighting Node Core", DetectCorsairLightingNodeControllers, CORSAIR_VID, CORSAIR_LIGHTING_NODE_CORE_PID); // 1 channel
REGISTER_HID_DETECTOR("Corsair Lighting Node Pro",  DetectCorsairLightingNodeControllers, CORSAIR_VID, CORSAIR_LIGHTING_NODE_PRO_PID);  // 2 channels
REGISTER_HID_DETECTOR("Corsair Commander Pro",      DetectCorsairLightingNodeControllers, CORSAIR_VID, CORSAIR_COMMANDER_PRO_PID);      // 2 channels
REGISTER_HID_DETECTOR("Corsair LS100 Lighting Kit", DetectCorsairLightingNodeControllers, CORSAIR_VID, CORSAIR_LS100_PID);              // 1 channel
REGISTER_HID_DETECTOR("Corsair 1000D Obsidian",     DetectCorsairLightingNodeControllers, CORSAIR_VID, CORSAIR_1000D_OBSIDIAN_PID);     // 2 channels
REGISTER_HID_DETECTOR("Corsair SPEC OMEGA RGB",     DetectCorsairLightingNodeControllers, CORSAIR_VID, CORSAIR_SPEC_OMEGA_RGB_PID);     // 2 channels
REGISTER_HID_DETECTOR("Corsair LT100",              DetectCorsairLightingNodeControllers, CORSAIR_VID, CORSAIR_LT100_PID);              // 2 channels
