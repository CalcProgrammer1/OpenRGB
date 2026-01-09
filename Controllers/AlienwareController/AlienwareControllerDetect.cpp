/*---------------------------------------------------------*\
| AlienwareControllerDetect.cpp                             |
|                                                           |
|   Detector for Dell Alienware RGB USB controller          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "AlienwareController.h"
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

void DetectAlienwareControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AlienwareController*     controller     = new AlienwareController(dev, *info, name);
        RGBController_Alienware* rgb_controller = new RGBController_Alienware(controller);

        DetectionManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR("Dell G Series LED Controller", DetectAlienwareControllers, ALIENWARE_VID, ALIENWARE_G_SERIES_PID1);
REGISTER_HID_DETECTOR("Dell G Series LED Controller", DetectAlienwareControllers, ALIENWARE_VID, ALIENWARE_G_SERIES_PID2);
