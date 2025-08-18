/*---------------------------------------------------------*\
| XPGSummonerControllerDetect.cpp                           |
|                                                           |
|   Detector for XPG Summoner keyboard                      |
|                                                           |
|   Erick Granados (eriosgamer)                             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "XPGSummonerController.h"
#include "RGBController_XPGSummoner.h"
#include <hidapi.h>

/******************************************************************************************\
*                                                                                          *
*   DetectXPGSummonerControllers                                                           *
*                                                                                          *
*       Tests the USB address to see if a XPG Summoner Keyboard controller exists there.   *
*                                                                                          *
\******************************************************************************************/

void DetectXPGSummonerControllers(hid_device_info *info, const std::string &name)
{
    hid_device *dev = hid_open_path(info->path);

    if(dev)
    {
        XPGSummonerController *controller = new XPGSummonerController(dev, info->path, info->product_id, name);
        RGBController_XPGSummoner *rgb_controller = new RGBController_XPGSummoner(controller);
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
} /* DetectXPGSummonerControllers() */

REGISTER_HID_DETECTOR_IPU("XPG Summoner Gaming Keyboard", DetectXPGSummonerControllers, XPG_VID, XPG_SUMMONER_PID, 2, 0xFF01, 0x0001);
