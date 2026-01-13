/*---------------------------------------------------------*\
| ThermaltakePoseidonZRGBControllerDetect.cpp               |
|                                                           |
|   Detector for Thermaltake Poseidon Z RGB                 |
|                                                           |
|   Adam Honse (CalcProgrammer1)                25 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "RGBController_ThermaltakePoseidonZRGB.h"
#include "ThermaltakePoseidonZRGBController.h"

#define TT_POSEIDON_Z_RGB_VID 0x264A
#define TT_POSEIDON_Z_RGB_PID 0x3006

DetectedControllers DetectPoseidonZRGBControllers(hid_device_info* info, const std::string&)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        PoseidonZRGBController*     controller     = new PoseidonZRGBController(dev, info->path);
        RGBController_PoseidonZRGB* rgb_controller = new RGBController_PoseidonZRGB(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IP("Thermaltake Poseidon Z RGB", DetectPoseidonZRGBControllers, TT_POSEIDON_Z_RGB_VID, TT_POSEIDON_Z_RGB_PID, 1, 0xFF01);
