/*---------------------------------------------------------*\
| GaiZhongGaiControllerDetect.cpp                           |
|                                                           |
|   Detector for GaiZhongGai keyboard                       |
|                                                           |
|   An Yang                                     24 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "GaiZhongGaiController.h"
#include "RGBController_GaiZhongGai.h"

/******************************************************************************************\
*                                                                                          *
*   DetectGaiZhongGaiKeyboardControllers                                                   *
*                                                                                          *
*       Tests the USB address to see if a GaiZhongGai RGB Keyboard controller exists there.*
*                                                                                          *
\******************************************************************************************/

void DetectGaiZhongGaiKeyboardControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        GaiZhongGaiKeyboardController* controller           = new GaiZhongGaiKeyboardController(dev, info, name);
        RGBController_GaiZhongGaiKeyboard* rgb_controller   = new RGBController_GaiZhongGaiKeyboard(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectGaiZhongGaiKeyboardControllers() */

REGISTER_HID_DETECTOR_I("GaiZhongGai 68+4 PRO",         DetectGaiZhongGaiKeyboardControllers, GAIZHONGGAI_VID, GAIZHONGGAI_68_PRO_PID,          3);
REGISTER_HID_DETECTOR_I("GaiZhongGai 42 PRO",           DetectGaiZhongGaiKeyboardControllers, GAIZHONGGAI_VID, GAIZHONGGAI_42_PRO_PID,          3);
REGISTER_HID_DETECTOR_I("GaiZhongGai Dial",             DetectGaiZhongGaiKeyboardControllers, GAIZHONGGAI_VID, GAIZHONGGAI_DIAL_PID,            3);
REGISTER_HID_DETECTOR_I("GaiZhongGai LightBoard",       DetectGaiZhongGaiKeyboardControllers, GAIZHONGGAI_VID, GAIZHONGGAI_LIGHT_BOARD_PID,     3);
REGISTER_HID_DETECTOR_I("GaiZhongGai RGB HUB Green",    DetectGaiZhongGaiKeyboardControllers, GAIZHONGGAI_VID, GAIZHONGGAI_RGB_HUB_GREEN_PID,   3);
REGISTER_HID_DETECTOR_I("GaiZhongGai RGB HUB Blue",     DetectGaiZhongGaiKeyboardControllers, GAIZHONGGAI_VID, GAIZHONGGAI_RGB_HUB_BLUE_PID,    3);
REGISTER_HID_DETECTOR_I("GaiZhongGai 17+4+Touch PRO",   DetectGaiZhongGaiKeyboardControllers, GAIZHONGGAI_VID, GAIZHONGGAI_17_TOUCH_PRO_PID,    3);
REGISTER_HID_DETECTOR_I("GaiZhongGai 17 PRO",           DetectGaiZhongGaiKeyboardControllers, GAIZHONGGAI_VID, GAIZHONGGAI_17_PRO_PID,          3);
REGISTER_HID_DETECTOR_I("GaiZhongGai 20 PRO",           DetectGaiZhongGaiKeyboardControllers, GAIZHONGGAI_VID, GAIZHONGGAI_20_PRO_PID,          3);
