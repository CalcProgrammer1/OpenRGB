/*---------------------------------------------------------*\
| LenovoMotherboardControllerDetect.cpp                     |
|                                                           |
|   Detector for Lenovo motherboard                         |
|                                                           |
|   Morgan Guimard (morg)                       26 Dec 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "dmiinfo.h"
#include "LenovoMotherboardController.h"
#include "RGBController_LenovoMotherboard.h"

/*---------------------------------------------------------*\
| vendor ID                                                 |
\*---------------------------------------------------------*/
#define LENOVO_MB_VID                                  0x17EF

/*---------------------------------------------------------*\
| Product ID                                                |
\*---------------------------------------------------------*/
#define LENOVO_MB_PID                                  0xC955

DetectedControllers DetectLenovoMotherboardControllers(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        DMIInfo dmi;

        LenovoMotherboardController*     controller         = new LenovoMotherboardController(dev, *info, name + " " + dmi.getMainboard());
        RGBController_LenovoMotherboard* rgb_controller     = new RGBController_LenovoMotherboard(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_PU("Lenovo", DetectLenovoMotherboardControllers, LENOVO_MB_VID, LENOVO_MB_PID, 0xFF89, 0xCC);
