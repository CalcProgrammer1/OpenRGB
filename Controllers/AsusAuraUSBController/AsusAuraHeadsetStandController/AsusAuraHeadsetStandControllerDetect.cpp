/*---------------------------------------------------------*\
| AsusAuraHeadsetStandControllerDetect.cpp                  |
|                                                           |
|   Detector for ASUS Aura headset stand devices            |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "AsusAuraHeadsetStandController.h"
#include "RGBController_AsusAuraHeadsetStand.h"

/*---------------------------------------------------------*\
| ASUS VID                                                  |
\*---------------------------------------------------------*/
#define AURA_USB_VID                                0x0B05

/*---------------------------------------------------------*\
| Headset Stand PIDs                                        |
\*---------------------------------------------------------*/
#define AURA_ROG_THRONE_PID                         0x18D9
#define AURA_ROG_THRONE_QI_PID                      0x18C5
#define AURA_ROG_THRONE_QI_GUNDAM_PID               0x1994

DetectedControllers DetectAsusAuraUSBHeadsetStand(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        AuraHeadsetStandController*     controller          = new AuraHeadsetStandController(dev, info->path, name);
        RGBController_AuraHeadsetStand* rgb_controller      = new RGBController_AuraHeadsetStand(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

/*---------------------------------------------------------*\
| Headset Stand Detectors                                   |
\*---------------------------------------------------------*/
REGISTER_HID_DETECTOR_I("ASUS ROG Throne",              DetectAsusAuraUSBHeadsetStand,  AURA_USB_VID, AURA_ROG_THRONE_PID,              0);
REGISTER_HID_DETECTOR_I("ASUS ROG Throne QI",           DetectAsusAuraUSBHeadsetStand,  AURA_USB_VID, AURA_ROG_THRONE_QI_PID,           0);
REGISTER_HID_DETECTOR_I("ASUS ROG Throne QI GUNDAM",    DetectAsusAuraUSBHeadsetStand,  AURA_USB_VID, AURA_ROG_THRONE_QI_GUNDAM_PID,    0);
