/*---------------------------------------------------------*\
| CreativeControllerDetect.cpp                              |
|                                                           |
|   Detector for Creative devices                           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "CreativeSoundBlasterXG6Controller.h"
#include "DetectionManager.h"
#include "RGBController_CreativeSoundBlasterXG6.h"

/*---------------------------------------------------------*\
| Creative vendor ID                                        |
\*---------------------------------------------------------*/
#define CREATIVE_VID                            0x041E

/*---------------------------------------------------------*\
| SoundCards                                                |
\*---------------------------------------------------------*/
#define CREATIVE_SOUNDBLASTERX_G6_PID           0x3256

DetectedControllers DetectCreativeDevice(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        CreativeSoundBlasterXG6Controller*     controller     = new CreativeSoundBlasterXG6Controller(dev, info->path, name);
        RGBController_CreativeSoundBlasterXG6* rgb_controller = new RGBController_CreativeSoundBlasterXG6(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------*\
| Sound Cards                                                                                                                                       |
\*-------------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_I("Creative SoundBlasterX G6",               DetectCreativeDevice,         CREATIVE_VID,     CREATIVE_SOUNDBLASTERX_G6_PID, 4);
