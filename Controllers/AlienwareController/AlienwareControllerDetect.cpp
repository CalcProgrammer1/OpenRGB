/*---------------------------------------------------------*\
| AlienwareControllerDetect.cpp                             |
|                                                           |
|   Detector for Dell Alienware RGB USB controller          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "AlienwareController.h"
#include "DetectionManager.h"
#include "RGBController_Alienware.h"

/*---------------------------------------------------------*\
| Alienware vendor ID                                       |
\*---------------------------------------------------------*/
#define ALIENWARE_VID                               0x187C

/*---------------------------------------------------------*\
| Alienware product ID                                      |
\*---------------------------------------------------------*/
#define ALIENWARE_G_SERIES_PID1                      0x0550
#define ALIENWARE_G_SERIES_PID2                      0x0551

DetectedControllers DetectAlienwareControllers(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        AlienwareController*     controller     = new AlienwareController(dev, *info, name);
        RGBController_Alienware* rgb_controller = new RGBController_Alienware(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR("Dell G Series LED Controller", DetectAlienwareControllers, ALIENWARE_VID, ALIENWARE_G_SERIES_PID1);
REGISTER_HID_DETECTOR("Dell G Series LED Controller", DetectAlienwareControllers, ALIENWARE_VID, ALIENWARE_G_SERIES_PID2);
