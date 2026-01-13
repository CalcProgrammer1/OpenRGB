/*---------------------------------------------------------*\
| MadCatzCyborgControllerDetect.cpp                         |
|                                                           |
|   Detector for MadCatz Cyborg Gaming Light                |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "MadCatzCyborgController.h"
#include "RGBController_MadCatzCyborg.h"

/*---------------------------------------------------------*\
| MadCatz Cyborg VID/PID                                    |
\*---------------------------------------------------------*/
#define MADCATZ_VID        0x06A3
#define MADCATZ_CYBORG_PID 0x0DC5

DetectedControllers DetectMadCatzCyborgControllers(hid_device_info* info, const std::string&  /*name*/)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        MadCatzCyborgController*     controller     = new MadCatzCyborgController(dev, info->path);
        controller->Initialize();

        RGBController_MadCatzCyborg* rgb_controller = new RGBController_MadCatzCyborg(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR("MadCatz Cyborg Gaming Light", DetectMadCatzCyborgControllers, MADCATZ_VID, MADCATZ_CYBORG_PID);
