/*---------------------------------------------------------*\
| AOCHeadsetControllerDetect.cpp                            |
|                                                           |
|   Detector for AOC headset                                |
|                                                           |
|   Adam Honse (CalcProgrammer1)                12 Jan 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "AOCHeadsetController.h"
#include "RGBController_AOCHeadset.h"

/*-----------------------------------------------------*\
| AOC Headset IDs                                       |
\*-----------------------------------------------------*/
#define AOC_VID                                     0x0D8C
#define AOC_GH300_PID                               0x01F3

/******************************************************************************************\
*                                                                                          *
*   DetectAOCHeadsetControllers                                                            *
*                                                                                          *
*       Tests the USB address to see if an AOC Headset controller exists there.            *
*                                                                                          *
\******************************************************************************************/

void DetectAOCHeadsetControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AOCHeadsetController*     controller     = new AOCHeadsetController(dev, info->path);
        RGBController_AOCHeadset* rgb_controller = new RGBController_AOCHeadset(controller);
        rgb_controller->name                      = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("AOC GH300", DetectAOCHeadsetControllers, AOC_VID, AOC_GH300_PID, 3, 0x000C, 0x0001);
