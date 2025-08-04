/*---------------------------------------------------------*\
| AOCKeyboardControllerDetect.cpp                           |
|                                                           |
|   Detector for AOC keyboard                               |
|                                                           |
|   Adam Honse (CalcProgrammer1)                10 May 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "AOCKeyboardController.h"
#include "RGBController_AOCKeyboard.h"

/*-----------------------------------------------------*\
| AOC Keyboard IDs                                      |
\*-----------------------------------------------------*/
#define AOC_VID                                     0x3938
#define AOC_GK500_PID                               0x1178
#define AOC_GK500_PID_2                             0x1229

/******************************************************************************************\
*                                                                                          *
*   DetectAOCKeyboardControllers                                                           *
*                                                                                          *
*       Tests the USB address to see if an AOC Keyboard controller exists there.           *
*                                                                                          *
\******************************************************************************************/

void DetectAOCKeyboardControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AOCKeyboardController*     controller     = new AOCKeyboardController(dev, info->path, name);
        RGBController_AOCKeyboard* rgb_controller = new RGBController_AOCKeyboard(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_PU("AOC GK500",  DetectAOCKeyboardControllers,   AOC_VID,    AOC_GK500_PID,   0xFF19, 0xFF19);
REGISTER_HID_DETECTOR_PU("AOC GK500",  DetectAOCKeyboardControllers,   AOC_VID,    AOC_GK500_PID_2, 0xFF19, 0xFF19);
