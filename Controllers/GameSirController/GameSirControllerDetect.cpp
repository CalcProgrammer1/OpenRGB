/*---------------------------------------------------------*\
| GameSirControllerDetect.cpp                               |
|                                                           |
|   GameSir RGB Device                                      |
|                                                           |
|   Added by OpenRGB Community                  08 Aug 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "RGBController_GameSir.h"

#define GAMESIR_VID 0x3537
#define GAMESIR_NOVA_LITE_2_PID 0x100F

DetectedControllers DetectGameSirControllers(hid_device_info* info, const std::string& /*name*/)
{
    DetectedControllers detected_controllers;

    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        GameSirController* controller     = new GameSirController(dev, info->path);
        RGBController_GameSir* rgb_controller = new RGBController_GameSir(controller);
        detected_controllers.push_back(rgb_controller);
    }

    return detected_controllers;
}

REGISTER_HID_DETECTOR_IPU("GameSir Nova 2 Lite", DetectGameSirControllers, GAMESIR_VID, GAMESIR_NOVA_LITE_2_PID, 2, 0xFF7A, 0x0001);
