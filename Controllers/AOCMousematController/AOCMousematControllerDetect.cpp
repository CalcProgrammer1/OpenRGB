/*---------------------------------------------------------*\
| AOCMousematControllerDetect.cpp                           |
|                                                           |
|   Detector for AOC mousemat                               |
|                                                           |
|   Adam Honse (CalcProgrammer1)                15 Apr 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "AOCMousematController.h"
#include "RGBController_AOCMousemat.h"

/*---------------------------------------------------------*\
| AOC Mousemat IDs                                          |
\*---------------------------------------------------------*/
#define AOC_VID                                     0x3938
#define AOC_AMM700_PID                              0x1162

DetectedControllers DetectAOCMousematControllers(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        AOCMousematController*     controller     = new AOCMousematController(dev, info->path, name);
        RGBController_AOCMousemat* rgb_controller = new RGBController_AOCMousemat(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IPU("AOC AGON AMM700",    DetectAOCMousematControllers,   AOC_VID,    AOC_AMM700_PID, 1,  0xFF19,  0xFF19);
