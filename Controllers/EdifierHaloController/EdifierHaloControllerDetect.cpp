/*---------------------------------------------------------*\
| EdifierHaloControllerDetect.cpp                           |
|                                                           |
|   Detector for Edifier Halo PixelBar USB/HID devices      |
|                                                           |
|   Bartholomew Ho <qiangqiang101@hotmail.com>  20 Jul 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include <memory>
#include "DetectionManager.h"
#include "LogManager.h"
#include "RGBController_EdifierHaloPixelbarAmbient.h"
#include "RGBController_EdifierHaloPixelbarScreen.h"
#include "EdifierHaloController.h"

/*---------------------------------------------------------*\
| Edifier HID vendor and product IDs                        |
\*---------------------------------------------------------*/
#define EDIFIER_VID                     0x2D99
#define EDIFIER_PIXELBAR_PID            0xA106
#define EDIFIER_PIXELBAR_INTERFACE      4
#define EDIFIER_PIXELBAR_USAGE_PAGE     0xFF14
#define EDIFIER_PIXELBAR_USAGE          0x01

DetectedControllers DetectEdifierHaloControllers(hid_device_info* info, const std::string&)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        EdifierHaloController* controller = new EdifierHaloController(dev, info->path);

        RGBController_EdifierHaloPixelbarAmbient* ambient_controller = new RGBController_EdifierHaloPixelbarAmbient(controller);
        RGBController_EdifierHaloPixelbarScreen* screen_controller = new RGBController_EdifierHaloPixelbarScreen(controller);

        detected_controllers.push_back(ambient_controller);
        detected_controllers.push_back(screen_controller);

        LOG_INFO("[EdifierHaloPixelbarDetect] Detected Edifier Halo PixelBar at %s (Instantiated Ambient & Screen controllers)", info->path);
    }
    else
    {
        LOG_WARNING("[EdifierHaloPixelbarDetect] Failed to open Edifier Halo PixelBar at %s", info->path);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IPU("Edifier Halo Pixelbar", DetectEdifierHaloControllers, EDIFIER_VID, EDIFIER_PIXELBAR_PID, EDIFIER_PIXELBAR_INTERFACE, EDIFIER_PIXELBAR_USAGE_PAGE, EDIFIER_PIXELBAR_USAGE);
