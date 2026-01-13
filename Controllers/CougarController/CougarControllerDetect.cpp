/*---------------------------------------------------------*\
| CougarControllerDetect.cpp                                |
|                                                           |
|   Detector for Cougar devices                             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "CougarKeyboardController.h"
#include "CougarRevengerSTController.h"
#include "DetectionManager.h"
#include "RGBController_CougarKeyboard.h"
#include "RGBController_CougarRevengerST.h"

/*---------------------------------------------------------*\
| Cougar vendor ID                                          |
\*---------------------------------------------------------*/
#define COUGAR_VID                                      0x12CF
#define COUGAR_VID_2                                    0x060B

/*---------------------------------------------------------*\
| Product ID                                                |
\*---------------------------------------------------------*/
#define COUGAR_700K_EVO_PID                             0x7010
#define COUGAR_REVENGER_ST_PID                          0x0412

DetectedControllers DetectCougarRevengerSTControllers(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        CougarRevengerSTController*     controller      = new CougarRevengerSTController(dev, *info, name);
        RGBController_CougarRevengerST* rgb_controller  = new RGBController_CougarRevengerST(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

DetectedControllers DetectCougar700kEvo(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        CougarKeyboardController*     controller        = new CougarKeyboardController(dev, info->path, name);
        RGBController_CougarKeyboard* rgb_controller    = new RGBController_CougarKeyboard(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IPU("Cougar 700K EVO Gaming Keyboard",        DetectCougar700kEvo,                COUGAR_VID_2,   COUGAR_700K_EVO_PID,    3, 0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cougar Revenger ST",                     DetectCougarRevengerSTControllers,  COUGAR_VID,     COUGAR_REVENGER_ST_PID, 0, 0x0001, 2);
