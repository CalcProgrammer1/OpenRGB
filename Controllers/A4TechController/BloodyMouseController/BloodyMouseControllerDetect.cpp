/*---------------------------------------------------------*\
| BloodyMouseControllerDetect.cpp                           |
|                                                           |
|   Detector for Bloody Mouse Devices                       |
|                                                           |
|   Chris M (Dr_No)                             30 Jun 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "BloodyMouseController.h"
#include "DetectionManager.h"
#include "RGBController_BloodyMouse.h"

/*---------------------------------------------------------*\
| A4 Tech USB vendor ID                                     |
\*---------------------------------------------------------*/
#define A4_TECH_VID                                 0x09DA

DetectedControllers DetectBloodyMouseControllers(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        BloodyMouseController*     controller       = new BloodyMouseController(dev, info->path, info->product_id, name);
        RGBController_BloodyMouse* rgb_controller   = new RGBController_BloodyMouse(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IPU("Bloody W60 Pro",  DetectBloodyMouseControllers,  A4_TECH_VID,    BLOODY_W60_PRO_PID, 2,  0xFF33, 0x0529);
REGISTER_HID_DETECTOR_IPU("Bloody W70 Max",  DetectBloodyMouseControllers,  A4_TECH_VID,    BLOODY_W70_MAX_PID, 2,  0xFF33, 0x0518);
REGISTER_HID_DETECTOR_IPU("Bloody W90 Max",  DetectBloodyMouseControllers,  A4_TECH_VID,    BLOODY_W90_MAX_PID, 2,  0xFF33, 0x053D);
REGISTER_HID_DETECTOR_IPU("Bloody W90 Pro",  DetectBloodyMouseControllers,  A4_TECH_VID,    BLOODY_W90_PRO_PID, 2,  0xFF33, 0x054D);
REGISTER_HID_DETECTOR_IPU("Bloody MP 50RS",  DetectBloodyMouseControllers,  A4_TECH_VID,    BLOODY_MP_50RS_PID, 2,  0xFFF2, 0x6009);
