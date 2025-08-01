/*---------------------------------------------------------*\
| CorsairICueLinkControllerDetect.cpp                       |
|                                                           |
|   Detector for Corsair iCue Link System Hub               |
|                                                           |
|   Aiden Vigue (acvigue)                       02 Mar 2025 |
|   Adam Honse <calcprogrammer1@gmail.com>      01 Aug 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "LogManager.h"
#include "CorsairICueLinkController.h"
#include "RGBController_CorsairICueLink.h"

#define CORSAIR_VID                         0x1B1C
#define CORSAIR_ICUE_LINK_SYSTEM_HUB_PID    0x0C3F

void DetectCorsairICueLinkControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        CorsairICueLinkController*     controller     = new CorsairICueLinkController(dev, info->path, name);
        RGBController_CorsairICueLink* rgb_controller = new RGBController_CorsairICueLink(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("Corsair iCUE Link System Hub", DetectCorsairICueLinkControllers, CORSAIR_VID, CORSAIR_ICUE_LINK_SYSTEM_HUB_PID, 0x00, 0xFF42, 0x01);
