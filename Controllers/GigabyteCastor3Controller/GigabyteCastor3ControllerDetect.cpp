/*---------------------------------------------------------*\
|  GigabyteCastor3ControllerDetect.cpp                      |
|                                                           |
|  Detector for Gigabyte Aorus Waterforce X II 360 AIO      |
|  (Castor3 USB HID controller)                             |
|                                                           |
|  This file is part of the OpenRGB project                 |
|  SPDX-License-Identifier: GPL-2.0-or-later                |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "GigabyteCastor3Controller.h"
#include "RGBController_GigabyteCastor3.h"

#define GIGABYTE_CASTOR3_VID            0x0414
#define GIGABYTE_CASTOR3_PID            0x7A5E

DetectedControllers DetectGigabyteCastor3Controllers(hid_device_info* info, const std::string& /*name*/)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        GigabyteCastor3Controller*      controller      = new GigabyteCastor3Controller(dev, info->path);
        RGBController_GigabyteCastor3*  rgb_controller  = new RGBController_GigabyteCastor3(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IPU("Gigabyte Aorus Waterforce X II 360", DetectGigabyteCastor3Controllers, GIGABYTE_CASTOR3_VID, GIGABYTE_CASTOR3_PID, 0, 0x0000, 0x0002);
