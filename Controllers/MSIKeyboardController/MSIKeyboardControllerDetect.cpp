/*---------------------------------------------------------*\
| MSIKeyboardControllerDetect.cpp                           |
|                                                           |
|   Detector for MSI Mystic Light MS-1565 Keyboard          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "MSIMysticLightKBController.h"
#include "RGBController_MSIMysticLightKB.h"

#define MSI_USB_VID         0x1462

DetectedControllers DetectMSIKeyboardController(hid_device_info* info, const std::string&  /*name*/)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        MSIKeyboardController*     controller     = new MSIKeyboardController(dev, info->path);
        RGBController_MSIKeyboard* rgb_controller = new RGBController_MSIKeyboard(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_PU("MSI Keyboard MS_1565", DetectMSIKeyboardController, MSI_USB_VID, 0x1601, 0x00FF, 0x01);
