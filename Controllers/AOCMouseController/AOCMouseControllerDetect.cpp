/*---------------------------------------------------------*\
| AOCMouseControllerDetect.cpp                              |
|                                                           |
|   Detector for AOC mouse                                  |
|                                                           |
|   Adam Honse (CalcProgrammer1)                20 Apr 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "AOCMouseController.h"
#include "RGBController_AOCMouse.h"

/*-----------------------------------------------------*\
| AOC Mouse IDs                                         |
\*-----------------------------------------------------*/
#define AOC_VID                                     0x3938
#define AOC_GM500_PID                               0x1179

/******************************************************************************************\
*                                                                                          *
*   DetectAOCMouseControllers                                                              *
*                                                                                          *
*       Tests the USB address to see if an AOC Mouse controller exists there.              *
*                                                                                          *
\******************************************************************************************/

void DetectAOCMouseControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AOCMouseController*     controller     = new AOCMouseController(dev, info->path, name);
        RGBController_AOCMouse* rgb_controller = new RGBController_AOCMouse(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("AOC GM500",  DetectAOCMouseControllers,  AOC_VID,    AOC_GM500_PID,  1,  0xFF19, 0xFF19);
