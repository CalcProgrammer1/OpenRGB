/*---------------------------------------------------------*\
| OKSKeyboardControllerDetect.cpp                           |
|                                                           |
|   Detector for OKS keyboard                               |
|                                                           |
|   Merafour (OKS)                              24 Feb 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "OKSKeyboardController.h"
#include "RGBController_OKSKeyboard.h"

/******************************************************************************************\
*  DetectOKSKeyboardControllers                                                            *
*  Reference: DuckyKeyboardController                                                      *
*  Tests the USB address to see if a OKS Optical Axis RGB Keyboard controller exists there.*
*  Reference:DetectDuckyKeyboardControllers                                                *
\******************************************************************************************/

void DetectOKSKeyboardControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        OKSKeyboardController*     controller     = new OKSKeyboardController(dev, info->path, info->product_id, name);
        RGBController_OKSKeyboard* rgb_controller = new RGBController_OKSKeyboard(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectOKSKeyboardControllers() */

REGISTER_HID_DETECTOR_I("OKS Optical Axis RGB",           DetectOKSKeyboardControllers, OKS_VID, OKS_OPTICAL_RGB_PID,   1);
