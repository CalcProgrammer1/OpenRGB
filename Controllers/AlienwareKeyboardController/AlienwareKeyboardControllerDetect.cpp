/*---------------------------------------------------------*\
| AlienwareKeyboardControllerDetect.cpp                     |
|                                                           |
|   Detector for Alienware Keyboard                         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "AlienwareAW410KController.h"
#include "AlienwareAW510KController.h"
#include "RGBController_AlienwareAW410K.h"
#include "RGBController_AlienwareAW510K.h"

/*---------------------------------------------------------*\
| Alienware vendor ID                                       |
\*---------------------------------------------------------*/
#define ALIENWARE_VID                           0x04F2

/*---------------------------------------------------------*\
| Keyboard product IDs                                      |
\*---------------------------------------------------------*/
#define ALIENWARE_AW510K_PID                    0x1830
#define ALIENWARE_AW410K_PID                    0x1968

DetectedControllers DetectAlienwareAW510KControllers(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        AlienwareAW510KController*     controller     = new AlienwareAW510KController(dev, info->path, name);
        RGBController_AlienwareAW510K* rgb_controller = new RGBController_AlienwareAW510K(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

DetectedControllers DetectAlienwareAW410KControllers(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        AlienwareAW410KController*     controller     = new AlienwareAW410KController(dev, info->path, name);
        RGBController_AlienwareAW410K* rgb_controller = new RGBController_AlienwareAW410K(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IPU("Alienware AW510K",   DetectAlienwareAW510KControllers,   ALIENWARE_VID,  ALIENWARE_AW510K_PID,   0x02,   0xFF00, 0x01);
REGISTER_HID_DETECTOR_IPU("Alienware AW410K",   DetectAlienwareAW410KControllers,   ALIENWARE_VID,  ALIENWARE_AW410K_PID,   0x02,   0xFF00, 0x01);
