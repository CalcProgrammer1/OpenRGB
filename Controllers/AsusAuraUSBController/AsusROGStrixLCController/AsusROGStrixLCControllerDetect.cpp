/*---------------------------------------------------------*\
| AsusROGStrixLCControllerDetect.cpp                        |
|                                                           |
|   Detector for ASUS ROG Strix LC devices                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "AsusROGStrixLCController.h"
#include "RGBController_AsusROGStrixLC.h"

/*---------------------------------------------------------*\
| ASUS VID                                                  |
\*---------------------------------------------------------*/
#define AURA_USB_VID                                0x0B05

/*---------------------------------------------------------*\
| ROG Strix LC PIDs                                         |
\*---------------------------------------------------------*/
#define ROG_STRIX_LC120_PID                         0x879E

DetectedControllers DetectAsusAuraUSBROGStrixLC(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        AsusROGStrixLCController*     controller     = new AsusROGStrixLCController(dev, info->path, name);
        RGBController_AsusROGStrixLC* rgb_controller = new RGBController_AsusROGStrixLC(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

/*---------------------------------------------------------*\
| ROG Strix LC Detectors                                    |
\*---------------------------------------------------------*/
REGISTER_HID_DETECTOR_PU("ASUS ROG Strix LC",   DetectAsusAuraUSBROGStrixLC,    AURA_USB_VID, ROG_STRIX_LC120_PID,  0x00FF, 1);
