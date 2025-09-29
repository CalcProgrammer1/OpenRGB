/*---------------------------------------------------------*\
| QMKVialRGBControllerDetect.cpp                            |
|                                                           |
|   Detector for VialRGB QMK Keyboard Protocol              |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      29 Sep 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <string>
#include <hidapi.h>
#include "Detector.h"
#include "QMKVialRGBController.h"
#include "RGBController_QMKVialRGB.h"

/*-----------------------------------------------------*\
| USB IDs                                               |
\*-----------------------------------------------------*/
#define RASPBERRY_PI_VID                        0x2E8A
#define RASPBERRY_PI_500_PLUS_PID               0x0011

/*-----------------------------------------------------*\
| Usage and Usage Page                                  |
\*-----------------------------------------------------*/
#define QMK_USAGE_PAGE                          0xFF60
#define QMK_USAGE                               0x61

void DetectQMKVialRGBControllers(hid_device_info *info, const std::string&)
{
    hid_device *dev = hid_open_path(info->path);

    if(dev)
    {
        QMKVialRGBController*     controller     = new QMKVialRGBController(dev, info->path);
        RGBController_QMKVialRGB* rgb_controller = new RGBController_QMKVialRGB(controller);
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_PU( "Raspberry Pi 500+", DetectQMKVialRGBControllers, RASPBERRY_PI_VID, RASPBERRY_PI_500_PLUS_PID, QMK_USAGE_PAGE, QMK_USAGE );
