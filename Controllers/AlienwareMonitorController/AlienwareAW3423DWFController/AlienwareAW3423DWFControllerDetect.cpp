/*---------------------------------------------------------*\
| AlienwareAW3423DWFControllerDetect.cpp                    |
|                                                           |
|   Detector for Alienware AW3423DWF monitor                |
|                                                           |
|   Ferréol DUBOIS COLI (Fefe_du_973)           23 Jan 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "AlienwareAW3423DWFController.h"
#include "DetectionManager.h"
#include "RGBController_AlienwareAW3423DWF.h"

/*---------------------------------------------------------*\
| Alienware Vendor ID                                       |
\*---------------------------------------------------------*/
#define ALIENWARE_VID                               0x187C

/*---------------------------------------------------------*\
| Alienware Vendor ID                                       |
\*---------------------------------------------------------*/
#define ALIENWARE_AW3423DWF_PID                     0x100E
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

REGISTER_HID_DETECTOR("Alienware AW3423DWF", DetectAlienwareAW3423DWFControllers, ALIENWARE_VID, ALIENWARE_AW3423DWF_PID);
