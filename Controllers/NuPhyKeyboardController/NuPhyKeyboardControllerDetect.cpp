/*---------------------------------------------------------*\
| NuPhyKeyboardControllerDetect.cpp                         |
|                                                           |
|   Detector for NuPhy QMK/VIA RGB Matrix keyboards         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "NuPhyKeyboardController.h"
#include "RGBController_NuPhyKeyboard.h"

#define NUPHY_VID                 0x19F5
#define NUPHY_AIR75_V2_PID        0x3246
#define NUPHY_AIR60_V2_PID        0x3255
#define NUPHY_AIR96_V2_PID        0x3266
#define QMK_VIA_USAGE_PAGE        0xFF60
#define QMK_VIA_USAGE             0x0061

void DetectNuPhyKeyboardControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        NuPhyKeyboardController* controller = new NuPhyKeyboardController(dev, info->path, name);

        if(controller->GetSupported())
        {
            RGBController_NuPhyKeyboard* rgb_controller = new RGBController_NuPhyKeyboard(controller);
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
        else
        {
            delete controller;
        }
    }
}

REGISTER_HID_DETECTOR_PU("NuPhy Air60 V2", DetectNuPhyKeyboardControllers, NUPHY_VID, NUPHY_AIR60_V2_PID, QMK_VIA_USAGE_PAGE, QMK_VIA_USAGE);
REGISTER_HID_DETECTOR_PU("NuPhy Air75 V2", DetectNuPhyKeyboardControllers, NUPHY_VID, NUPHY_AIR75_V2_PID, QMK_VIA_USAGE_PAGE, QMK_VIA_USAGE);
REGISTER_HID_DETECTOR_PU("NuPhy Air96 V2", DetectNuPhyKeyboardControllers, NUPHY_VID, NUPHY_AIR96_V2_PID, QMK_VIA_USAGE_PAGE, QMK_VIA_USAGE);
