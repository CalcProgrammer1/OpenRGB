/*---------------------------------------------------------*\
| SonyGamepadControllerDetect.cpp                           |
|                                                           |
|   Detector for Sony Gamepads                              |
|                                                           |
|   Pol Rius (alpemwarrior)                     24 Sep 2020 |
|   Flora Aubry                                 01 Jul 2022 |
|   Yoan Berthelot                              06 Mar 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <vector>
#include <hidapi.h>
#include "RGBController_SonyDS4.h"
#include "RGBController_SonyDualSense.h"
#include "Detector.h"

#define SONY_VID                        0x054C

#define SONY_DS4_V1_PID                 0x05C4
#define SONY_DS4_V2_PID                 0x09CC
#define SONY_DS4_RECEIVER_PID           0x0BA0
#define SONY_DUALSENSE_PID              0x0CE6
#define SONY_DUALSENSE_EDGE_PID         0x0DF2

void DetectSonyDS4Controllers(hid_device_info* info, const std::string&)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        SonyDS4Controller*      controller      = new SonyDS4Controller(dev, info->path);
        RGBController_SonyDS4*  rgb_controller  = new RGBController_SonyDS4(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectSonyDualSenseControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        bool is_bluetooth = info->interface_number    == -1;
        SonyDualSenseController*      controller      = new SonyDualSenseController(dev, info->path, is_bluetooth, name);
        RGBController_SonyDualSense*  rgb_controller  = new RGBController_SonyDualSense(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR("Sony DualShock 4", DetectSonyDS4Controllers,       SONY_VID, SONY_DS4_V1_PID);
REGISTER_HID_DETECTOR("Sony DualShock 4", DetectSonyDS4Controllers,       SONY_VID, SONY_DS4_V2_PID);
REGISTER_HID_DETECTOR("Sony DualShock 4", DetectSonyDS4Controllers,       SONY_VID, SONY_DS4_RECEIVER_PID);
REGISTER_HID_DETECTOR("Sony DualSense",   DetectSonyDualSenseControllers, SONY_VID, SONY_DUALSENSE_PID);
REGISTER_HID_DETECTOR("Sony DualSense Edge",   DetectSonyDualSenseControllers, SONY_VID, SONY_DUALSENSE_EDGE_PID);
