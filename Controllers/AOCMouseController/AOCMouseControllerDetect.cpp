/*---------------------------------------------------------*\
| AOCMouseControllerDetect.cpp                              |
|                                                           |
|   Detector for AOC mouse                                  |
|                                                           |
|   Adam Honse (CalcProgrammer1)                20 Apr 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "AOCMouseController.h"
#include "RGBController_AOCMouse.h"

/*---------------------------------------------------------*\
| AOC Mouse IDs                                             |
\*---------------------------------------------------------*/
#define AOC_VID                                     0x3938
#define AOC_GM500_PID                               0x1179

DetectedControllers DetectAOCMouseControllers(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        AOCMouseController*     controller     = new AOCMouseController(dev, info->path, name);
        RGBController_AOCMouse* rgb_controller = new RGBController_AOCMouse(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IPU("AOC GM500",  DetectAOCMouseControllers,  AOC_VID,    AOC_GM500_PID,  1,  0xFF19, 0xFF19);
