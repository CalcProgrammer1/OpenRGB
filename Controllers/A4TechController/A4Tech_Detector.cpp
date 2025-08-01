/*---------------------------------------------------------*\
| A4TechDetector.cpp                                        |
|                                                           |
|   Detector for A4Tech Devices                             |
|                                                           |
|   Chris M (Dr_No)                             30 Jun 2022 |
|   Mohammed Julfikar Ali Mahbub (o-julfikar)   01 Apr 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

/*-----------------------------------------------------*\
| OpenRGB includes                                      |
\*-----------------------------------------------------*/
#include <hidapi.h>
#include "Detector.h"

/*-----------------------------------------------------*\
| A4 Tech specific includes                             |
\*-----------------------------------------------------*/
#include "RGBController_BloodyMouse.h"
#include "RGBController_BloodyB820R.h"

/*-----------------------------------------------------*\
| A4 Tech USB vendor ID                                 |
\*-----------------------------------------------------*/
#define A4_TECH_VID                                 0x09DA

void DetectA4TechMouseControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        BloodyMouseController* controller           = new BloodyMouseController(dev, info->path, info->product_id, name);
        RGBController_BloodyMouse* rgb_controller   = new RGBController_BloodyMouse(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectBloodyB820R(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        BloodyB820RController*     controller     = new BloodyB820RController(dev, info->path, name);
        RGBController_BloodyB820R* rgb_controller = new RGBController_BloodyB820R(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("Bloody W60 Pro",  DetectA4TechMouseControllers,       A4_TECH_VID,    BLOODY_W60_PRO_PID,     2,      0xFF33,     0x0529);
REGISTER_HID_DETECTOR_IPU("Bloody W70 Max",  DetectA4TechMouseControllers,       A4_TECH_VID,    BLOODY_W70_MAX_PID,     2,      0xFF33,     0x0518);
REGISTER_HID_DETECTOR_IPU("Bloody W90 Max",  DetectA4TechMouseControllers,       A4_TECH_VID,    BLOODY_W90_MAX_PID,     2,      0xFF33,     0x053D);
REGISTER_HID_DETECTOR_IPU("Bloody W90 Pro",  DetectA4TechMouseControllers,       A4_TECH_VID,    BLOODY_W90_PRO_PID,     2,      0xFF33,     0x054D);
REGISTER_HID_DETECTOR_IPU("Bloody MP 50RS",  DetectA4TechMouseControllers,       A4_TECH_VID,    BLOODY_MP_50RS_PID,     2,      0xFFF2,     0x6009);
REGISTER_HID_DETECTOR_IPU("Bloody B820R",    DetectBloodyB820R,                  A4_TECH_VID,    BLOODY_B820R_PID,       2,      0xFF52,     0x0210);
