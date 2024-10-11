/*---------------------------------------------------------*\
| HPOmen30LControllerDetect.cpp                             |
|                                                           |
|   Detector for HP Omen 30L                                |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "HPOmen30LController.h"
#include "RGBController_HPOmen30L.h"

#define HP_OMEN_30L_VID 0x103C
#define HP_OMEN_30L_PID 0x84FD

/******************************************************************************************\
*                                                                                          *
*   DetectHPOmen30LController                                                              *
*                                                                                          *
*       Tests the USB address to see if an HP Omen 30L controller exists there.            *
*                                                                                          *
\******************************************************************************************/

void DetectHPOmen30LController(hid_device_info* info, const std::string&)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        HPOmen30LController*     controller     = new HPOmen30LController(dev, info->path);
        RGBController_HPOmen30L* rgb_controller = new RGBController_HPOmen30L(controller);
        // Constructor sets the name
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR("HP Omen 30L", DetectHPOmen30LController, HP_OMEN_30L_VID, HP_OMEN_30L_PID);
