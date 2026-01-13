/*---------------------------------------------------------*\
| IonicoControllerDetect.cpp                                |
|                                                           |
|   Detector for Ionico-II-17                               |
|                                                           |
|   Lucas Strafe                                31 Dec 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "RGBController.h"
#include "hidapi.h"
#include "IonicoController.h"
#include "RGBController_Ionico.h"

/*---------------------------------------------------------*\
|                       FRONT BAR                           |
\*---------------------------------------------------------*/
#define IONICO_FB_VID        0x048D
#define IONICO_FB_PID        0x6005

/*---------------------------------------------------------*\
|                       KEYBOARD                            |
\*---------------------------------------------------------*/
#define IONICO_KB_VID        0x048D
#define IONICO_KB_PID        0xCE00

DetectedControllers DetectIonicoControllers(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        IonicoController*     controller         = new IonicoController(dev, *info, info->product_id, name);
        RGBController_Ionico* rgb_controller     = new RGBController_Ionico(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_PU("Ionico Light Bar", DetectIonicoControllers, IONICO_FB_VID, IONICO_FB_PID,  0xFF03, 0x01);
REGISTER_HID_DETECTOR_PU("Ionico Keyboard", DetectIonicoControllers, IONICO_KB_VID, IONICO_KB_PID,  0xFF12, 0x01);
