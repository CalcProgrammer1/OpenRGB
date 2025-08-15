/*---------------------------------------------------------*\
| PatriotViperMouseControllerDetect.cpp                     |
|                                                           |
|   Detector for Patriot Viper Mouse                        |
|                                                           |
|   mi4code                                     07 Apr 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "PatriotViperMouseController.h"
#include "RGBController_PatriotViperMouse.h"


/*-----------------------------------------------------*\
| Patriot Viper Mouse IDs                               |
\*-----------------------------------------------------*/

#define PATRIOT_VID    0x0C45
#define VIPER_V550_PID 0x7E18


void DetectPatriotViperMouseControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        PatriotViperMouseController*     controller     = new PatriotViperMouseController(dev, info->path, name);
        RGBController_PatriotViperMouse* rgb_controller = new RGBController_PatriotViperMouse(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("Patriot Viper V550",  DetectPatriotViperMouseControllers,  PATRIOT_VID,  VIPER_V550_PID, 2, 0xFF18, 0x01);
