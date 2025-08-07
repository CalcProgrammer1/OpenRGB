/*---------------------------------------------------------*\
| DuckyKeyboardControllerDetect.cpp                         |
|                                                           |
|   Detector for Ducky keyboard                             |
|                                                           |
|   Adam Honse (CalcProgrammer1)                04 Jul 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "DuckyKeyboardController.h"
#include "RGBController_DuckyKeyboard.h"
#include <hidapi.h>

/******************************************************************************************\
*                                                                                          *
*   DetectDuckyKeyboardControllers                                                         *
*                                                                                          *
*       Tests the USB address to see if a Ducky RGB Keyboard controller exists there.      *
*                                                                                          *
\******************************************************************************************/

void DetectDuckyKeyboardControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        DuckyKeyboardController*     controller     = new DuckyKeyboardController(dev, info->path, info->product_id, name);
        RGBController_DuckyKeyboard* rgb_controller = new RGBController_DuckyKeyboard(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectDuckyKeyboardControllers() */

REGISTER_HID_DETECTOR_I("Ducky Shine 7/Ducky One 2 RGB", DetectDuckyKeyboardControllers, DUCKY_VID, DUCKY_SHINE_7_ONE_2_RGB_PID, 1);
REGISTER_HID_DETECTOR_I("Ducky One 2 RGB TKL",           DetectDuckyKeyboardControllers, DUCKY_VID, DUCKY_ONE_2_RGB_TKL_PID,     1);
