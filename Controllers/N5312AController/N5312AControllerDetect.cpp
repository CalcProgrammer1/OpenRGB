/*---------------------------------------------------------*\
| N5312AControllerDetect.cpp                                |
|                                                           |
|   Detector for N5312A                                     |
|                                                           |
|   Morgan Guimard (morg)                       02 Apr 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "N5312AController.h"
#include "RGBController_N5312A.h"

/*---------------------------------------------------------*\
| N5312A vendor ID                                          |
\*---------------------------------------------------------*/
#define N5312A_VID                                     0x4E53

/*---------------------------------------------------------*\
| Product ID                                                |
\*---------------------------------------------------------*/
#define N5312A_PID                                     0x5406

void DetectN5312AControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        N5312AController*     controller         = new N5312AController(dev, *info, name);
        RGBController_N5312A* rgb_controller     = new RGBController_N5312A(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("N5312A USB Optical Mouse", DetectN5312AControllers, N5312A_VID, N5312A_PID, 1, 0xFF01, 0x01);
