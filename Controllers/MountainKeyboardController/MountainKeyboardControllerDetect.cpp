/*---------------------------------------------------------*\
| MountainKeyboardControllerDetect.cpp                      |
|                                                           |
|   Detector for Mountain keyboard                          |
|                                                           |
|   Wojciech Lazarski / Soufian Batta              Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "QMessageLogger"
#include "MountainKeyboardController.h"
#include "RGBController_MountainKeyboard.h"
#include "Mountain60KeyboardController.h"
#include "RGBController_Mountain60Keyboard.h"

/*---------------------------------------------------------------*\
| Mountain vendor ID                                              |
\*---------------------------------------------------------------*/
#define MOUNTAIN_VID                                        0x3282

/*----------------------------------------------------------------*\
| Everest 60 keyboard Connection IDs                               |
\*----------------------------------------------------------------*/
#define MOUNTAIN60_EVEREST_60_PID                           0x0005
#define MOUNTAIN60_EVEREST_60_INTERFACE                     2
#define MOUNTAIN60_EVEREST_60_U                             0x01
#define MOUNTAIN60_EVEREST_60_UP                            0xffff
/*----------------------------------------------------------------------------------------*\
|                                                                                          |
|   DetectMountainKeyboardControllers                                                      |
|                                                                                          |
|       Tests the USB address to see if a Mountain RGB Keyboard controller exists there.   |
|                                                                                          |
\*----------------------------------------------------------------------------------------*/

void DetectMountain60KeyboardControllers(hid_device_info* info, const std::string& name)
{
    QMessageLogger* log = new QMessageLogger();

    log->info("Hello World 2");
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        Mountain60KeyboardController*     controller     = new Mountain60KeyboardController(dev, info->path);
        RGBController_Mountain60Keyboard* rgb_controller = new RGBController_Mountain60Keyboard(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectMountainKeyboardControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        MountainKeyboardController*     controller     = new MountainKeyboardController(dev, info->path);
        RGBController_MountainKeyboard* rgb_controller = new RGBController_MountainKeyboard(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("Mountain Everest", DetectMountainKeyboardControllers, MOUNTAIN_VID, MOUNTAIN_EVEREST_PID, 3, 0xFF00, 0x01);
REGISTER_HID_DETECTOR_IPU("Mountain Everest 60", DetectMountain60KeyboardControllers, MOUNTAIN_VID, MOUNTAIN60_EVEREST_60_PID, MOUNTAIN60_EVEREST_60_INTERFACE, MOUNTAIN60_EVEREST_60_UP, MOUNTAIN60_EVEREST_60_U);
