/*---------------------------------------------------------*\
| DreamCheekyControllerDetect.cpp                           |
|                                                           |
|   Detector for Dream Cheeky devices                       |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      06 Sep 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "DreamCheekyController.h"
#include "RGBController_DreamCheeky.h"

/*---------------------------------------------------------*\
| Dream Cheeky USB Vendor ID                                |
\*---------------------------------------------------------*/
#define DREAM_CHEEKY_VID                            0x1D34

/*---------------------------------------------------------*\
| Dream Cheeky USB Product ID                               |
\*---------------------------------------------------------*/
#define DREAM_CHEEKY_WEBMAIL_NOTIFIER_PID           0x0004

DetectedControllers DetectDreamCheekyControllers(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        DreamCheekyController*     controller     = new DreamCheekyController(dev, info->path, name);
        RGBController_DreamCheeky* rgb_controller = new RGBController_DreamCheeky(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR( "Dream Cheeky Webmail Notifier", DetectDreamCheekyControllers, DREAM_CHEEKY_VID, DREAM_CHEEKY_WEBMAIL_NOTIFIER_PID );
