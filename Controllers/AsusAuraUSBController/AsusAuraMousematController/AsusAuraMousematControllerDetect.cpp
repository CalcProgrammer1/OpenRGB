/*---------------------------------------------------------*\
| AsusAuraMousematControllerDetect.cpp                      |
|                                                           |
|   Detector for ASUS Aura Mousemat devices                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "AsusAuraMousematController.h"
#include "RGBController_AsusAuraMousemat.h"

/*---------------------------------------------------------*\
| ASUS VID                                                  |
\*---------------------------------------------------------*/
#define AURA_USB_VID                                0x0B05

/*---------------------------------------------------------*\
| MOUSEMAT PIDs                                             |
\*---------------------------------------------------------*/
#define AURA_ROG_BALTEUS_PID                        0x1891
#define AURA_ROG_BALTEUS_QI_PID                     0x1890

DetectedControllers DetectAsusAuraUSBMousemats(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        AuraMousematController*     controller              = new AuraMousematController(dev, info->path, name);
        RGBController_AuraMousemat* rgb_controller          = new RGBController_AuraMousemat(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

/*---------------------------------------------------------*\
| Mousemat Detectors                                        |
\*---------------------------------------------------------*/
REGISTER_HID_DETECTOR_PU("ASUS ROG Balteus",    DetectAsusAuraUSBMousemats, AURA_USB_VID, AURA_ROG_BALTEUS_PID,     0xFF06, 1);
REGISTER_HID_DETECTOR_PU("ASUS ROG Balteus Qi", DetectAsusAuraUSBMousemats, AURA_USB_VID, AURA_ROG_BALTEUS_QI_PID,  0xFF06, 1);
