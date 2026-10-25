/*---------------------------------------------------------*\
| AsusCerberusKeyboardControllerDetect.cpp                  |
|                                                           |
|   Detector for ASUS Cerberus keyboard devices             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "AsusCerberusKeyboardController.h"
#include "RGBController_AsusCerberusKeyboard.h"

/*---------------------------------------------------------*\
| ASUS VID                                                  |
\*---------------------------------------------------------*/
#define ASUS_LEGACY_USB_VID                         0x195D

/*---------------------------------------------------------*\
| ASUS Cerberus Mech PID                                    |
\*---------------------------------------------------------*/
#define ASUS_CERBERUS_MECH_PID                      0x2047

DetectedControllers DetectAsusCerberusMech(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        AsusCerberusKeyboardController*     controller          = new AsusCerberusKeyboardController(dev, info->path, info->release_number, name);
        RGBController_AsusCerberusKeyboard* rgb_controller      = new RGBController_AsusCerberusKeyboard(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

/*---------------------------------------------------------*\
| ASUS Cerberus Mech Detector                               |
\*---------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("ASUS Cerberus Mech", DetectAsusCerberusMech, ASUS_LEGACY_USB_VID, ASUS_CERBERUS_MECH_PID, 1, 0xFF01, 1);
