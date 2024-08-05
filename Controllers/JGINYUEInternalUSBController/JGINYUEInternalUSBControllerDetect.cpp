/*---------------------------------------------------------*\
| JGINYUEInternalUSBControllerDetect.cpp                    |
|                                                           |
|   Detector for JGINYUE USB motherboard                    |
|                                                           |
|   Tong R (tcr020)                             09 Aug 2023 |
|   Liu ShiMeng(Moon dream stars)               09 Aug 2023 |
|   Dongguan Yonghang Electronic Technology Co., Ltd        |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <hidapi.h>
#include "RGBController_JGINYUEInternalUSB.h"
#include "JGINYUEInternalUSBController.h"
#include "RGBController.h"
#include "Detector.h"

/*---------------------------------------------------------*\
| JGINYUE vendor ID                                         |
\*---------------------------------------------------------*/
#define JGINYUE_VID                                 0x0416

/*---------------------------------------------------------*\
| JGINYUE product ID                                        |
\*---------------------------------------------------------*/
#define JGINYUE_MOTHERBOARD_PID                     0xA125

void DetectJGINYUEInternalUSBController(hid_device_info* info,const std::string& /*name*/)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        JGINYUEInternalUSBController*       controller      =new JGINYUEInternalUSBController(dev,info->path);
        RGBController_JGINYUEInternalUSB*   rgb_controller  =new RGBController_JGINYUEInternalUSB(controller);
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

#ifdef _WIN32
REGISTER_HID_DETECTOR("JGINYUE Internal USB Controller", DetectJGINYUEInternalUSBController, JGINYUE_VID, JGINYUE_MOTHERBOARD_PID);
#else
REGISTER_HID_DETECTOR_IPU("JGINYUE Internal USB Controller", DetectJGINYUEInternalUSBController, JGINYUE_VID, JGINYUE_MOTHERBOARD_PID, 0, 0xFF00, 1);
#endif
