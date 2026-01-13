/*---------------------------------------------------------*\
| AlienwareMonitorControllerDetect.cpp                      |
|                                                           |
|   Detector for Alienware monitors                         |
|                                                           |
|   Adam Honse (CalcProgrammer1)                08 May 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "AlienwareAW3423DWFController.h"
#include "AlienwareMonitorController.h"
#include "DetectionManager.h"
#include "RGBController_AlienwareAW3423DWF.h"
#include "RGBController_AlienwareMonitor.h"

/*---------------------------------------------------------*\
| Alienware Vendor ID                                       |
\*---------------------------------------------------------*/
#define ALIENWARE_VID                               0x187C

/*---------------------------------------------------------*\
| Alienware Vendor ID                                       |
\*---------------------------------------------------------*/
#define ALIENWARE_AW3423DWF_PID                     0x100E
#define ALIENWARE_AW3225QF_PID                      0x1013
#define ALIENWARE_USAGE_PAGE                        0xFFDA
#define ALIENWARE_USAGE                             0x00DA

DetectedControllers DetectAlienwareAW3423DWFControllers(hid_device_info* info, const std::string&)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        AlienwareAW3423DWFController*     controller     = new AlienwareAW3423DWFController(dev, info->path);
        RGBController_AlienwareAW3423DWF* rgb_controller = new RGBController_AlienwareAW3423DWF(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

DetectedControllers DetectAlienwareMonitorControllers(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        AlienwareMonitorController*     controller     = new AlienwareMonitorController(dev, info->path, name);
        RGBController_AlienwareMonitor* rgb_controller = new RGBController_AlienwareMonitor(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR("Alienware AW3423DWF", DetectAlienwareAW3423DWFControllers, ALIENWARE_VID, ALIENWARE_AW3423DWF_PID);
REGISTER_HID_DETECTOR("Alienware AW3225QF", DetectAlienwareMonitorControllers, ALIENWARE_VID, ALIENWARE_AW3225QF_PID);
