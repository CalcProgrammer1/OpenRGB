/*---------------------------------------------------------*\
| RedSquareKeyroxControllerDetect.cpp                       |
|                                                           |
|   Detector for Red Square Keyrox                          |
|                                                           |
|   cafeed28                                    03 Nov 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "RedSquareKeyroxController.h"
#include "RGBController_RedSquareKeyrox.h"

/*---------------------------------------------------------*\
| Red Square vendor ID                                      |
\*---------------------------------------------------------*/
#define RED_SQUARE_VID                             0x1A2C

/*---------------------------------------------------------*\
| Red Square product ID                                     |
\*---------------------------------------------------------*/
#define RED_SQUARE_KEYROX_TKL_PID                  0x1511
#define RED_SQUARE_KEYROX_TKL_V2_PID               0x2511

DetectedControllers DetectRedSquareKeyroxTKL(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        RedSquareKeyroxController*     controller     = new RedSquareKeyroxController(dev, *info, KEYROX_VARIANT_TKL, name);
        RGBController_RedSquareKeyrox* rgb_controller = new RGBController_RedSquareKeyrox(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IPU("Red Square Keyrox TKL",          DetectRedSquareKeyroxTKL,           RED_SQUARE_VID,                     RED_SQUARE_KEYROX_TKL_PID,          3, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Red Square Keyrox TKL V2",       DetectRedSquareKeyroxTKL,           RED_SQUARE_VID,                     RED_SQUARE_KEYROX_TKL_V2_PID,       3, 0xFF00, 2);
