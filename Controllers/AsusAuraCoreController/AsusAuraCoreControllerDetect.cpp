/*---------------------------------------------------------*\
| AsusAuraCoreControllerDetect.cpp                          |
|                                                           |
|   Detector for ASUS ROG Aura Core                         |
|                                                           |
|   Adam Honse (CalcProgrammer1)                13 Apr 2020 |
|   Chris M (Dr_No)                             28 Jul 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "AsusAuraCoreController.h"
#include "RGBController.h"
#include "RGBController_AsusAuraCore.h"
#include "RGBController_AsusAuraCoreLaptop.h"
#include <hidapi.h>

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

DetectedControllers DetectAsusAuraCoreLaptopControllers(hid_device_info* info, const std::string& /*name*/)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        AsusAuraCoreLaptopController*     controller        = new AsusAuraCoreLaptopController(dev, info->path);
        RGBController_AsusAuraCoreLaptop* rgb_controller    = new RGBController_AsusAuraCoreLaptop(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}


REGISTER_HID_DETECTOR   ("ASUS Aura Core",              DetectAsusAuraCoreControllers,          AURA_CORE_VID, 0x1854);
REGISTER_HID_DETECTOR   ("ASUS Aura Core",              DetectAsusAuraCoreControllers,          AURA_CORE_VID, 0x1866);
REGISTER_HID_DETECTOR   ("ASUS Aura Core",              DetectAsusAuraCoreControllers,          AURA_CORE_VID, 0x1869);
REGISTER_HID_DETECTOR_PU("ASUS ROG Strix SCAR 15",      DetectAsusAuraCoreLaptopControllers,    AURA_CORE_VID, AURA_STRIX_SCAR_15_PID,  0xFF31, 0x79);
REGISTER_HID_DETECTOR_PU("ASUS ROG Strix SCAR 17",      DetectAsusAuraCoreLaptopControllers,    AURA_CORE_VID, 0x1866,                  0xFF31, 0x79);
