/*---------------------------------------------------------*\
| GigabyteAorusCPUCoolerControllerDetect.cpp                |
|                                                           |
|   Detector for Gigabyte Aorus CPU coolers                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "ATC800Controller.h"
#include "RGBController_AorusATC800.h"

/*-----------------------------------------------------*\
| Vendor ID                                             |
\*-----------------------------------------------------*/
#define HOLTEK_VID                            0x1044

/*-----------------------------------------------------*\
| Controller product ids                                |
\*-----------------------------------------------------*/
#define ATC_800_CONTROLLER_PID                0x7A42

/******************************************************************************************\
*                                                                                          *
*   DetectAorusCPUCoolerControllers                                                        *
*                                                                                          *
*       Tests the USB address to see if a Aorus RGB CPU Cooler exists there.               *
*                                                                                          *
\******************************************************************************************/

void DetectGigabyteAorusCPUCoolerControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        ATC800Controller*          controller     = new ATC800Controller(dev, info->path, name);
        RGBController_AorusATC800* rgb_controller = new RGBController_AorusATC800(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("Gigabyte AORUS ATC800", DetectGigabyteAorusCPUCoolerControllers, HOLTEK_VID, ATC_800_CONTROLLER_PID, 0, 0xFF01, 1);
