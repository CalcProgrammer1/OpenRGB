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

/******************************************************************************************\
*                                                                                          *
*   DetectAuraCoreControllers                                                              *
*                                                                                          *
*       Tests the USB address to see if an Asus ROG Aura Core controller exists there      *
*                                                                                          *
\******************************************************************************************/

void DetectAsusAuraCoreControllers(hid_device_info* info, const std::string& /*name*/)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AuraCoreController*     controller                  = new AuraCoreController(dev, info->path);
        RGBController_AuraCore* rgb_controller              = new RGBController_AuraCore(controller);

        if(rgb_controller->GetDeviceType() != DEVICE_TYPE_UNKNOWN)
        {
            DetectionManager::get()->RegisterRGBController(rgb_controller);
        }
        else
        {
            delete rgb_controller;
        }
    }
}

void DetectAsusAuraCoreLaptopControllers(hid_device_info* info, const std::string& /*name*/)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AsusAuraCoreLaptopController*     controller        = new AsusAuraCoreLaptopController(dev, info->path);
        RGBController_AsusAuraCoreLaptop* rgb_controller    = new RGBController_AsusAuraCoreLaptop(controller);

        DetectionManager::get()->RegisterRGBController(rgb_controller);
    }
}


REGISTER_HID_DETECTOR   ("ASUS Aura Core",              DetectAsusAuraCoreControllers,          AURA_CORE_VID, 0x1854);
REGISTER_HID_DETECTOR   ("ASUS Aura Core",              DetectAsusAuraCoreControllers,          AURA_CORE_VID, 0x1866);
REGISTER_HID_DETECTOR   ("ASUS Aura Core",              DetectAsusAuraCoreControllers,          AURA_CORE_VID, 0x1869);
REGISTER_HID_DETECTOR_PU("ASUS ROG Strix SCAR 15",      DetectAsusAuraCoreLaptopControllers,    AURA_CORE_VID, AURA_STRIX_SCAR_15_PID,  0xFF31, 0x79);
REGISTER_HID_DETECTOR_PU("ASUS ROG Strix SCAR 17",      DetectAsusAuraCoreLaptopControllers,    AURA_CORE_VID, 0x1866,                  0xFF31, 0x79);
