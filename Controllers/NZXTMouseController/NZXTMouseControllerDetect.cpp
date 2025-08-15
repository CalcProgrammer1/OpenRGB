/*---------------------------------------------------------*\
| NZXTMouseControllerDetect.cpp                             |
|                                                           |
|   Detector for NZXT Mouse                                 |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      13 Dec 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "NZXTMouseController.h"
#include "RGBController_NZXTMouse.h"

/*-----------------------------------------------------*\
| NZXT USB IDs                                          |
\*-----------------------------------------------------*/
#define NZXT_VID                        0x1E71
#define NZXT_LIFT_PID                   0x2100

/******************************************************************************************\
*                                                                                          *
*   DetectNZXTMouseControllers                                                             *
*                                                                                          *
*       Detect devices supported by the NZXTMouse driver                                   *
*                                                                                          *
\******************************************************************************************/

static void DetectNZXTMouseControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        NZXTMouseController*     controller     = new NZXTMouseController(dev, info->path, name);
        RGBController_NZXTMouse* rgb_controller = new RGBController_NZXTMouse(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("NZXT Lift",  DetectNZXTMouseControllers, NZXT_VID,   NZXT_LIFT_PID, 0, 0xFFCA, 1);
