/*---------------------------------------------------------*\
| AsusROGAllyControllerDetect.cpp                           |
|                                                           |
|   Detector for ASUS ROG Ally devices                      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "AsusROGAllyController.h"
#include "RGBController_AsusROGAlly.h"

/*---------------------------------------------------------*\
| ASUS VID                                                  |
\*---------------------------------------------------------*/
#define AURA_USB_VID                                0x0B05

/*---------------------------------------------------------*\
| ROG Ally PIDs                                             |
\*---------------------------------------------------------*/
#define ASUS_ROG_ALLY_PID                           0x1ABE
#define ASUS_ROG_ALLY_X_PID                         0x1B4C

DetectedControllers DetectAsusROGAlly(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        ROGAllyController*         controller           = new ROGAllyController(dev, info->path, name);
        RGBController_AsusROGAlly* rgb_controller       = new RGBController_AsusROGAlly(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

/*---------------------------------------------------------*\
| ROG Ally Detectors                                        |
\*---------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("ASUS ROG Ally",      DetectAsusROGAlly,  AURA_USB_VID, ASUS_ROG_ALLY_PID,    2,  0xFF31, 0x0076);
REGISTER_HID_DETECTOR_IPU("ASUS ROG Ally X",    DetectAsusROGAlly,  AURA_USB_VID, ASUS_ROG_ALLY_X_PID,  2,  0xFF31, 0x0076);
