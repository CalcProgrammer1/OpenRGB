/*---------------------------------------------------------*\
| AsusAuraCoreControllerDetect.cpp                          |
|                                                           |
|   Detector for ASUS ROG Aura Core                         |
|                                                           |
|   Adam Honse (CalcProgrammer1)                13 Apr 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "AsusAuraCoreController.h"
#include "DetectionManager.h"
#include "RGBController_AsusAuraCore.h"

#define AURA_CORE_VID                   0x0B05

DetectedControllers DetectAsusAuraCoreControllers(hid_device_info* info, const std::string& /*name*/)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        AuraCoreController*     controller                  = new AuraCoreController(dev, info->path);
        RGBController_AuraCore* rgb_controller              = new RGBController_AuraCore(controller);

        if(rgb_controller->GetDeviceType() != DEVICE_TYPE_UNKNOWN)
        {
            detected_controllers.push_back(rgb_controller);
        }
        else
        {
            delete rgb_controller;
        }
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR   ("ASUS Aura Core",              DetectAsusAuraCoreControllers,          AURA_CORE_VID, 0x1854);
REGISTER_HID_DETECTOR   ("ASUS Aura Core",              DetectAsusAuraCoreControllers,          AURA_CORE_VID, 0x1866);
REGISTER_HID_DETECTOR   ("ASUS Aura Core",              DetectAsusAuraCoreControllers,          AURA_CORE_VID, 0x1869);
