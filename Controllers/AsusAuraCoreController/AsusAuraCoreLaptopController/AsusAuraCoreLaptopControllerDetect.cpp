/*---------------------------------------------------------*\
| AsusAuraCoreLaptopControllerDetect.cpp                    |
|                                                           |
|   Detector for ASUS ROG Aura Core Laptop                  |
|                                                           |
|   Chris M (Dr_No)                             28 Jul 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "AsusAuraCoreLaptopController.h"
#include "DetectionManager.h"
#include "RGBController_AsusAuraCoreLaptop.h"

#define AURA_CORE_VID                   0x0B05

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

REGISTER_HID_DETECTOR_PU("ASUS ROG Strix SCAR 15",      DetectAsusAuraCoreLaptopControllers,    AURA_CORE_VID, AURA_STRIX_SCAR_15_PID,  0xFF31, 0x79);
REGISTER_HID_DETECTOR_PU("ASUS ROG Strix SCAR 17",      DetectAsusAuraCoreLaptopControllers,    AURA_CORE_VID, 0x1866,                  0xFF31, 0x79);
