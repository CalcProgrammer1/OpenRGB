/*---------------------------------------------------------*\
| ClevoLightbarControllerDetect.cpp                         |
|                                                           |
|   Detector for Clevo laptop lightbar (ITE 8291 rev 0.03)  |
|                                                           |
|   Kyle Cascade (kyle@cascade.family)          16 Jan 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "ClevoLightbarController.h"
#include "RGBController_ClevoLightbar.h"
#include "RGBController.h"
#include <hidapi.h>

/*-----------------------------------------------------*\
| ITE Tech vendor ID                                    |
\*-----------------------------------------------------*/
#define ITE_VID                         0x048D

/*-----------------------------------------------------*\
| CLEVO Lightbar product ID                            |
\*-----------------------------------------------------*/
#define CLEVO_LIGHTBAR_PID             0x7001

void DetectClevoLightbarControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        ClevoLightbarController*     controller     = new ClevoLightbarController(dev, *info);
        RGBController_ClevoLightbar* rgb_controller = new RGBController_ClevoLightbar(controller);
        rgb_controller->name                         = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_PU("CLEVO Lightbar", DetectClevoLightbarControllers, ITE_VID, CLEVO_LIGHTBAR_PID, 0xFF03, 0x02);
