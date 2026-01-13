/*---------------------------------------------------------*\
| XPGSummonerControllerDetect.cpp                           |
|                                                           |
|   Detector for XPG Summoner keyboard                      |
|                                                           |
|   Erick Granados (eriosgamer)                             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "RGBController_XPGSummoner.h"
#include "XPGSummonerController.h"

DetectedControllers DetectXPGSummonerControllers(hid_device_info *info, const std::string &name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        XPGSummonerController*     controller     = new XPGSummonerController(dev, info->path, info->product_id, name);
        RGBController_XPGSummoner* rgb_controller = new RGBController_XPGSummoner(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IPU("XPG Summoner Gaming Keyboard", DetectXPGSummonerControllers, XPG_VID, XPG_SUMMONER_PID, 2, 0xFF01, 0x0001);
