/*---------------------------------------------------------*\
| CryorigH7QuadLumiControllerDetect.cpp                     |
|                                                           |
|   Detector for Cryorig H7 Quad Lumi                       |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      15 Apr 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "CryorigH7QuadLumiController.h"
#include "DetectionManager.h"
#include "RGBController_CryorigH7QuadLumi.h"

/*---------------------------------------------------------*\
| CRYORIG/NZXT USB IDs                                      |
\*---------------------------------------------------------*/
#define NZXT_VID                    0x1E71
#define CRYORIG_H7_QUAD_LUMI_PID    0x1712

DetectedControllers DetectCryorigH7QuadLumi(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        CryorigH7QuadLumiController*     controller     = new CryorigH7QuadLumiController(dev, info->path, name);
        RGBController_CryorigH7QuadLumi* rgb_controller = new RGBController_CryorigH7QuadLumi(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR("CRYORIG H7 Quad Lumi",   DetectCryorigH7QuadLumi, NZXT_VID,   CRYORIG_H7_QUAD_LUMI_PID);
