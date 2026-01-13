/*---------------------------------------------------------*\
| AOCKeyboardControllerDetect.cpp                           |
|                                                           |
|   Detector for AOC keyboard                               |
|                                                           |
|   Adam Honse (CalcProgrammer1)                10 May 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "AOCKeyboardController.h"
#include "RGBController_AOCKeyboard.h"

/*---------------------------------------------------------*\
| AOC Keyboard IDs                                          |
\*---------------------------------------------------------*/
#define AOC_VID                                     0x3938
#define AOC_GK500_PID                               0x1178
#define AOC_GK500_PID_2                             0x1229

DetectedControllers DetectAOCKeyboardControllers(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        AOCKeyboardController*     controller     = new AOCKeyboardController(dev, info->path, name);
        RGBController_AOCKeyboard* rgb_controller = new RGBController_AOCKeyboard(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_PU("AOC GK500",  DetectAOCKeyboardControllers,   AOC_VID,    AOC_GK500_PID,   0xFF19, 0xFF19);
REGISTER_HID_DETECTOR_PU("AOC GK500",  DetectAOCKeyboardControllers,   AOC_VID,    AOC_GK500_PID_2, 0xFF19, 0xFF19);
