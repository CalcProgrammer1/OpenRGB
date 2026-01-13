/*---------------------------------------------------------*\
| MountainKeyboardControllerDetect.cpp                      |
|                                                           |
|   Detector for Mountain keyboard                          |
|                                                           |
|   Wojciech Lazarski / O'D.Sæzl                   Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "Mountain60KeyboardController.h"
#include "MountainKeyboardController.h"
#include "RGBController_Mountain60Keyboard.h"
#include "RGBController_MountainKeyboard.h"

/*---------------------------------------------------------*\
| Mountain vendor ID                                        |
\*---------------------------------------------------------*/
#define MOUNTAIN_VID                                0x3282

/*---------------------------------------------------------*\
| Everest 60 keyboard Connection IDs                        |
\*---------------------------------------------------------*/
#define MOUNTAIN60_EVEREST_60_PID_ANSII             0x0005
#define MOUNTAIN60_EVEREST_60_PID_ISO               0x0006
#define MOUNTAIN60_EVEREST_60_INTERFACE             2
#define MOUNTAIN60_EVEREST_60_U                     0x01
#define MOUNTAIN60_EVEREST_60_UP                    0xFFFF

DetectedControllers DetectMountain60KeyboardControllers(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        Mountain60KeyboardController*     controller     = new Mountain60KeyboardController(dev, info->path, name);
        RGBController_Mountain60Keyboard* rgb_controller = new RGBController_Mountain60Keyboard(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

DetectedControllers DetectMountainKeyboardControllers(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        MountainKeyboardController*     controller     = new MountainKeyboardController(dev, info->path, name);
        RGBController_MountainKeyboard* rgb_controller = new RGBController_MountainKeyboard(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IPU("Mountain Everest", DetectMountainKeyboardControllers, MOUNTAIN_VID, MOUNTAIN_EVEREST_PID, 3, 0xFF00, 0x01);
REGISTER_HID_DETECTOR_IPU("Mountain Everest 60", DetectMountain60KeyboardControllers, MOUNTAIN_VID, MOUNTAIN60_EVEREST_60_PID_ANSII, MOUNTAIN60_EVEREST_60_INTERFACE, MOUNTAIN60_EVEREST_60_UP, MOUNTAIN60_EVEREST_60_U);
REGISTER_HID_DETECTOR_IPU("Mountain Everest 60", DetectMountain60KeyboardControllers, MOUNTAIN_VID, MOUNTAIN60_EVEREST_60_PID_ISO, MOUNTAIN60_EVEREST_60_INTERFACE, MOUNTAIN60_EVEREST_60_UP, MOUNTAIN60_EVEREST_60_U);
