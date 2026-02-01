/*---------------------------------------------------------*\
| TecknetControllerDetect.cpp                               |
|                                                           |
|   Detector for Tecknet devices                            |
|                                                           |
|   Chris M (Dr_No)                             29 Jul 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "TecknetController.h"
#include "RGBController_Tecknet.h"

#define TECKNET_VID             0x04D9

#define TECKNET_M0008_PID       0xFC05
#define TECKNET_M0008_U         0x01        //Usage 01
#define TECKNET_M0008_UPG       0xFFA0      //Vendor Defined Usage Page

DetectedControllers DetectTecknetControllers(hid_device_info* info, const std::string&)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        TecknetController*     controller     = new TecknetController(dev, info->path);
        RGBController_Tecknet* rgb_controller = new RGBController_Tecknet(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_PU("Tecknet M008", DetectTecknetControllers, TECKNET_VID, TECKNET_M0008_PID, TECKNET_M0008_UPG, TECKNET_M0008_U);
